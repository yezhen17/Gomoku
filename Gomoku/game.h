#pragma once
#ifndef _GAME_H_
#define _GAME_H_
#include "chessboard.h"
#include "player.h"
#include "robot.h"

/***************
* [类] 游戏
* 程序主类，包含游戏逻辑
***************/
class Game: public Chessboard, public Player, public Robot {
public:
	Game() { }				// 构造函数
	~Game() { }				// 析构函数
	void start();			// 运行函数 --- 入口点
};

#endif