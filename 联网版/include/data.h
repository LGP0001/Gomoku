# ifndef DATABASE_H
# define DATABASE_H

# include <sqlite3.h>
# include <bcrypt.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>

// 用户角色常量
#define USER_ROLE_ADMIN "admin"
#define USER_ROLE_USER  "user"

typedef struct {
    char username[50];
    char password[50];
    char role[10]; // "admin" 或 "user"
} User;

// 初始化数据库连接
sqlite3* Init_Database(const char* path);
// 执行SQL命令
int Execute_SQL(sqlite3* db, const char *sql, int (*callback)(void*, int, char**, char**), void* data);
// 关闭数据库
void Close_Database(sqlite3 *db);
// 处理密码哈希和盐生成
bool hash_password (const char* password, char* hash, char* salt);
// 初始化用户表
int setup_user_database();
// 用户注册功能
bool register_user(User *user);
// 用户登录功能
bool login_user(User *user);
// 删除用户功能
int delete_user(const char* username); 

# endif