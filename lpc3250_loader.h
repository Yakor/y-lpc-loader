int main  (int argc, char *argv[], char *env[] );
int setup_port(int port_fd);
int wait_byte (int port_fd,char byte,int skip);
int send_byte (int port_fd,char byte);
