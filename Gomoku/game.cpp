#include "game.h"
#include <iostream>

using namespace std;

/***************
* [函数] 运行函数 --- 入口点
***************/
void Game::start() {
	// TODO
	cout << "*************五子棋人机对弈AI*************" << endl;
	cout << "输入: newblack  电脑先手" << endl;
	cout << "输入: newwhite  电脑后手" << endl;
	cout << "输入: move x y  表示落子点" << endl;
	this->printChessboard();	
	return;
}