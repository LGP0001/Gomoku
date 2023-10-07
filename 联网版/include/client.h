# ifndef COMMON_H
# define COMMON_H

# include <ncurses.h>
# include <locale.h>
# include <stddef.h>
# include <stdbool.h>

# include "network.h"
# include "common_structs.h"
# include "log.h"

# define ROW 15
# define COL 15
# define BLACK 'x'
# define WHITE 'o'
# define BLANK ' '

bool check_win(char chessboard[][COL], int x, int y, char color) ;                 // 判断胜负
bool perform_register(int sockfd, const char *username, const char *password) ;
bool perform_login(int sockfd, const char *username, const char *password) ;
bool perform_delete(int sockfd, const char *username, const char *password) ;




# endif