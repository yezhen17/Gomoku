#pragma once
#ifndef _CHESSBOARD_H_
#define _CHESSBOARD_H_
#include <iostream>
#include <string.h>
#include <vector>
#include <regex>

/***************
* [常量] 定义
***************/
const int GRID_NUM = 15;	// 棋盘规模
const int EFFECTIVE_DIAGONAL_NUM = 29; // 对角线数量

/***************
* [枚举] 棋/状态
***************/
enum class Chess {
	BLANK = 0,		// 空白
	BLACK = 1,		// 黑子
	WHITE = 2, 		// 白子
	POSSIBLE = 3	// 候选落子区域
};
enum class Status {
	S_OK = 0,		// 执行成功
	F_OUTSIDE = 1,	// 执行失败 - 界外
	F_NOBLANK = 2,	// 执行失败 - 非空
	F_NOLAST = 3,	// 执行失败 - 无上一步
	G_BLACK = 4,	// 游戏结束 - 黑子获胜
	G_WHITE = 5,	// 游戏结束 - 白子获胜
	G_DRAW = 6,		// 游戏结束 - 和棋
	G_CONTINUE = 7,	// 游戏继续 - 无法判定胜负
};

/***************
* [结构] 走法
***************/
struct Move {
	int x;			// X 坐标
	int y;			// Y 坐标
	Move(int x = 0, int y = 0) : x(x), y(y) { }
};

/***************
* [类] 声明
***************/
class Player;
class Robot;

/***************
* [类] 棋局
* 包含棋局数据与相关函数
***************/
class Chessboard {
public:
	Chessboard();										// 构造函数
	~Chessboard();										// 析构函数
	void initMove();									// 棋局初始化
	Status makeMove(const int x, const int y);			// 棋局执行走法
	Status unMakeMove();								// 棋局撤销走法
	Status gameOver();									// 判断棋局胜负结果
	void printChessboard();								// 打印棋盘
	bool printChessRecord(int step);					// 打印棋局记录
protected:
	// 根据棋局记录获取当前下一步数
	inline int getCurrentStep() {
		return (chessRecord.size() + 1);
	}
	// 根据棋局记录获取当前下一棋色
	inline Chess getCurrentChess() {
		return ((chessRecord.size() & 1) ? Chess::WHITE : Chess::BLACK);
	}
private:
	Chess chessboard[GRID_NUM + 1][GRID_NUM + 1];		// 棋盘变量
	std::vector<Move> chessRecord;						// 棋局记录
	int possibleMoves[GRID_NUM + 1][GRID_NUM + 1];		// 辅助数组 候选落子位置
	char horizontals[GRID_NUM][GRID_NUM + 3];			// 辅助数组 水平行棋子
	char verticals[GRID_NUM][GRID_NUM + 3];				// 辅助数组 竖直列棋子
	char up_diagonals[EFFECTIVE_DIAGONAL_NUM][GRID_NUM + 3];	// 辅助数组 左下-右上方向对角线棋子
	char down_diagonals[EFFECTIVE_DIAGONAL_NUM][GRID_NUM + 3];	// 辅助数组 右下-左上方向对角线棋子
	
	int fx[16] = {-1, -1, -1, 0, 0, 1, 1, 1, -2, -2, -2, 0, 0, 2, 2, 2};
	int fy[16] = {-1, 0, 1, -1, 1, -1, 0, 1, -2, 0, 2, -2, 2, -2, 0, 2};
	int fnum = 16;

	// 检验招法是否在棋盘上
	inline bool inChessboard(const int x, const int y) {
		return (x >= 1 && x <= GRID_NUM && y >= 1 && y <= GRID_NUM);	
	}
	// 清空棋盘
	inline void clearChessboard() {
		memset(chessboard, 0, sizeof(chessboard));
		memset(possibleMoves, 0, sizeof(possibleMoves));
	}
	// 棋子转字符
	inline char chess2char(Chess x) {
		return '0' + int(x);
	}
	// 友元类声明
	friend class Player;
	friend class Robot;

};
#endif