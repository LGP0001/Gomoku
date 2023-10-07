# include "../../include/network.h"
# include "../../include/log.h"

// 函数：获取本地 IP 地址
void get_local_ip (char *ip_str, size_t maxlen) 
{
    struct ifaddrs *ifaddr, *ifa;
    int family, s;

    LogDebug ("获取本地 IP 地址开始get_local_ip");
    // 获取本地所有网络接口的地址
    if (getifaddrs(&ifaddr) == -1) 
    {
        LogError ("获取本地所有网络接口地址失败");
        exit (EXIT_FAILURE);
    }
    else
        LogInfo ("获取本地所有网络接口地址成功");

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;
        // 如果是 IPv4 地址
        if (family == AF_INET)
        {
            // 将网络地址转换为字符串格式
            s = getnameinfo (ifa->ifa_addr, sizeof (struct sockaddr_in), ip_str, maxlen, NULL, 0, NI_NUMERICHOST);
            if (s != 0)
                {
                    char error_msg[512];
                    snprintf(error_msg, sizeof(error_msg), "网络地址转换为字符串格式失败 失败原因: %s", gai_strerror(s));
                    LogError ("网络地址转换为字符串格式失败");
                    exit (EXIT_FAILURE);
                }
                else
                    LogInfo ("网络地址转换为字符串格式成功");

                // 跳过本地地址 127.0.0.1
                if (strcmp (ip_str, "127.0.0.1") != 0)
                    break;
        }
    }
    // 释放网络接口地址列表
    freeifaddrs(ifaddr);
    LogDebug ("获取本地 IP 地址结束get_local_ip");
}

// 函数：获取一个可用的端口号
unsigned short get_free_port () 
{
    int sockfd;
    struct sockaddr_in addr;
    socklen_t len = sizeof (addr);

    LogDebug ("获取一个可用的端口号开始get_free_port");
    // 创建 socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        LogError ("创建socket失败");
        exit(EXIT_FAILURE);
    }
    else 
        LogInfo ("创建socket成功");

    memset (&addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons (0);

    // 绑定 socket 到任意端口
    if (bind(sockfd, (struct sockaddr *) &addr, sizeof (addr)) < 0) 
    {
        char error_msg[512];
        snprintf (error_msg, sizeof(error_msg), "无法绑定到IP 和端口. 错误原因: %s", strerror(errno));
        LogError (error_msg);
        close (sockfd);
        exit (EXIT_FAILURE);
    }
    else
        LogInfo ("绑定地址成功");

    // 查询系统分配的端口号
    if (getsockname(sockfd, (struct sockaddr *)&addr, &len) == -1) 
    {
        LogError ("获取端口号失败");
        close (sockfd);
        exit (EXIT_FAILURE);
    }
    else
        LogInfo ("获取端口号成功");

    // 关闭 socket
    close(sockfd);
    return ntohs(addr.sin_port);
    LogDebug ("获取一个可用的端口号结束get_free_port");
}

// 函数：将 IP 和端口号写入 config.txt 文件
void write_config (const char *ip, unsigned short port, bool server) 
{
    LogDebug ("将 IP 和端口号写入文件开始write_config");
    static char config_path[PATH_MAX] = {0};
    const char *filename = server ? "config_server.txt" : "config_client.txt";
    if (server == true)
    {
        snprintf(config_path, PATH_MAX, "%s%s%s", getenv("HOME"), "/桌面/text/Gomoku/联网版/server/bin/", filename);
    }
    else 
    {
        snprintf(config_path, PATH_MAX, "%s%s%s", getenv("HOME"), "/桌面/text/Gomoku/联网版/client/bin/", filename);
    }
    printf ("网络配置文件路径: %s\n", config_path);

    FILE *file = fopen (config_path, "w");
    if (file == NULL) 
    {
        char error_msg[4096];
        snprintf (error_msg, sizeof(error_msg), "无法打开 %s. 错误原因: %s", config_path, strerror(errno));
        LogError (error_msg);
        exit(EXIT_FAILURE);
    }
    else 
    {
        fprintf(file, "IP: %s\nPort: %hu\n", ip, port);
        LogInfo ("将 IP 和端口号写入文件成功");
    }
    fclose(file);
    LogDebug ("将 IP 和端口号写入文件结束write_config");
}

void auto_config (bool server) 
{
    LogDebug ("创建或更新配置文件开始auto_config");
    char ip_str[INET_ADDRSTRLEN];
    get_local_ip (ip_str, sizeof(ip_str));
    unsigned short port = get_free_port ();
    write_config(ip_str, port, server);

    LogInfo("自动配置成功");
    LogDebug ("创建或更新配置文件结束auto_config");
}

/*初始化网络*/
int Init_Address(const char *ip, const char *port, bool server)
{
    LogDebug ("初始化网络开始");
    int sockfd;
    Addr_in addr_in;
    socklen_t addrlen = sizeof(addr_in);
    // 创建套接字
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        LogError("创建socket失败");
        return ERROR;
    }
    else
    {
        LogInfo("成功创建socket");
    }
    
    bzero(&addr_in, addrlen);
    addr_in.sin_family = AF_INET;
    int parsed_port = atoi(port);// 检查端口
    if(parsed_port <= 0 || parsed_port > 65535)
    {
        LogError("错误端口");
        return ERROR;
    }
    addr_in.sin_port = htons(parsed_port);
    if(inet_aton(ip, &addr_in.sin_addr) == 0)// 地址转换失败
    {
        LogError("错误IP");
        return ERROR;
    }
    else
    {
        LogInfo("成功识别IP");
    }
    if (server) // 判断是否为服务端
    {
        /*地址快速重用*/
        int opt = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));

        /*绑定地址*/
        if(bind(sockfd, (struct sockaddr *)&addr_in, addrlen) < 0)
        {
            char error_msg[512];
            snprintf(error_msg, sizeof(error_msg), "无法绑定到IP: %s 和端口: %s. 错误原因: %s", ip, port, strerror(errno));
            LogError(error_msg);
            return ERROR;
        }
        else
        {
            LogInfo("成功绑定地址");
        }
        /*设定为监听模式*/
        if(listen(sockfd, BACKLOG) < 0)
        {
            char error_msg[512];
            snprintf(error_msg, sizeof(error_msg), "无法监听到IP: %s 和端口: %s. 错误原因: %s", ip, port, strerror(errno));
            LogError(error_msg);
            return ERROR;
        }
        else
        {
            LogInfo("成功监听地址");
        }
    }
    else /*如果是客户端就发起连接请求*/
    {
        if(connect(sockfd, (struct sockaddr *)&addr_in, addrlen) < 0)
        {
            char error_msg[512];
            snprintf(error_msg, sizeof(error_msg), "无法链接到IP: %s 和端口: %s. 错误原因: %s", ip, port, strerror(errno));
            LogError(error_msg);            
            return ERROR;
        }
        else
        {
            LogInfo("成功链接地址");
        }
    }

    return sockfd;
    LogDebug ("初始化网络结束");
}

bool load_config(const char *config_path, char *server_ip, char *server_port) 
{
    LogDebug ("加载网络配置文件开始load_config");
    FILE *file = fopen(config_path, "r");
    if (file == NULL) 
        return false;

    char line[256];
    while (fgets(line, sizeof(line), file)) 
    {
        if (strncmp(line, "IP:", 3) == 0) 
        {
            strncpy(server_ip, line + 4, 15);
            server_ip[15] = '\0'; 
            // remove newline if present
            server_ip[strcspn(server_ip, "\n")] = 0;
        } 
        else if (strncmp(line, "Port:", 5) == 0) 
        {
            strncpy(server_port, line + 6, 6);
            server_port[strlen(server_port) - 1] = '\0';
            // remove newline if present
            server_port[strcspn(server_port, "\n")] = 0;

        }
    }
    LogDebug ("加载网络配置文件结束load_config");
    fclose(file);
    return true;
    
}