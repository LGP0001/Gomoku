# include "../../include/network.h"
# include "../../include/log.h"
# include <stdio.h>
# include <stdbool.h>

int main() 
{
    char server_ip[16];
    char server_port[8];
    bool server = true;
    InitLogger(NULL, true);
        
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
    // 先尝试加载配置文件
    bool config_loaded = load_config(config_path, server_ip, server_port);
    if (config_loaded) {
        // 如果配置文件加载成功，打印相关信息
        printf("Config loaded: IP = %s, Port = %s\n", server_ip, server_port);
    } else {
        // 如果配置文件加载失败（可能是因为文件不存在），则执行自动配置
        printf("网络配置文件不存在. 正在创建中...\n");
        auto_config(true);  // 这个函数应该包含get_local_ip, get_free_port和write_config的调用
        config_loaded = load_config(config_path, server_ip, server_port);  // 重新加载配置文件
        
        if (config_loaded) 
        {
            printf("创建成功: IP = %s, Port = %s\n", server_ip, server_port);
        } 
        else 
        {
            printf("自动创建文件失败.\n");
            return -1;  // 或其他错误处理
        }
        printf("自动创建网络配置文件成功.\n");
    }

    return 0;
}
