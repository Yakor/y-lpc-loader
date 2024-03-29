/**
 * @file   y-lpc-loader.h
 * @author Sergey Yakovlev <yakor.spb@gmail.com>
 * @copyright (c) 2012, Sergey Yakovlev this code is released uder BSD license. Can be found in LICENSE.
 *
 * @brief defines and declared functions needed for y-lpc-loader.c
 *
 *
 */

#ifndef Y_LPC_LOADER_H
#define Y_LPC_LOADER_H

#define HELP_STRING                                                     \
  "Usage:\n"                                                            \
  "-h, --help               help\n"                                     \
  "-c, --config             file use alternate config file\n"           \
  "-p, --port               COM port\n"                                 \
  "-f, --first              primary execurable file\n"                  \
  "-s, --second             secondary execurable file\n"                \
  "-i, --primary-address    primary file load address\n"                \
  "-d, --secondary-address  secondary file load address\n"              \
  "-a, --print-all-char     output all comunicates computer and board\n" \
  "-w, --wait               Wait, dont exit after last exec\n"

int               main (int argc, char *argv[], char *env[]);
int               setup_port (int port_fd);
int               wait_byte (int port_fd, char byte, int skip, int prnt_char, int pure_output);
int               send_byte (int port_fd, char byte, int prnt_char);
int               send_file_to_port (int port_fd, char *file_name, int addr, char confirm,
                                     int prnt_char);
int               send_4_bytes_reverse (int port_fd, int num);
int               create_str (char **dest, const char *source);
char              port_to_stdout (int port_fd);

#endif /* Y_LPC_LOADER_H */
