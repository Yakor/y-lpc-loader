#define HELP_STRING                                             \
  "Usage:\n"                                                    \
  "-h, --help               help\n"                             \
  "-c, --config             file use alternate config file\n"   \
  "-p, --port               port\n"                             \
  "-f, --first              primary execurable file\n"          \
  "-s, --second             secondary execurable file\n"        \
  "-i, --primary-address    primary file load address\n"        \
  "-d, --secondary-address  secondary file load address\n"
int               main (int argc, char *argv[], char *env[]);
int               setup_port (int port_fd);
int               wait_byte (int port_fd, char byte, int skip, int prnt_char);
int               send_byte (int port_fd, char byte);
int               send_file_to_port (int port_fd, char *file_name, int addr, char confirm);
int               send_4_bytes_reverse (int port_fd, int num);
int               create_str (char **dest, const char *source);
