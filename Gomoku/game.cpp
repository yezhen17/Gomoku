#include "game.h"
#include <iostream>
#include <string>
using namespace std;

/***************
* [函数] 运行函数 --- 程序主循环
***************/
void Game::start() {
	// TODO
	while (true) {
		// 清空屏幕
		system("cls");
		// 打印帮助信息
		this->describe();
		// 打印棋盘
		this->printChessboard();
		// 测试 - 获取/输出用户输入
		this->getUserInput(Chess::BLACK);
		cout << int(this->getOperation()) << endl;
		cout << this->getMove().x << endl;
		cout << this->getMove().y << endl;
	}
	return;
}

/***************
* [函数] 描述函数 --- 输出帮助信息
***************/
void Game::describe() {
	printf_s("************* 五子棋人机对弈  Ver 0.1 *************\n\n");
	printf_s("<newblack>: 电脑持黑先手  <move x y>: 落子  <tips>: 提示\n");
	printf_s("<newwhite>: 电脑持白后手  <withdraw>: 悔棋  <exit>: 退出\n\n");
	return;
}