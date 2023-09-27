# ifndef COMMON_H
# define COMMON_H

# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>

# define ROW 15
# define COL 15
# define WHITE -1
# define BLACK 1
# define BLANK 0

void init_chessboard (int row, int col, int chessboard[][COL]);                  // 初始化棋盘
void init_chessman (int type, char *tableline);                                  // 初始化棋子
void init_menu (void);                                                           // 初始化主菜单
void person_person (void);                                                       // 人人对战
bool check_full (int chessboard[][COL], int row, int col);                       // 判断棋盘是否为满
bool check_win(int chessboard[][COL], int x, int y, int color) ;                 // 判断胜负
void save_chess (int chessboard[][COL], int row, int col);                       // 存盘
void replay_chess (void);                                                        // 复盘


# endif