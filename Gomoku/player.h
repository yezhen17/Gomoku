#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "chessboard.h"

/***************
* [常量] 定义
***************/
const int BUFFER_LENGTH = 128;	// 缓冲区大小

/***************
* [枚举] 操作
***************/
enum class Operation {
	NEWBLACK = 0,	// 开局 AI持黑先手 
	NEWWHITE = 1,	// 开局 AI持白后手
	MOVE = 2,		// 落子
	WITHDRAW = 3,	// 悔棋
	TIPS = 4,		// 提示
	EXIT = 5,		// 退出
};

/***************
* [类] 玩家
* 包含与玩家交互相关
***************/
class Player {
public:
	Player();						// 构造函数
	~Player();						// 析构函数
	void getUserInput(Chess chess);	// 获取用户输入
	inline Operation getOperation() {
		return operation;			// 获取操作
	}
	inline Move getMove() {
		return move;				// 获取走法
	}
private:
	Operation operation;			// 操作变量
	Move move;						// 走法变量
	char buffer[128];				// 缓冲区
	char icon[16], order[16];		// 临时字符串

};


#endif