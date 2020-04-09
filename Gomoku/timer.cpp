#include "timer.h"

/***************
* [函数] 构造函数
***************/
Timer::Timer(double sec) {
	limitClock = clock_t(sec * double(CLOCKS_PER_SEC));
	return;
}

/***************
* [函数] 析构函数
***************/
Timer::~Timer() {
	return;
}

/***************
* [函数] 重置时间限制
***************/
void Timer::reset(double sec) {
	limitClock = clock_t(sec * double(CLOCKS_PER_SEC));
	return;
}

/***************
* [函数] 设定计时起始点
***************/
void Timer::start() {
	startClock = clock();
	return;
}

/***************
* [函数] 判断计时检查点
* 返回值 超时返回 false，否则返回 true
***************/
bool Timer::check() {
	checkClock = clock();
	return ((checkClock - startClock) <= limitClock);
}