# include "common.h"

bool check_direction (char chessboard[][COL], int x, int y, int dx, int dy, char color)
{   
    int count = 0;
    for (int i = -4; i <= 4; i++)
    {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx >= 0 && nx < ROW && ny >= 0 && ny < COL && chessboard[nx][ny] == color)
            count++;
        else
            count = 0;  // reset count if a piece from the other player or an empty spot is found

        if (count >= 5)
            return true;
    }
    return false;
}
                      
bool check_win(char chessboard[][COL], int x, int y, char color) // 判断胜负
{
    return (
        check_direction(chessboard, x, y, 1, 0, color) ||
        check_direction(chessboard, x, y, 0, 1, color) ||
        check_direction(chessboard, x, y, 1, 1, color) ||
        check_direction(chessboard, x, y, 1, -1, color)
    );
}