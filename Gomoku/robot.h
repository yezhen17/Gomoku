#pragma once
#ifndef _ROBOT_H_
#define _ROBOT_H_
#include "chessboard.h"

/***************
* [类] 机器
* 包含机器相关算法
***************/
class Robot {
public:
	Robot();					// 构造函数
	~Robot();					// 析构函数
	Move getRobotDecision(Chessboard& chessboard, Chess chess);	// [接口] 获取机器决策
private:
	std::vector<Move> createMoves(Chessboard &chessboard);	// 获取合法落子点
	int evaluate(Chessboard& chessboard);					// 估值算法，返回估值
	int searchMove(Chessboard& chessboard);					// 搜索核心
	std::regex black_regex[100];
	std::regex white_regex[100];
	int cost[7];
};

#endif
