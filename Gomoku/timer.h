#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_
#include <ctime>

/***************
* [类] 计时器
* 辅助进行迭代加深
***************/
class Timer {
public:
	Timer(double sec = 0);	// 构造函数
	~Timer();				// 析构函数
	void reset(double sec);	// 重置时间限制
	void start();			// 设定计时起始点
	bool check();			// 判断计时检查点
	double time();			// 返回经过时间
private:
	clock_t limitClock;		// 时间限制
	clock_t startClock;		// 起始时间
	clock_t checkClock;		// 检查时间
};





#endif
