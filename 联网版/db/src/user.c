# include "../../include/data.h"
# include "../../include/log.h"

sqlite3* db = NULL;
bool is_authenticated = true;

// 处理密码哈希和盐生成
bool hash_password (const char* password, char* hash, char* salt)
{
    LogDebug ("密码哈希和盐生成开始hash_password");
    int workfactor = 4;
    int ret = bcrypt_gensalt (workfactor, salt);
    if (ret != 0) 
    {
        LogError ("生成盐值失败bcrypt_gensalt");
        return false;
    } 
    else 
        LogInfo ("生成盐值成功bcrypt_gensalt");

    ret = bcrypt_hashpw (password, salt, hash);
    if (ret != 0) 
    {
        LogError ("生成哈希密码失败bcrypt_hashpw");
        return false;
    }
    else
        LogInfo ("生成哈希密码成功bcrypt_hashpw");

    LogDebug ("密码哈希和盐生成结束hash_password");
    return true;
}

// 初始化用户表
int setup_user_database()
{
    LogDebug ("初始化用户表开始setup_user_database");
    db = Init_Database("/home/lgp/桌面/text/Gomoku/联网版/db/db/user.db");
    if (db == NULL) 
    {
        char error_msg[4096];
        snprintf (error_msg, sizeof(error_msg), "无法打开数据库. 错误原因: %s", sqlite3_errmsg(db));
        LogError (error_msg);
        return -1;
    }
    else
        LogInfo ("打开数据库成功");

    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS Users ("
        "Username TEXT PRIMARY KEY NOT NULL, "
        "PasswordHash TEXT NOT NULL, "
        "PasswordSalt TEXT NOT NULL, "
        "Role TEXT NOT NULL);";    
    int rc = Execute_SQL(db, create_table_sql, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        char error_msg[4096];
        snprintf (error_msg, sizeof(error_msg), "无法创建用户表. 错误原因: %s", sqlite3_errmsg(db));
        LogError (error_msg);
        Close_Database(db);
        return -1;
    }
    else
        LogInfo ("创建用户表成功");
    
    const char* admin_username = "root";
    const char* admin_hash = "$2a$04$Qk45p8ljDlFQASHu3SADxuUaI9XZfnv76vzEMwqqRBpe6HZqzMJfm";
    char admin_salt[23];

    strncpy(admin_salt, admin_hash + 7, 22);
    admin_salt[22] = '\0';
    LogInfo ("盐值成功");

    char *sql_check_existence = "SELECT EXISTS(SELECT 1 FROM Users WHERE Username = ?);";
    sqlite3_stmt *stmt;
    int sql_result = sqlite3_prepare_v2(db, sql_check_existence, -1, &stmt, NULL);
    if (sql_result != SQLITE_OK) 
    {
        char error_msg[4096];
        snprintf(error_msg, sizeof(error_msg), "准备SQL语句失败. 错误原因: %s", sqlite3_errmsg(db));
        LogError(error_msg);
        sqlite3_finalize(stmt);
        Close_Database(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, admin_username, -1, SQLITE_STATIC);

    int exists = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        exists = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    if (exists) 
    {
        LogInfo("管理员用户已存在，跳过创建管理员用户.");
    } 
    else 
    {
        char insert_admin_sql[512];
        snprintf(insert_admin_sql, sizeof(insert_admin_sql),
             "INSERT INTO Users (Username, PasswordHash, PasswordSalt, Role) VALUES ('%s', '%s', '%s', 'admin');",
             admin_username, admin_hash, admin_salt);
        rc = Execute_SQL(db, insert_admin_sql, NULL, NULL);
        if (rc != SQLITE_OK)
        {
            char error_msg[4096];
            snprintf(error_msg, sizeof(error_msg), "无法创建管理员. 错误原因: %s", sqlite3_errmsg(db));
            LogError(error_msg);
            Close_Database(db);
            return -1;
        }
        else
            LogInfo("创建管理员成功");
    }

    LogDebug ("初始化用户表结束setup_user_database");
    Close_Database(db);
    return 0; 
}

// 用户注册功能
bool register_user(User *user)
{
    LogDebug ("用户注册开始register_user");
    char query[256];
    int ret;
    char salt[BCRYPT_HASHSIZE];
    char hash[BCRYPT_HASHSIZE];
    char insert_sql[512];

    if (!user || !user->username || strlen(user->username) == 0 ||
        !user->password || strlen(user->password) == 0)
        {
            LogError ("无效数据输入");
            return false;
        }
    else
        LogInfo ("数据输入有效");

    sqlite3 *db = Init_Database("/home/lgp/桌面/text/Gomoku/联网版/db/db/user.db");
    if (!db) {
        LogError ("无法初始化数据库");
        return false;
    }
    else
        LogInfo ("初始化数据库有效");

    snprintf (query, sizeof(query), "SELECT Username FROM Users WHERE Username='%s';", user->username);
    sqlite3_stmt *stmt;
    ret = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (ret != SQLITE_OK)
    {
        char error_msg[4096];
        snprintf(error_msg, sizeof(error_msg), "SQL Preparation Error: %s", sqlite3_errmsg(db));
        LogError(error_msg);
        LogError ("准备SQL语句失败");
        Close_Database (db);
        return false;
    }
    else
        LogInfo ("准备SQL语句成功");

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        LogError ("用户名已存在");
        sqlite3_finalize (stmt);
        Close_Database (db);
        return false;
    }
    else
        LogInfo ("准备SQL语句成功");

    sqlite3_finalize (stmt);

    if (!hash_password (user->password, hash, salt))
    {
        LogError ("哈希密码失败");
        Close_Database(db);
        return false;
    }
    else
        LogInfo ("哈希密码成功");

    snprintf(insert_sql, sizeof(insert_sql),
             "INSERT INTO Users (Username, PasswordHash, PasswordSalt, Role) VALUES ('%s', '%s', '%s', 'user');",
             user->username, hash, salt);
    if (Execute_SQL (db, insert_sql, NULL, NULL) != SQLITE_OK) {
        LogError ("无法插入新用户");
        Close_Database (db);
        return false;
    }
    else
        LogInfo ("插入新用户成功");

    LogDebug ("用户注册结束register_user");
    Close_Database (db);
    return true;
}

// 用户登录功能
bool login_user(User *user)
{
    LogDebug("用户登录开始login_user");

    // 检查输入有效性
    if (!user || !user->username || strlen(user->username) == 0 ||
        !user->password || strlen(user->password) == 0)
    {
        LogError("无效数据输入");
        return false;
    }
    else
        LogInfo ("数据输入有效");

    sqlite3 *db = Init_Database("/home/lgp/桌面/text/Gomoku/联网版/db/db/user.db");
    if (!db)
    {
        LogError("无法初始化数据库");
        return false;
    }
    else
        LogInfo ("初始化数据库有效");

    // 准备查询SQL
    char query[256];
    snprintf(query, sizeof(query),
             "SELECT PasswordHash, PasswordSalt, Role FROM Users WHERE Username='%s';",
             user->username);

    sqlite3_stmt *stmt;
    int ret = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (ret != SQLITE_OK)
    {
        LogError("准备SQL语句失败");
        Close_Database(db);
        return false;
    }
    else
        LogInfo ("准备SQL语句成功");

    // 执行查询
    if (sqlite3_step(stmt) != SQLITE_ROW)
    {
        LogError("用户名不存在或查询失败");
        sqlite3_finalize(stmt);
        Close_Database(db);
        return false;
    }

    // 获取数据库中的哈希密码和盐值
    const unsigned char *db_hash = sqlite3_column_text(stmt, 0);
    const unsigned char *db_salt = sqlite3_column_text(stmt, 1);
    const unsigned char *db_role = sqlite3_column_text(stmt, 2);
    
    // 用盐值和用户提供的密码生成哈希
    char hash[BCRYPT_HASHSIZE];
    bcrypt_hashpw(user->password, (const char *)db_salt, hash);

    // 比较哈希值是否匹配
    if (strncmp(hash, (const char *)db_hash, BCRYPT_HASHSIZE) != 0)
    {
        LogError("密码错误");
        sqlite3_finalize(stmt);
        Close_Database(db);
        return false;
    }
    else  
        LogInfo("密码正确");

    if (strcmp((const char *)db_role, "admin") == 0)
    {
        LogInfo("管理员登录成功，欢迎回来");
    }
    else
    {
        LogInfo("用户登录成功，欢迎回来");
    }
    // 清理并返回成功
    sqlite3_finalize(stmt);
    Close_Database(db);
    LogInfo("登录成功");
    LogDebug("用户登录结束login_user");
    return true;
}

int delete_user(const char* username) 
{
    LogDebug("用户删除开始delete_user");
    sqlite3* db;
    int rc = sqlite3_open("/home/lgp/桌面/text/Gomoku/联网版/db/db/user.db", &db);
    if (rc != SQLITE_OK)
    {
        char error_msg[4096];
        snprintf(error_msg, sizeof(error_msg), "无法打开数据库: %s", sqlite3_errmsg(db));
        LogError(error_msg);
        LogError ("打开数据库失败");
        return -1;
    }
    else
        LogInfo ("打开数据库成功");
        
    const char* sql = "DELETE FROM Users WHERE Username=?";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) 
    {
        char error_msg[4096];
        snprintf(error_msg, sizeof(error_msg), "准备SQL失败: %s\n", sqlite3_errmsg(db));
        LogError(error_msg);
        LogError ("准备SQL失败");
        return -1;
    }
    else
        LogInfo ("准备SQL成功");

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) 
    {
        int deleted_rows = sqlite3_changes(db);
        if (deleted_rows == 0) 
        {
            char error_msg[4096];
            snprintf(error_msg, sizeof(error_msg), "没有找到用户: %s\n", username);
            LogError(error_msg);
            LogError ("准备SQL失败");
        } 
        else 
        {
            printf("成功删除用户: %s\n", username);
        }
    } 
    else 
    {
        fprintf(stderr, "删除失败: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    LogDebug("用户删除结束delete_user");
    return rc == SQLITE_DONE ? 0 : -1;
}





