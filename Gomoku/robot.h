#pragma once
#ifndef _ROBOT_H_
#define _ROBOT_H_
#include "chessboard.h"


/***************
* [常量] 定义
***************/
const int MAX_VALUE = 0x3f3f3f3f;		// 极大值
const int MIN_VALUE = -0x3f3f3f3f;		// 极小值
const int MAX_DEPTH = 2;				// 最大搜索深度

/***************
* [类] 机器
* 包含机器相关算法
***************/
class Robot {
public:
	Robot();					// 构造函数
	~Robot();					// 析构函数
	Move getRobotDecision(Chessboard& chessboard);	// [接口] 获取机器决策
	
private:
	std::vector<Move> createMoves(Chessboard &chessboard);			// 获取合法落子点
	int evaluate(Chessboard& chessboard);							// 估值算法
	Move searchMove(Chessboard& chessboard);						// 搜索算法
	int maxValue(Chessboard& chessboard, int depth, int a, int b);	// minimax对抗搜索 - MAX
	int minValue(Chessboard& chessboard, int depth, int a, int b);	// minimax对抗搜索 - MIN
	Chess chess;					// 所执棋色
	std::regex black_regex[100];	// 辅助数组
	std::regex white_regex[100];	// 辅助数组
	int cost_self[10];				// 辅助数组
	int cost_opp[10];				// 辅助数组

};

#endif
