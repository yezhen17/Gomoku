#include "game.h"
#include <iostream>
#include <string>
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
	char buffer[100] = "";
	char a[100], b[100], c[200];
	int x; int y;
	while (true) {
		printf_s("[○] 您的回合，请输入操作:");
		scanf_s("%[^\n]%*c", buffer, sizeof(buffer));
		sscanf_s(buffer, "%s %d %d", a, sizeof(a), &x, &y);
		cout << a << endl << x << endl << y << endl;
	}
	return;
}
