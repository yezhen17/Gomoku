#pragma once
#ifndef _ROBOT_H_
#define _ROBOT_H_
#include "chessboard.h"
#include "timer.h"

/***************
* [常量] 定义
***************/
const int MAX_VALUE = 0x3f3f3f3f;			// 极大值
const int MIN_VALUE = -0x3f3f3f3f;			// 极小值
const int MIN_DEPTH = 2;					// 最小搜索层数
const int MAX_DEPTH = 6;					// 最大搜索层数
const double THRESHOLD_TIME = 3.0;			// 搜索临界时间(秒)
const bool ITERATIVE_DEEPENING_MODE = true;	// 迭代加深模式
// 置为 false，则将根据 MAX_DEPTH 固定搜索层数


/***************
* [函数] 声明
***************/
void compute_prefix(char* P, int *pi, int m);
int KMP_matcher(char* P, int *pi, char* T, int m, int n);

/***************
* [结构] 定义
***************/
struct pattern {
	char P[7];  // 模式字符串
	int m;  // 长度
	int pi[8];  // 前缀数组
	pattern() {}
	pattern(pattern& pattern_) { memcpy(P, pattern_.P, 7); m = pattern_.m; memcpy(pi, pattern_.pi, 8); }
	pattern(const char *P_, int m_) { memcpy(P, P_, 7); m = m_; compute_prefix(P, pi, m); }
};

// PointRate 
const int FIVE_OURS = 10000000;
const int FIVE_OPPO = 1000000;
const int ALIVE_FOUR_OURS = 100000;
const int ALIVE_FOUR_OPPO = 10000;
const int DOUBLE_FOUR_OURS = 100000;
const int DOUBLE_FOUR_OPPO = 10000;
const int THREE_OR_DEAD_FOUR_OURS = 1000;
const int DEAD_FOUR_OPPO = 1000;
const int WEAK_OURS = 100;

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
	pattern black_p[20];  // 所有黑色模式
	pattern white_p[20];  // 所有白色模式
	int cost_self[20];	// 己方模式价值数组
	int cost_opp[20];	 // 对方模式价值数组
	int bar[16] = {12, 12, 11, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // 每行/列/对角线一色棋子数量对应的起始模式编号
	int pat_num = 12;  // 模式串数量
	// int bar[16] = {19, 19, 18, 7, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// int pat_num = 19;
	Timer timer;					// 计时器实例
};

#endif
