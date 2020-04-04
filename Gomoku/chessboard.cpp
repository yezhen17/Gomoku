#include "chessboard.h"
#include <string.h> 
#include <regex>
using namespace std;

/***************
* [函数] 构造函数
***************/
Chessboard::Chessboard() {
	this->initMove();
	return;
}

/***************
* [函数] 析构函数
***************/
Chessboard::~Chessboard() {
	return;
}

/***************
* [函数] 棋局初始化
***************/
void Chessboard::initMove() {
	this->clearChessboard();	// 清空棋盘
	chessRecord.clear();		// 清空记录
	return;
}

/***************
* [函数] 棋局执行走法
* 返回  S_OK |  F_OUTSIDE | F_NOBLANK
***************/
Status Chessboard::makeMove(const int x, const int y) {
	// 判断走法是否合法
	if (!inChessboard(x, y))
		return Status::F_OUTSIDE;
	if (chessboard[x][y] != Chess::BLANK)
		return Status::F_NOBLANK;
	// 执行走法
	Chess cur = getCurrentChess();	// 更新棋盘
	chessboard[x][y] = cur;

	// 更新辅助数组
	char c = chess2char(cur);
	horizontals[y - 1][x] = c;
	verticals[x - 1][y] = c;
	if (x + y <= GRID_NUM + 1) {
		up_diagonals[x + y - 2][x] = c;
	}
	else {
		up_diagonals[x + y - 2][GRID_NUM + 1 - y] = c;
	}
	if (x - y <= 0) {
		down_diagonals[x - y + GRID_NUM - 1][x] = c;
	}
	else {
		down_diagonals[x - y + GRID_NUM - 1][y] = c;
	}
	
	chessRecord.push_back(Move(x, y));		// 更新记录
	return Status::S_OK;
}


/***************
* [函数] 棋局撤销走法
* 返回  S_OK | F_NOLAST
***************/
Status Chessboard::unMakeMove() {
	// 判断能否撤销
	if (chessRecord.empty())
		return Status::F_NOLAST;
	// 撤销走法
	Move move = chessRecord.back();
	int x = move.x, y = move.y;
	chessboard[x][y] = Chess::BLANK;	 // 更新棋盘

	// 更新辅助数组
	horizontals[y - 1][x] = '0';
	verticals[x - 1][y] = '0';
	if (x + y <= GRID_NUM + 1) {
		up_diagonals[x + y - 2][x] = '0';
	}
	else {
		up_diagonals[x + y - 2][GRID_NUM + 1 - y] = '0';
	}
	if (x - y <= 0) {
		down_diagonals[x - y + GRID_NUM - 1][x] = '0';
	}
	else {
		down_diagonals[x - y + GRID_NUM - 1][y] = '0';
	}

	chessRecord.pop_back();						// 更新记录
	return Status::S_OK;
}

/***************
* [函数] 棋局胜负判断
* 返回  G_BLACK | G_WHITE | G_DRAW | G_CONTINUE
***************/
Status Chessboard::gameOver() {
	// 正则表达式判断是否出现五子连珠
	std::regex black_win("11111");
	std::regex white_win("22222");
	for (int i = 0; i < GRID_NUM; i++) {
		if (std::regex_search(horizontals[i], black_win)) {
			return Status::G_BLACK;
		}
		if (std::regex_search(horizontals[i], white_win)) {
			return Status::G_WHITE;
		}
	}
	for (int i = 0; i < GRID_NUM; i++) {
		if (std::regex_search(verticals[i], black_win)) {
			return Status::G_BLACK;
		}
		if (std::regex_search(verticals[i], white_win)) {
			return Status::G_WHITE;
		}
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		if (std::regex_search(up_diagonals[i], black_win)) {
			return Status::G_BLACK;
		}
		if (std::regex_search(up_diagonals[i], white_win)) {
			return Status::G_WHITE;
		}
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		if (std::regex_search(down_diagonals[i], black_win)) {
			return Status::G_BLACK;
		}
		if (std::regex_search(down_diagonals[i], white_win)) {
			return Status::G_WHITE;
		}
	}
	
	if (getCurrentStep() > GRID_NUM * GRID_NUM) {
		return Status::G_DRAW; // 和局
	}
	return Status::G_CONTINUE;
}

/***************
* [函数] 打印棋盘
***************/
void Chessboard::printChessboard() {
	// TODO
	for (int i = 0; i < 16; i++) {
		if (i == 0) {
			printf("  ");
			for (int j = 1; j < 16; j++)
				printf("%-2d", j);
			printf("\n");
			continue;
		}
		for (int j = 0; j < 16; j++) {
			if (j == 0) {
				printf("%2d", i);
				continue;
			}
			if (chessboard[i][j] == Chess::BLACK)		//此处为黑子
				printf("○");
			else if (chessboard[i][j] == Chess::WHITE)	//此处为白子
				printf("●");
			else {
				// 边界打印
				if (i == 1 && j == 1)
					printf("╔ ");
				else if (i == 1 && j == 15)
					printf("╗");
				else if (i == 15 && j == 1)
					printf("╚ ");
				else if (i == 15 && j == 15)
					printf("╝");
				else if (i == 1 && j != 15 && j != 1)
					printf("┯ ");
				else if (i == 15 && j != 15 && j != 1)
					printf("┷ ");
				else if (j == 1 && i != 15 && i != 1)
					printf("┠ ");
				else if (j == 15 && i != 15 && i != 1)
					printf("┨");
				else
					printf("┼ ");
			}
		}
		printf("\n");
	}
}