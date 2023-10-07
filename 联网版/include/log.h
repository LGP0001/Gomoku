# ifndef LOG_H
# define LOG_H

# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <linux/limits.h>
# include <time.h>

void InitLogger(const char *path, bool server); 
void LogError(const char *message);
void LogInfo(const char *message);
void LogWarning(const char *message);
void LogDebug(const char *message);

# endif // LOG_H
