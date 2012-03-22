
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
#include "lpc3250_loader.h"

int
main (int argc, char *argv[], char *env[])
{
  int               i;
  config_t          config;
  char              def_conf_file_n[] = CONFIG_FILE_NAME;
  char             *conf_file_n = 0;
  wordexp_t         we;
  int               port_fd;
  int               opt = 0;
  int               long_index = 0;

  static const struct option long_opt[] = {
    {"help", no_argument, NULL, 'h'},
    {"config", required_argument, NULL, 'c'},
    {NULL, no_argument, NULL, 0}
  };
  const char       *short_opt_s = "hc:";

  printf ("lpc3250_loader\n");
  opt = getopt_long (argc, argv, short_opt_s, long_opt, &long_index);
  while (opt != -1)
    {
      switch (opt)
        {
        case 'h':
          {
            printf
              ("Usage:\n -h (--help) this output\n -c (--config) file use alternate config file\n");
            exit (1);
            break;
          }
        case 'c':
          {
            conf_file_n = optarg;
            break;
          }
        }
      opt = getopt_long (argc, argv, short_opt_s, long_opt, &long_index);
    }

  if (!conf_file_n)
    conf_file_n = def_conf_file_n;

  wordexp (conf_file_n, &we, 0);
  strcpy (conf_file_n, we.we_wordv[0]);
  wordfree (&we);

  printf ("Use config: %s\n\n", conf_file_n);

  if (!read_config_yaml (conf_file_n, &config))
    {
      printf ("error: config not readed\n");
      return 1;
    }

  printf ("Port: %s\n", config.port);
  for (i = 0; i < config.qty_exec; i++)
    {
      printf ("i: %i\n", i);
      printf ("PrimaryFileName: %s\n", config.executables[i].primary_filename);
      printf ("IRAMaddress: %x\n", config.executables[i].iram_address);
      printf ("SecondaryFileName: %s\n", config.executables[i].secondary_filename);
      printf ("SDRAMaddress: %x\n", config.executables[i].sdram_address);
    }
  printf ("\n");
  port_fd = open (config.port, O_RDWR | O_NOCTTY | O_NDELAY);

  setup_port (port_fd);

  for (i = 0; i < config.qty_exec; i++)
    {
      if (!wait_byte (port_fd, '5', 1, 0))
        return 1;
      send_byte (port_fd, 'A');
      if (!wait_byte (port_fd, '5', 0, 0))
        return 1;
      send_byte (port_fd, 'U');
      send_byte (port_fd, '3');
      if (!wait_byte (port_fd, 'R', 0, 0))
        return 1;

      send_file_to_port (port_fd, config.executables[i].primary_filename,
                         config.executables[i].iram_address, 0);
      if (!wait_byte (port_fd, 'X', 0, 0))
        return 1;
      send_byte (port_fd, 'p');
      send_file_to_port (port_fd, config.executables[i].secondary_filename,
                         config.executables[i].sdram_address, 'o');
      if (!wait_byte (port_fd, 't', 0, 0))
        return 1;

      while (!wait_byte (port_fd, '5', 0, 1))
        {
          usleep (500000);
        }
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
wait_byte (int port_fd, char byte, int skip, int prnt_char)
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
  if (!prnt_char)
    printf ("Waiting for '%s' ... ", byte_s);
  poller.fd = port_fd;
  poller.events = POLLIN;
  poller.revents = 0;
  ok = poll (&poller, 1, wait_time);

  if (ok == 0)
    {
      if (!prnt_char)
        printf ("error poll\n");
      return 0;
    }

  ioctl (port_fd, FIONREAD, &bytes);

  for (i = 0; i < bytes; i++)
    {
      read (port_fd, answer, 1);
      if (prnt_char)
        putchar (answer[0]);
    }

  if ((answer[0] == byte) && skip)
    {
      usleep (500000);
      ioctl (port_fd, FIONREAD, &bytes);
      for (i = 0; i < bytes; i++)
        {
          read (port_fd, answer, 1);
          if (prnt_char)
            putchar (answer[0]);
        }
    }

  if (answer[0] != byte)
    {
      if (!prnt_char)
        printf ("error answer\n");
      return 0;
    }
  if (!prnt_char)
    printf ("ok\n");
  return 1;
}

int
send_byte (int port_fd, char byte)
{
  int               send;
  char              byte_s[2];

  byte_s[0] = byte;
  byte_s[1] = 0;
  printf ("Sending '%s' ... ", byte_s);
  send = write (port_fd, &byte, 1);
  if (send == 0)
    {
      printf ("error\n");
      return 0;
    }
  printf ("ok\n");
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
      write (port_fd, &byte_to_send, 1);
      tmp = tmp >> 8;
    }

  return 1;
}

int
send_file_to_port (int port_fd, char *file_name, int addr, char confirm)
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
  fstat (fileno (f), &stat_file);
  file_size = stat_file.st_size;
  buf = malloc (file_size);
  fread (buf, file_size, 1, f);
  fclose (f);

  send_4_bytes_reverse (port_fd, addr);
  send_4_bytes_reverse (port_fd, file_size);

  if (confirm)
    {
      wait_byte (port_fd, confirm, 0, 0);
    }

  tmp = 0;
  i = 0;

  printf ("Sending %s ", we.we_wordv[0]);
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
      printf (".");
      file_size -= tmp;
    }

  free (buf);
  printf (" ok\n");

  return 1;
}
