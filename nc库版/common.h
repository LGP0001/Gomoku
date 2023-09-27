# ifndef COMMON_H
# define COMMON_H

# include <stdbool.h>
# include <ncurses.h>
# include <locale.h>
# include <stddef.h>

# define ROW 15
# define COL 15
# define BLACK 'x'
# define WHITE 'o'
# define BLANK ' '

bool check_win(char chessboard[][COL], int x, int y, char color) ;                 // 判断胜负

# endif