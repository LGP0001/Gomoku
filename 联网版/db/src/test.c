# include "../../include/data.h"
# include "../../include/log.h"  // 请确保此头文件包含您之前实现函数的声明
# include <assert.h>
# include <stdio.h>
# include <stdlib.h>  // 用于 malloc 和 free
# include <string.h>

void test_setup_user_database()
{
    int result = setup_user_database();
    assert(result == 0);
    printf("测试 setup_user_database 通过!\n");
}

void test_register_user()
{
    User *user = malloc(sizeof(User));  // 分配内存
    if (user == NULL) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    
    delete_user("test");
    strcpy(user->username, "test");  // 使用 strcpy 复制字符串
    strcpy(user->password, "password123");
    
    bool result = register_user(user);  // 传递 user 而不是 &user
    assert(result);
    printf("测试 register_user 通过!\n");
    
    free(user);  // 释放内存
}

void test_login_user()
{
    User *user = malloc(sizeof(User));  // 分配内存
    if (user == NULL) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    
    strcpy(user->username, "test");  // 使用 strcpy 复制字符串
    strcpy(user->password, "password123");
    
    bool result = login_user(user);  // 传递 user 而不是 &user
    assert(result);
    printf("测试 login_user 通过!\n");
    
    free(user);  // 释放内存
}

int main()
{
    InitLogger(NULL, true);
    test_setup_user_database();
    test_register_user();
    test_login_user();
    
    printf("所有测试通过!\n");
    
    return 0;
}