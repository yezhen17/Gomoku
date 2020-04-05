#include "game.h"
#include <iostream>
#include <string>
using namespace std;

const bool DEBUG_MODE = false;

/***************
* [函数] 构造函数
***************/
Game::Game(): sente(Role::PLAYER), stage(Stage::DEFAULT) {
	return;
}

/***************
* [函数] 析构函数
***************/
Game::~Game() {
	return;
}

/***************
* [函数] 运行函数 --- 程序主循环
***************/
void Game::start() {
	Status status = Status::S_OK;
	Chess chess = Chess::BLANK;
	Operation operation = Operation::NEWBLACK;
	Move move = Move(0, 0);
	while (true) {
		// *** 打印游戏信息 ***
		system("cls");		// 清空屏幕
		describe();			// 打印帮助信息
		printChessboard();	// 打印棋盘
		// *** 用户进行决策 ***
		chess = getCurrentChess();
		while (true) {
			// * 获取用户输入 *
			getUserInput(chess);
			operation = getUserOperation();
			move = getUserMove();
			if (DEBUG_MODE)
				printf_s("@ 用户输入操作：(%d,%d,%d)\n", int(operation), move.x, move.y);
			// * 处理用户输入 *
			if (operation == Operation::NEWBLACK) {
				sente = Role::ROBOT;		// 更新先行方
				stage = Stage::UNDERWAY;	// 更新游戏阶段
				initMove();					// 重置棋盘
				break;
			}
			if (operation == Operation::NEWWHITE) {
				sente = Role::PLAYER;		// 更新先行方
				stage = Stage::UNDERWAY;	// 更新游戏阶段
				initMove();					// 重置棋盘
				break;
			}
			if (operation == Operation::MOVE) {
				if (stage != Stage::UNDERWAY) {
					printf_s("[×] 对局未开始，请键入<newblack>/<newwhite>以开始对局。\n");
					continue;
				}
				status = makeMove(move.x, move.y);	// 行棋
				if (status == Status::F_OUTSIDE) {
					printf_s("[×] 非法落子 - 界外！\n");
					continue;
				}
				if (status == Status::F_NOBLANK) {
					printf_s("[×] 非法落子 - 非空！\n");
					continue;
				}
				break;
			}
			if (operation == Operation::WITHDRAW) {
				if (stage != Stage::UNDERWAY) {
					printf_s("[×] 对局未开始，请键入<newblack>/<newwhite>以开始对局。\n");
					continue;
				}
				status = unMakeMove();				// 悔棋 - 撤销AI
				status = unMakeMove();				// 悔棋 - 撤销玩家
				break;
			}
			if (operation == Operation::TIPS) {
				if (stage != Stage::UNDERWAY) {
					printf_s("[×] 对局未开始，请键入<newblack>/<newwhite>以开始对局。\n");
					continue;
				}
				printf_s("[√] 只要思想不滑坡，问题总比办法多！\n");
				continue;
			}
			if (operation == Operation::EXIT) {
				exit(0);							// 退出程序
			}
		}
		// *** AI进行决策 ***
		chess = getCurrentChess();
		if (stage != Stage::UNDERWAY)
			continue;
		if ((chess == Chess::BLACK) && (sente == Role::PLAYER))
			continue;
		if ((chess == Chess::WHITE) && (sente == Role::ROBOT))
			continue;
		move = getRobotDecision(*this, chess);	// 获取机器决策
		if (makeMove(move.x, move.y) != Status::S_OK)	// 行棋
			exit(1);
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
