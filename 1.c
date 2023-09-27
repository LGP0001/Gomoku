#include <stdio.h>
#include <string.h>
#include <stdint.h>
char *board[] = {
	"╔═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╗",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢",
	"╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝"
};

struct wuziqi {
	int step;
	uint16_t pos_state[15];
	uint16_t pos_piece[15];
}pie;

char *PosStr(int x, int y) {
	static char str[7] = {};
	strncpy(str, &board[x][y*6], 6);
	return str;
}

void show() {
	int i, j;
	for(i = 0; i < 15; i++) {
		for(j = 0; j < 15; j++) {
			if(pie.pos_state[i] & 0x1 << j) {
				printf("%s", (pie.pos_piece[i] & 0x1 << j)?"① ": "❶ ");
			} else {
				printf("%s", PosStr(i, j));
			}
		}
		printf("\n");
	}
}

void play() {
	int x, y;
	scanf("%d%d", &x, &y);
	if(!(pie.pos_state[x] & (0x1 << y))) {
		pie.pos_state[x] |= 0x1 << y;
		pie.pos_piece[x] |= (pie.step % 2) << y;
	}
	pie.step++;
}

int main(int argc, char *argv[])
{
	while(1) {
		show();
		play();
	}
	return 0;
}