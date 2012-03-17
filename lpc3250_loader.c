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
#include "read_config.h"
#include "lpc3250_loader.h"

int
main (int argc, char *argv[], char *env[])
{
  int               i;
  config_t          config;
  char              conf_file_n[200];
  wordexp_t         we;
  int               port_fd;


  sprintf (conf_file_n, CONFIG_FILE_NAME);
  wordexp (conf_file_n, &we, 0);
  strcpy (conf_file_n, we.we_wordv[0]);
  wordfree (&we);

  printf ("lpc3250_loader\n");
  printf ("Use config: %s\n\n", conf_file_n);

  read_config_yaml (conf_file_n, &config);

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
  if (wait_byte (port_fd, '5', 1))
    printf ("'5'\n");
  send_byte (port_fd, 'A');
  if (wait_byte (port_fd, '5', 0))
    printf ("'5'\n");
  send_byte (port_fd, 'U');
  send_byte (port_fd, '3');
  if (wait_byte (port_fd, 'R', 0))
    printf ("'R'\n");

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
  tio.c_iflag = IGNBRK;
  tio.c_oflag = 0;
  tio.c_lflag = 0;
  tio.c_cc[VTIME] = 5;

  tcflush (port_fd, TCOFLUSH);
  tcflush (port_fd, TCIFLUSH);
  tcsetattr (port_fd, TCIFLUSH, &tio);
  return 1;
}

int
wait_byte (int port_fd, char byte, int skip)
{
  struct pollfd     poller;
  int               bytes;
  char              answer[2];
  int               ok;
  int               i;
  int               wait_time;
  char              byte_s[2];

  wait_time = 100;
  if (skip)
    wait_time = 10000;

  ok = 0;
  answer[1] = 0;
  byte_s[0] = byte;
  printf ("Waiting for '%s' ... ", byte_s);
  poller.fd = port_fd;
  poller.events = POLLIN;
  poller.revents = 0;
  ok = poll (&poller, 1, wait_time);

  if (ok == 0)
    {
      printf ("error\n");
      return 0;
    }

  ioctl (port_fd, FIONREAD, &bytes);

  for (i = 0; i < bytes; i++)
    {
      read (port_fd, answer, 1);
    }

  if ((answer[0] == byte) && skip)
    {
      usleep (500000);
      ioctl (port_fd, FIONREAD, &bytes);
      for (i = 0; i < bytes; i++)
	{
	  read (port_fd, answer, 1);
	}
    }

  if (answer[0] != byte)
    {
      printf ("error\n");
      return 0;
    }
  printf ("ok\n");
  return 1;
}

int
send_byte (int port_fd, char byte)
{
  int               send;
  char              byte_s[2];

  byte_s[0] = byte;
  printf ("Sending '%s' ...", byte_s);
  send = write (port_fd, &byte, 1);
  if (send == 0)
    {
      printf ("error\n");
      return 0;
    }
  printf ("ok\n");
  return 1;
}
