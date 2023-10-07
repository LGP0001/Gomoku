# ifndef NETWORK_H
# define NETWORK_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdbool.h>
# include <errno.h>
# include <fcntl.h>
# include <ifaddrs.h>
# include <netdb.h>
# include <linux/limits.h>

# define BACKLOG 10
# define ERROR -1 

typedef struct sockaddr_in Addr_in;
typedef struct sockaddl Addr;

int Init_Address (const char *ip, const char *port, bool server);
bool load_config (const char *config_path, char *server_ip, char *server_port) ;

// 创建或更新配置文件
void auto_config(bool server); 
void write_config(const char *ip, unsigned short port, bool server);
unsigned short get_free_port(void);
void get_local_ip(char *ip_str, size_t maxlen);

# endif