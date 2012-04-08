
/**
 * @file   lpc3250_loader.c
 * @author  <yakor.spb@gmail.com>
 * @date   Thu Mar 15 19:27:35 2012
 *
 * @brief lpc3250 loader
 *
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <wordexp.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include "read_config.h"
#include "y-lpc-loader.h"
#include "version.h"

int
main (int argc, char *argv[], char *env[])
{
  int               i;
  config_t          config;
  char             *conf_file_n = 0;
  char             *conf_file_option = 0;
  wordexp_t         we;
  int               port_fd;
  int               opt = 0;
  int               long_index = 0;
  char             *primary_filename = 0;
  char             *secondary_filename = 0;
  char             *port = 0;
  int               iram_address = -1;
  int               sdram_address = -1;
  int               conf_ok = 0;
  int               prnt_all_char = 0;
  executables_t    *ex;

  static const struct option long_opt[] = {
    {"help", no_argument, NULL, 'h'},
    {"config", required_argument, NULL, 'c'},
    {"first", required_argument, NULL, 'f'},
    {"second", required_argument, NULL, 's'},
    {"primary-address", required_argument, NULL, 'i'},
    {"secondary-address", required_argument, NULL, 'd'},
    {"port", required_argument, NULL, 'p'},
    {"print-all-char", no_argument, NULL, 'a'},
    {NULL, no_argument, NULL, 0}
  };
  const char       *short_opt_s = "hc:p:s:i:d:f:a";

  printf (Y_LPC_LOADER_FULL_NAME "\n\n");
  opt = getopt_long (argc, argv, short_opt_s, long_opt, &long_index);
  while (opt != -1)
    {
      switch (opt)
        {
        case 'h':
          {
            printf (HELP_STRING);
            exit (1);
            break;
          }
        case 'c':
          {
            conf_file_option = optarg;
            break;
          }
        case 'f':
          {
            primary_filename = optarg;
            break;
          }
        case 's':
          {
            secondary_filename = optarg;
            break;
          }
        case 'i':
          {
            iram_address = strtol ((char *) optarg, NULL, 0);
            break;
          }
        case 'd':
          {
            sdram_address = strtol ((char *) optarg, NULL, 0);
            break;
          }
        case 'p':
          {
            port = optarg;
            break;
          }
        case 'a':
          {
            prnt_all_char = 1;
            break;
          }
        }
      opt = getopt_long (argc, argv, short_opt_s, long_opt, &long_index);
    }

  if (!conf_file_option)
    create_str (&conf_file_n, DEFAULT_CONFIG_FILE_NAME);
  else
    create_str (&conf_file_n, conf_file_option);

  wordexp (conf_file_n, &we, 0);
  create_str (&conf_file_n, we.we_wordv[0]);
  wordfree (&we);

  printf ("Using config: %s\nReading config ... ", conf_file_n);

  if (!read_config_yaml (conf_file_n, &config))
    {
      printf ("error\n");
    }
  else
    {
      printf ("ok\n");
      conf_ok = 1;
    }

  prnt_all_char |= config.prnt_all_char;

  if (port)
    {
      config.port = port;
    }

  if (!config.port)
    {
      create_str (&config.port, DEFAULT_PORT);
      printf ("Port not specified. Use default\n");
    }

  if (primary_filename)
    {
      printf ("Found manualy specified first execurable file\nExecutables from config ignored\n");
      config.qty_exec = 1;
      free (config.executables);
      config.executables = malloc (sizeof (config_t));
      default_setting (config.executables);
      config.executables[0].primary_filename = primary_filename;
      if (iram_address != -1)
        config.executables[0].iram_address = iram_address;
      if (secondary_filename)
        config.executables[0].secondary_filename = secondary_filename;
      if (sdram_address != -1)
        config.executables[0].sdram_address = sdram_address;
    }

  printf ("\nPort: %s\n", config.port);
  printf ("PrintAllChar: %i\n", prnt_all_char);
  for (i = 0; i < config.qty_exec; i++)
    {
      printf ("%i\n", i + 1);
      printf ("   PrimaryFileName: %s\n", config.executables[i].primary_filename);
      printf ("   IRAMaddress: %x\n", config.executables[i].iram_address);
      if (config.executables[i].secondary_filename)
        {
          printf ("   SecondaryFileName: %s\n", config.executables[i].secondary_filename);
          printf ("   SDRAMaddress: %x\n", config.executables[i].sdram_address);
        }
    }
  printf ("\n");
  port_fd = open (config.port, O_RDWR | O_NOCTTY | O_NDELAY);

  setup_port (port_fd);

  for (i = 0; i < config.qty_exec; i++)
    {
      ex = &config.executables[i];

      if (!wait_byte (port_fd, '5', 1, prnt_all_char, 0))
        return 1;
      send_byte (port_fd, 'A', prnt_all_char);
      if (!wait_byte (port_fd, '5', 0, prnt_all_char, 0))
        return 1;
      send_byte (port_fd, 'U', prnt_all_char);
      send_byte (port_fd, '3', prnt_all_char);
      if (!wait_byte (port_fd, 'R', 0, prnt_all_char, 0))
        return 1;

      if (!send_file_to_port (port_fd, ex->primary_filename, ex->iram_address, 0, prnt_all_char))
        return 1;

      if (ex->secondary_filename)
        {
          if (!wait_byte (port_fd, 'X', 0, prnt_all_char, 0))
            return 1;
          send_byte (port_fd, 'p', prnt_all_char);
          if (!send_file_to_port
              (port_fd, ex->secondary_filename, ex->sdram_address, 'o', prnt_all_char))
            return 1;
          if (!wait_byte (port_fd, 't', 0, prnt_all_char, 0))
            return 1;
        }

      while (!wait_byte (port_fd, '5', 0, 1, 1))
        {
          usleep (500000);
        }
      printf ("\n");
    }

  return 0;
}

int
setup_port (int port_fd)
{
  struct termios    tio;

  tcgetattr (port_fd, &tio);
  cfsetispeed (&tio, B115200);
  cfsetospeed (&tio, B115200);
  tio.c_cflag &= ~PARENB;
  tio.c_cflag &= ~CSTOPB;
  tio.c_cflag &= ~CSIZE;
  tio.c_cflag |= CS8;
  tio.c_cflag &= ~CRTSCTS;
  tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  tio.c_oflag &= ~OPOST;

  tcflush (port_fd, TCOFLUSH);
  tcflush (port_fd, TCIFLUSH);
  tcsetattr (port_fd, TCIFLUSH, &tio);
  return 1;
}

int
wait_byte (int port_fd, char byte, int skip, int prnt_all_char, int pure_output)
{
  struct pollfd     poller;
  int               bytes;
  char              answer[2];
  int               ok;
  int               i;
  int               wait_time;
  char              byte_s[2];

  wait_time = 1000;
  if (skip)
    wait_time = 10000;

  ok = 0;
  answer[1] = 0;
  byte_s[0] = byte;
  byte_s[1] = 0;
  if (!prnt_all_char)
    printf ("Waiting for '%s' ... ", byte_s);
  poller.fd = port_fd;
  poller.events = POLLIN;
  poller.revents = 0;
  ok = poll (&poller, 1, wait_time);

  if (ok == 0)
    {
      if (!prnt_all_char)
        printf ("error poll\n");
      return 0;
    }

  ioctl (port_fd, FIONREAD, &bytes);

  if (!skip && !pure_output)
    bytes = 1;

  for (i = 0; i < bytes; i++)
    {
      read (port_fd, answer, 1);
      if (prnt_all_char)
        {
          if (pure_output)
            printf ("%s", answer);
          else
            printf ("< %s\n", answer);
        }
    }

  if ((answer[0] == byte) && skip)
    {
      usleep (500000);
      ioctl (port_fd, FIONREAD, &bytes);
      for (i = 0; i < bytes; i++)
        {
          if (prnt_all_char)
            {
              read (port_fd, answer, 1);
              if (pure_output)
                printf ("%s", answer);
              else
                printf ("< %s\n", answer);
            }
        }
    }

  if (answer[0] != byte)
    {
      if (!prnt_all_char)
        printf ("error answer\n");
      return 0;
    }
  if (!prnt_all_char)
    printf ("ok\n");
  return 1;
}

int
send_byte (int port_fd, char byte, int prnt_all_char)
{
  int               send;
  char              byte_s[2];

  byte_s[0] = byte;
  byte_s[1] = 0;
  if (!prnt_all_char)
    printf ("Sending '%s' ... ", byte_s);
  send = write (port_fd, &byte, 1);
  if (send == 0)
    {
      if (!prnt_all_char)
        printf ("error\n");
      return 0;
    }
  if (!prnt_all_char)
    printf ("ok\n");
  else
    printf ("> %s\n", byte_s);

  return 1;
}

int
send_4_bytes_reverse (int port_fd, int num)
{
  int               i;
  int               tmp;
  char              byte_to_send;

  tmp = num;

  for (i = 0; i < 4; i++)
    {
      byte_to_send = tmp & 0xff;
      if (write (port_fd, &byte_to_send, 1) != 1)
        {
          printf ("error write to port\n");
          return 0;
        }
      tmp = tmp >> 8;
    }

  return 1;
}

int
send_file_to_port (int port_fd, char *file_name, int addr, char confirm, int prnt_all_char)
{
  FILE             *f;
  struct stat       stat_file;
  char             *buf;
  int               file_size;
  int               i;
  int               tmp;
  wordexp_t         we;

  wordexp (file_name, &we, 0);
  f = fopen (we.we_wordv[0], "r");
  if (!f)
    {
      printf ("Error opening %s\n", file_name);
      return 0;
    }
  fstat (fileno (f), &stat_file);
  file_size = stat_file.st_size;
  buf = malloc (file_size);
  fread (buf, file_size, 1, f);
  fclose (f);

  if (!send_4_bytes_reverse (port_fd, addr))
    return 0;
  if (!send_4_bytes_reverse (port_fd, file_size))
    return 0;

  if (confirm)
    {
      if (!wait_byte (port_fd, confirm, 0, prnt_all_char, 0))
        return 0;
    }

  tmp = 0;
  i = 0;
  if (!prnt_all_char)
    printf ("Sending %s ", we.we_wordv[0]);
  else
    printf ("> {%s}\n", we.we_wordv[0]);
  wordfree (&we);
  while (file_size > 0 && tmp >= 0)
    {
      tmp = write (port_fd, &buf[i], file_size);
      usleep (50000);
      if (tmp < 0)
        {
          if (errno == EAGAIN)
            {
              tmp = 0;
            }
          else
            {
              printf ("error write to port\n");
              free (buf);
              return 0;
            }
        }
      i += tmp;
      if (!prnt_all_char)
        printf (".");
      file_size -= tmp;
    }

  free (buf);
  if (!prnt_all_char)
    printf (" ok\n");

  return 1;
}

int
create_str (char **dest, const char *source)
{
  int               len;

  free (*dest);
  len = strlen (source);
  *dest = malloc (len + 1);
  strncpy (*dest, source, len);

  return 1;
}
