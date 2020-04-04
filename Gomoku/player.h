#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "chessboard.h"

/***************
* [枚举] 操作
***************/
enum Operation {
	NEWBLACK = 0,	// 开局 AI持黑先手 
	NEWWHITE = 1,	// 开局 AI持白后手
	MOVE = 2,		// 落子
	WITHDRAW = 4,	// 悔棋
	PROMPT = 5,		// 提示
};

/***************
* [类] 玩家
* 包含与玩家交互相关
***************/
class Player {
public:
	Player() { }	// 构造函数
	~Player() { }	// 析构函数
};


#endif