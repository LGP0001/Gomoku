# include "../../include/client.h"

void send_request_to_server(int sockfd, ChessResponse *gameResponse);
int main(int argc, char const *argv[])
{
    char client_ip[16];
    char client_port[8];
    bool client = false;
    static char config_path[PATH_MAX] = {0};
    const char *filename = client ? "config_server.txt" : "config_client.txt";

    InitLogger (NULL, false);
    snprintf(config_path, PATH_MAX, "%s%s%s", getenv("HOME"), "/桌面/text/Gomoku/联网版/client/bin/", filename);
    bool config_loaded = load_config(config_path, client_ip, client_port);
    if (config_loaded) 
    {
        // 如果配置文件加载成功，打印相关信息
        printf("加载成功: IP = %s, Port = %s\n", client_ip, client_port);
    } 
    else 
    {
        // 如果配置文件加载失败（可能是因为文件不存在），则执行自动配置
        printf("网络配置文件不存在. 正在创建中...\n");
<<<<<<< HEAD
        auto_config(true);  // 这个函数应该包含get_local_ip, get_free_port和write_config的调用
=======
        auto_config(client);  // 这个函数应该包含get_local_ip, get_free_port和write_config的调用
>>>>>>> 14d2211 (修改文件目录)
        config_loaded = load_config(config_path, client_ip, client_port);  // 重新加载配置文件
        
        if (config_loaded) 
        {
            printf("创建成功: IP = %s, Port = %s\n", client_ip, client_port);
        } 
        else 
        {
            LogError ("自动创建文件失败.\n");
            return -1;  // 或其他错误处理
        }
        LogDebug ("自动创建网络配置文件成功.\n");
    }

    int client_sockfd = Init_Address(client_ip, client_port, false);

    printf("[CLIENT] socket套接字为: %d\n", client_sockfd);

<<<<<<< HEAD
    setlocale(LC_ALL, "");
=======
    setlocale(LC_ALL, "zh_CN.UTF-8");
>>>>>>> 14d2211 (修改文件目录)
    initscr ();                                                                                     // 初始化ncurses
    noecho ();                                                                                      // 禁止键盘回显   
    cbreak ();                                                                                      // 禁止行缓冲
    keypad (stdscr, TRUE);                                                                          // 启动键盘映射
    curs_set (1);                                                                                   // 设置光标可见

<<<<<<< HEAD
    char chessboard[ROW][COL] = { 0 };                                                              // 初始化棋盘
=======
    wchar_t chessboard[ROW][COL] = { 0 };                                                              // 初始化棋盘
>>>>>>> 14d2211 (修改文件目录)
    for (int i = 0; i < ROW; ++i) 
    {
        for (int j = 0; j < COL; ++j) 
        {
            chessboard[i][j] = BLANK;
        }
    }
    int cur_x = 0, cur_y = 0;                                                                       // 光标位置
    char current_player = BLACK;                                                                    // 当前玩家
    bool game_over = false;

<<<<<<< HEAD
    ChessRequest request;
    ChessResponse response;

    while (1)
    {
        clear ();
        mvwprintw (stdscr, 5, 5, "1. 注册");
        mvwprintw (stdscr, 6, 5, "2. 登录");
        mvwprintw (stdscr, 7, 5, "3. 退出");
=======
    while (1)
    {
        clear ();
        mvwaddwstr (stdscr, 5, 5, L"1. 注册");
        mvwaddwstr (stdscr, 6, 5, L"2. 登录");
        mvwaddwstr (stdscr, 7, 5, L"3. 退出");
        mvwaddwstr (stdscr, 8, 5, L"请输入你的选择： ");
>>>>>>> 14d2211 (修改文件目录)
        wrefresh(stdscr);

        int choice = wgetch(stdscr);

        switch (choice)
        {
            case '1':
            {            
                char username[50], password[50];
                mvwprintw (stdscr, 9, 5, "请输入用户名：");
                getstr (username);
                mvwprintw (stdscr, 10, 5, "请输入密码：");
                getstr(password);

                if (perform_register (client_sockfd, username, password)) 
                {
                    mvwprintw (stdscr, 12, 5, "注册成功！");
                    wgetch(stdscr);
                }
                else
                {
                    mvwprintw (stdscr, 12, 5, "注册失败，请重试！");
                    wgetch(stdscr);
                }
                break;
            }
            case '2':
            {
                char username[50], password[50];
<<<<<<< HEAD
=======
                bool is_admin;

>>>>>>> 14d2211 (修改文件目录)
                mvwprintw (stdscr, 9, 5, "请输入用户名：");
                getstr (username);
                mvwprintw (stdscr, 10, 5, "请输入密码：");
                getstr(password);

<<<<<<< HEAD
                bool login_successful = perform_login(client_sockfd, username, password);
                if (login_successful) 
                {
                    bool is_admin = check_admin_status(client_sockfd, username);  // 需要一个函数来从服务器查询这个用户是否是管理员
        
=======
                bool login_successful = perform_login(client_sockfd, username, password, &is_admin);
                if (login_successful) 
                {        
>>>>>>> 14d2211 (修改文件目录)
                    if (is_admin) 
                    {
                        // 管理员操作，例如删除用户等...
                        mvwprintw(stdscr, 12, 5, "管理员登录成功！");

                        char ch = true;
                        while (ch)
                        {
                            clear ();
                            mvwprintw (stdscr, 5, 5, "1. 删除用户");
                            mvwprintw (stdscr, 6, 5, "2. 返回");
                            wrefresh(stdscr);

                            int admin_choice = wgetch(stdscr); 

                            switch(admin_choice)
                            {
                                case '1':
                                {
                                    mvwprintw (stdscr, 9, 5, "请输入用户名：");
                                    getstr (username);
                                    mvwprintw (stdscr, 10, 5, "请输入密码：");
                                    getstr(password);

                                    if (perform_delete (client_sockfd, username, password)) 
                                    {
                                        mvwprintw (stdscr, 12, 5, "用户已删除！");
                                        wgetch(stdscr);
                                    }
                                    else
                                    {
                                        mvwprintw (stdscr, 12, 5, "用户删除失败，请重试！");
                                        wgetch(stdscr);
                                    }
                                    break;
                                }
                                case '2':
                                {
                                    clear();
                                    mvwprintw(stdscr, 8, 5, "返回中...");
                                    wgetch(stdscr); // 暂停，等待用户按键继续
<<<<<<< HEAD
                                    return;
=======
                                    ch = false;
                                    break;
>>>>>>> 14d2211 (修改文件目录)
                                }
                                default:
                                {
                                    mvwprintw(stdscr, 8, 5, "无效的选项，请重新选择！");
                                    wgetch(stdscr); // 暂停，等待用户按键继续
                                    break;
                                }
                            }
                        }
                    } 
                    else 
                    {    // 进入你的五子棋游戏逻辑...
                        ChessResponse gameResponse;
                        // 这里发送一个请求到服务器询问游戏状态
                        send_request_to_server(client_sockfd, &gameResponse);  // 这个函数需要你实现

                        while (gameResponse.game_state == WAITING_FOR_OPPONENT)
                        {
                            mvwprintw(stdscr, ROW + 2, 0, "等待另一个玩家加入...");
                            sleep(2);  // 等待两秒再次检查
                            send_request_to_server(client_sockfd, &gameResponse);  // 再次查询
                        }

                        if (gameResponse.game_state == GAME_IN_PROGRESS)
                        {
                            // 进入你的五子棋游戏逻辑...
                            while (!game_over)
                            {
                                clear();

                                // 绘制棋盘
<<<<<<< HEAD
                                for (int i = 0; i < ROW; ++i)
                                {
                                    for (int j = 0; j < COL; ++j)
                                    {
                                        mvaddch(i, j * 2, chessboard[i][j]);
                                    }
                                }
=======
                                init_chessboard(ROW, COL, chessboard); 
>>>>>>> 14d2211 (修改文件目录)

                                mvwprintw(stdscr, ROW, 0, "玩家：%lc", current_player);

                                move(cur_x, cur_y * 2);
                                int ch = getch();

                                switch (ch)
                                {
                                    case KEY_UP:
                                        if (cur_x > 0)
                                            cur_x--;
                                        break;
                                    case KEY_DOWN:
                                        if (cur_x < ROW - 1)
                                            cur_x++;
                                        break;
                                    case KEY_LEFT:
                                        if (cur_y > 0) 
                                            cur_y--;
                                        break;
                                    case KEY_RIGHT:
                                        if (cur_y < COL - 1) 
                                            cur_y++;
                                        break;
                                    case ' ':
                                        if (chessboard[cur_x][cur_y] == BLANK)
                                        {
                                            ChessRequest req;
                                            memset (&req, 0, sizeof(ChessRequest));

                                            req.type = MOVE;
                                            req.x_move = cur_x;
                                            req.y_move = cur_y;
                                            req.player_color = current_player;
                                            send(client_sockfd, (char*)&req, sizeof(ChessRequest), 0);

                                            ChessResponse res;
                                            memset (&res, 0, sizeof(ChessResponse));
                                            recv(client_sockfd, (char*)&res, sizeof(ChessResponse), 0);

                                            // 更新本地棋盘
                                            memcpy(chessboard, res.board, sizeof(res.board));

                                            if (strcmp(res.message, "") != 0)  // 如果服务器发送了胜利消息
                                            {
                                                game_over = true;
                                                mvwprintw(stdscr, ROW + 1, 0, "%s", res.message);
                                            }
                                            else
                                            {
                                                current_player = res.current_turn;
                                            }
                                        }
                                        break;
                                }
                            }
                        }
                    }
                    getch();
                }
                else
                {
                    mvwprintw(stdscr, 12, 5, "登录失败，请重试！");
                    getch();
                }
<<<<<<< HEAD
            }
        }

        


    }
    endwin();
    return 0;
=======
                break;
            }
            case '3':
            {
                mvwprintw (stdscr, 9, 5, "期待与你的下次相遇！");
                wgetch(stdscr);
                endwin();
                return 0;
            }
            default:
            {
                mvwprintw (stdscr, 9, 5, "输入无效");
                wgetch(stdscr);
                break;
            }
        }
    }

>>>>>>> 14d2211 (修改文件目录)
}

bool perform_register(int sockfd, const char *username, const char *password) 
{
    ChessRequest req;
    ChessResponse res;

    memset (&req, 0, sizeof(ChessRequest));
    memset (&res, 0, sizeof(ChessResponse));

    req.type = REGISTER;
    strncpy(req.username, username, sizeof(req.username) - 1);
    strncpy(req.password, password, sizeof(req.password) - 1);

    send(sockfd, (char*)&req, sizeof(ChessRequest), 0);  // 发送注册请求
    recv(sockfd, (char*)&res, sizeof(ChessResponse), 0);  // 接收服务器响应

    return res.register_status;
}

<<<<<<< HEAD
bool perform_login(int sockfd, const char *username, const char *password) 
=======
bool perform_login(int sockfd, const char *username, const char *password, bool *is_admin) 
>>>>>>> 14d2211 (修改文件目录)
{
    ChessRequest req;
    ChessResponse res;

    memset (&req, 0, sizeof(ChessRequest));
    memset (&res, 0, sizeof(ChessResponse));

    req.type = LOGIN;
    strncpy(req.username, username, sizeof(req.username) - 1);
    strncpy(req.password, password, sizeof(req.password) - 1);

    send(sockfd, (char*)&req, sizeof(ChessRequest), 0);  // 发送登录请求
    recv(sockfd, (char*)&res, sizeof(ChessResponse), 0);  // 接收服务器响应

<<<<<<< HEAD
=======
    *is_admin = res.is_admin;

>>>>>>> 14d2211 (修改文件目录)
    return res.login_status;
}

bool perform_delete(int sockfd, const char *username, const char *password) 
{
    ChessRequest req;
    ChessResponse res;

    memset (&req, 0, sizeof(ChessRequest));
    memset (&res, 0, sizeof(ChessResponse));

    req.type = DELETE;
    strncpy(req.username, username, sizeof(req.username) - 1);
    strncpy(req.password, password, sizeof(req.password) - 1);

    send(sockfd, (char*)&req, sizeof(ChessRequest), 0);  // 发送登录请求
    recv(sockfd, (char*)&res, sizeof(ChessResponse), 0);  // 接收服务器响应

    if (res.delete_status) 
        return true;
    else
        return false;
}

void send_request_to_server(int sockfd, ChessResponse *gameResponse) 
{
    ChessRequest gameRequest;
    memset(&gameRequest, 0, sizeof(gameRequest));
    gameRequest.type = REQUEST_GAME_STATUS;  // 假设你有这样一个枚举值来请求游戏状态

    // 发送请求到服务器
    if (send(sockfd, &gameRequest, sizeof(gameRequest), 0) == -1) {
        mvwprintw(stdscr, ROW + 3, 0, "向服务器发送请求错误!");
        return;
    }

    // 等待并接收服务器的响应
    memset(gameResponse, 0, sizeof(ChessResponse));
    int bytes_received = recv(sockfd, gameResponse, sizeof(ChessResponse), 0);
    if (bytes_received == -1) {
        mvwprintw(stdscr, ROW + 3, 0, "从服务器接收响应错误!");
    } else if (bytes_received == 0) {
        mvwprintw(stdscr, ROW + 3, 0, "服务器关闭连接!");
    }
}
<<<<<<< HEAD
=======

void init_chessman(int type, wchar_t *tableline) 
{
    if (type == WHITE)
        printw("O");
    else if (type == BLACK)
        printw("x");
    else
        printw("%ls", tableline);
}

void init_chessboard(int row, int col, wchar_t chessboard[][COL]) 
{
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            move(i, j*2);

            if (i == 0) {
                if (j == 0)
                    init_chessman(chessboard[i][j], L"╔");
                else if (j == 14)
                    init_chessman(chessboard[i][j], L"╗");
                else
                    init_chessman(chessboard[i][j], L"╤");
            }
            else if (i == 14) {
                if (j == 0)
                    init_chessman(chessboard[i][j], L"╚");
                else if (j == 14)
                    init_chessman(chessboard[i][j], L"╝");
                else
                    init_chessman(chessboard[i][j], L"╧");
            }
            else {
                if (j == 0)
                    init_chessman(chessboard[i][j], L"╟");
                else if (j == 14)
                    init_chessman(chessboard[i][j], L"╢");
                else
                    init_chessman(chessboard[i][j], L"┼");
            }
        }
    }
}
>>>>>>> 14d2211 (修改文件目录)
