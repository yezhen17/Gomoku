#pragma once
#ifndef _ROBOT_H_
#define _ROBOT_H_
#include "chessboard.h"
#include "timer.h"

/***************
* [常量] 定义
***************/
const int MAX_VALUE = 0x3f3f3f3f;		// 极大值
const int MIN_VALUE = -0x3f3f3f3f;		// 极小值
const int MIN_DEPTH = 2;				// 最小搜索层数
const int MAX_DEPTH = 6;				// 最大搜索层数
const double THRESHOLD_TIME = 3.0;		// 搜索临界时间(秒)
/***************
* [结构] 定义
***************/
struct pattern {
	char P[7];
	int m;
	pattern() {}
	pattern(const char *P_, int m_) { memcpy(P, P_, 7); m = m_; }
};

/***************
* [函数] 声明
***************/
int* compute_prefix(char* P, int m);
int KMP_matcher(char* P, char* T, int m, int n);

/***************
* [类] 机器
*+ 包含机器相关算法
***************/
class Robot {
public:
	Robot();					// 构造函数
	~Robot();					// 析构函数
	Move getRobotDecision(Chessboard& chessboard);	// [接口] 获取机器决策
	
private:
	std::vector<Move> createMoves(Chessboard& chessboard);			// 获取合法落子点
	int evaluate(Chessboard& chessboard);							// 估值算法
	int evaluatePoint(Chessboard& chessboard, Move mv, Chess cur, int min = 0);
	Move searchMove(Chessboard& chessboard);						// 搜索算法
	int maxValue(Chessboard& chessboard, int depth, int a, int b);	// minimax对抗搜索 - MAX
	int minValue(Chessboard& chessboard, int depth, int a, int b);	// minimax对抗搜索 - MIN
	Chess chess;					// 所执棋色
	pattern black_p[20];
	pattern white_p[20];
	std::regex black_regex[100];	// 辅助数组
	std::regex white_regex[100];	// 辅助数组
	int cost_self[20];				// 辅助数组
	int cost_opp[20];				// 辅助数组
	int bar[16] = {12, 12, 11, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int pat_num = 12;
	Timer timer;					// 计时器实例
};

#endif
