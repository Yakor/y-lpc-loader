int               main (int argc, char *argv[], char *env[]);
int               setup_port (int port_fd);
int               wait_byte (int port_fd, char byte, int skip);
int               send_byte (int port_fd, char byte);
int               send_file_to_port (int port_fd, char *file_name);
int               send_4_bytes_reverse (int port_fd, int num);
