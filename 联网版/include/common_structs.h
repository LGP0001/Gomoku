# ifndef COMMON_STRUCTS_H
# define COMMON_STRUCTS_H

# define ROW 15
# define COL 15

// 自定义协议类型
typedef enum 
{
	// 用户请求
    REGISTER = 1,               // 注册请求
    LOGIN,                      // 登录请求
    DELETE,                     // 删除请求                        

    // 五子棋请求
    MOVE,                       // 下棋请求
    GAME_STATUS_UPDATE,         // 游戏状态更新请求
    CHECK_WINNER,               // 检查胜利者请求

    // 新增的协议类型
    ACK,                        // 确认消息
    REQUEST_GAME_STATUS,
    EXIT                        // 退出信息
}Type;

typedef enum
{
    WAITING_FOR_OPPONENT,
    GAME_IN_PROGRESS,
    GAME_OVER
} GameState;

// 请求结构体
typedef struct 
{
    Type type;
    char username[50];
    char password[50];
    bool is_admin;  // 标记是否是管理员
    int x_move;        // 棋子的x坐标
    int y_move;        // 棋子的y坐标
    char player_color; // 棋子颜色，例如 'x' or 'o'
} ChessRequest;

// 响应结构体
typedef struct 
{
    Type type;
    GameState game_state;
    char message[500];  // 消息，例如 "BLACK Wins!"
    char board[ROW][COL]; // 更新后的棋盘
    char current_turn;  // 当前该谁下
    bool login_status;     // 登录成功或失败
    bool register_status;  // 注册成功或失败
    bool delete_status; // 删除成功或失败
    bool is_admin;  // 标记是否是管理员
} ChessResponse;

typedef struct
{
    int type;
    char name[32];
    char data[256];
    char board[ROW][COL]; // 如果消息是棋盘更新，可以使用这个字段
    int x_move;        // 棋子的x坐标
    int y_move;        // 棋子的y坐标
    char player_color; // 棋子颜色
} MSG;

#endif // COMMON_STRUCTS_H