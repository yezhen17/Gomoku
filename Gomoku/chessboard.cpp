#include "chessboard.h"
#include <string.h> 
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
	chessboard[x][y] = getCurrentChess();	// 更新棋盘
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
	chessboard[move.x][move.y] = Chess::BLANK;	// 更新棋盘
	chessRecord.pop_back();						// 更新记录
	return Status::S_OK;
}

/***************
* [函数] 棋局胜负判断
* 返回  G_BLACK | G_WHITE | G_DRAW | G_CONTINUE
***************/
Status Chessboard::gameOver() {
	// TODO
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