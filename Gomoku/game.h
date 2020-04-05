#pragma once
#ifndef _GAME_H_
#define _GAME_H_
#include "chessboard.h"
#include "player.h"
#include "robot.h"

/***************
* [枚举] 角色
***************/
enum class Role {
	PLAYER = 0,		// 玩家
	ROBOT = 1,		// 机器
};

/***************
* [类] 游戏
* 程序主类，控制游戏逻辑
***************/
class Game: public Chessboard, public Player, public Robot {
public:
	Game() { }				// 构造函数
	~Game() { }				// 析构函数
	void start();			// 运行函数 --- 程序主循环
	void describe();		// 描述函数 --- 输出帮助信息
private:
	// TODO

};
#endif