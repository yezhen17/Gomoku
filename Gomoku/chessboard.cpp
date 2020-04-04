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
	// 打印横坐标
	printf("   ");
	for (int i = 1; i <= 15; i++)
		printf("%2X", i);
	printf("\n");
	for (int i = 1; i <= 15; i++) {
		printf(" %2X", i);	// 打印纵坐标
		for (int j = 1; j <= 15; j++) {
			if (chessboard[i][j] == Chess::BLACK)
				printf("○");		// 打印黑子
			else if (chessboard[i][j] == Chess::WHITE)
				printf("●");		// 打印白子
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
	return;
}

/***************
* [函数] 打印棋盘记录
* 参数  步数索引（负数则倒数）
* 返回  true - 打印成功, false - 打印失败
***************/
bool Chessboard::printChessRecord(int step) {
	if (step <= 0)
		step += (chessRecord.size() + 1);
	if (step <= 0 || step > chessRecord.size())
		return false;
	char label[20];
	sprintf_s(label, "【第%d步】", step);
	if (step & 1)
		printf("%-12s ○ → (%X, %X)\n", label, chessRecord[step - 1].x, chessRecord[step - 1].y);
	else
		printf("%-12s ● → (%X, %X)\n", label, chessRecord[step - 1].x, chessRecord[step - 1].y);
	return true;
}