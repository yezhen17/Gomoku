#include "player.h"
#include <iostream>
using namespace std;

/***************
* [函数] 构造函数
***************/
Player::Player() {
	operation = Operation::NEWBLACK;
	memset(buffer, 0, sizeof(buffer));
	return;
}

/***************
* [函数] 析构函数
***************/
Player::~Player() {
	return;
}

/***************
* [函数] 获取用户输入
***************/
void Player::getUserInput(Chess chess) {
	if (chess == Chess::BLACK)
		strcpy_s(icon, sizeof(icon), "[○]");
	else
		strcpy_s(icon, sizeof(icon), "[●]");
	printf_s("%s 您的回合，请输入操作:", icon);
	while (true) {
		// 获取用户输入
		move.x = move.y = 0;
		cin.getline(buffer, BUFFER_LENGTH);
		sscanf_s(buffer, "%s %X %X", order, sizeof(order), &move.x, &move.y);
		// 匹配操作
		if (strcmp(order, "newblack") == 0) {
			operation = Operation::NEWBLACK;
			return;
		}
		if (strcmp(order, "newwhite") == 0) {
			operation = Operation::NEWWHITE;
			return;
		}
		if (strcmp(order, "move") == 0) {
			operation = Operation::MOVE;
			return;
		}
		if (strcmp(order, "withdraw") == 0) {
			operation = Operation::WITHDRAW;
			return;
		}
		if (strcmp(order, "tips") == 0) {
			operation = Operation::TIPS;
			return;
		}
		if (strcmp(order, "exit") == 0) {
			operation = Operation::EXIT;
			return;
		}
		// 重新输入
		printf_s("%s 非法格式，请重输操作:", icon);
	}
	return;
}
