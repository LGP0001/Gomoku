# include "../include/log.h"

static FILE *log_file = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// 获取日志文件路径
static const char* get_log_path(const char *path, bool server)
{
    printf ("获取日志文件路径开始\n");
    static char log_path[PATH_MAX] = {0};
    if (log_path[0] == '\0') 
    {
        if (path != NULL) 
            strncpy(log_path, path, PATH_MAX - 1);
        else 
        {
            const char *filename = server ? "server.log" : "client.log";
            snprintf(log_path, PATH_MAX, "%s%s%s", getenv("HOME"), "/桌面/text/Gomoku/联网版/log/", filename);
        }
    }
    printf ("获取日志文件路径结束\n");
    return log_path;

}

// 检查并创建目录
static int check_and_create_dir(const char *path) 
{
    printf ("检查并创建目录开始\n");
    char dir_path[PATH_MAX] = {0};
    char *slash_pos = strrchr(path, '/');
    if (slash_pos == NULL) 
    {
        fprintf(stderr, "无效的文件路径: %s\n", path);
        return -1;
    }
    else
        fprintf(stderr, "有效的文件路径: %s\n", path);

    strncpy (dir_path, path, slash_pos - path);  // 提取目录路径
    fprintf(stderr, "提取的文件路径: %s\n", dir_path);
    struct stat st = {0};
    int stat_result = stat(dir_path, &st);
    fprintf(stderr, "stat 结果: %d \n", stat_result);
    if (stat(dir_path, &st) == -1) 
    {
        if (mkdir(dir_path, 0755) == -1) 
        {
            perror("创建目录失败");
            return -1;
        }
    }
    return 0;
    printf ("检查并创建目录结束\n");
}

// 获取当前时间戳
static const char* GetCurrentTimestamp() 
{
    static char buffer[20];
    time_t t;
    struct tm *tmp;

    t = time(NULL);
    tmp = localtime(&t);

    if (tmp == NULL) {
        return "";
    }

    if (strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tmp) == 0) {
        return "";
    }
    return buffer;
}
// 初始化日志模块
void InitLogger(const char *path, bool server) 
{
    printf ("初始化日志开始\n");
    const char *logFilePath = get_log_path( path, server);
    if (check_and_create_dir (logFilePath) != 0) 
    {
        fprintf (stderr, "初始化日志模块失败: 无法创建日志目录\n");
        exit (EXIT_FAILURE);
    }
    pthread_mutex_lock (&log_mutex);
    if (log_file)
        fclose (log_file);
    log_file = fopen (logFilePath, "a");
    pthread_mutex_unlock (&log_mutex);
    if (log_file == NULL) 
    {
        perror ("无法打开日志文件");
        exit (EXIT_FAILURE);
    }
    printf ("初始化日志结束\n");
}

void LogMessage (const char *level, const char *message)
{
    pthread_mutex_lock (&log_mutex);  // 锁定互斥锁以确保线程安全
    if (log_file != NULL) 
    {
        fprintf (log_file, "[%s] [%s] %s\n", level, GetCurrentTimestamp(), message);
        fflush (log_file);  // 刷新输出流，确保日志消息立即写入文件
    } 
    else
        fprintf (stderr, "日志文件未初始化\n");

    fprintf(stdout, "[%s] %s\n", level, message);
    pthread_mutex_unlock (&log_mutex);  // 释放互斥锁
}

void LogError (const char *message)
{
    LogMessage ("Error", message);
}

void LogInfo (const char *message)
{
    LogMessage ("Info", message);
}

void LogWarning (const char *message)
{
    LogMessage ("Warning", message);
}

void LogDebug (const char *message)
{
    LogMessage ("Debug", message);
}