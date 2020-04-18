#include "game.h"
#include <iostream>
#include <string>
using namespace std;

const bool DEBUG_MODE = false;

/***************
* [函数] 构造函数
***************/
Game::Game(): sente(Role::DEFAULT), winner(Chess::BLANK), stage(Stage::DEFAULT) {
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
	double TIME = -1;
	while (true) {
		// *** 打印游戏信息 ***
		if (!DEBUG_MODE) system("cls");					// 清空屏幕			
		describe();						// 打印帮助信息
		printChessboard();				// 打印棋盘
		printChessRecord(-2);			// 打印棋局记录
		printChessRecord(-1);			// 打印棋局记录
		if (stage == Stage::GAMEOVER) {	// 打印获胜信息
			printf_s("%-12s", "【游戏结束】 ");
			if (winner == Chess::BLACK)
				printf_s("○ 获胜！\n");
			else if (winner == Chess::WHITE)
				printf_s("● 获胜！\n");
			else
				printf_s("和棋！\n");
		}
		if (TIME >= 0) {
			printf_s("[√] AI决策用时：%.3lf s.\n", TIME);
			TIME = -1;
		}
		printf_s("\n");
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
			// <newblack>
			if (operation == Operation::NEWBLACK) {
				sente = Role::ROBOT;		// 更新先行方
				stage = Stage::UNDERWAY;	// 更新游戏阶段
				initMove();					// 重置棋盘
				break;
			}
			// <newwhite>
			if (operation == Operation::NEWWHITE) {
				sente = Role::PLAYER;		// 更新先行方
				stage = Stage::UNDERWAY;	// 更新游戏阶段
				initMove();					// 重置棋盘
				break;
			}
			// <move>
			if (operation == Operation::MOVE) {
				if (stage == Stage::UNDERWAY) {
					// 行棋
					status = makeMove(move.x, move.y);
					if (DEBUG_MODE)
						printf_s("@ 玩家落子：(%d,%d,%d)\n", move.x, move.y, status);
					if (status == Status::F_OUTSIDE) {
						printf_s("[×] 非法落子 - 界外！\n");
						continue;
					}
					if (status == Status::F_NOBLANK) {
						printf_s("[×] 非法落子 - 非空！\n");
						continue;
					}
					// 胜负判断
					status = gameOver();
					if (status == Status::G_BLACK) {
						winner = Chess::BLACK;
						stage = Stage::GAMEOVER;
					}
					else if (status == Status::G_WHITE) {
						winner = Chess::WHITE;
						stage = Stage::GAMEOVER;
					}
					else if (status == Status::G_DRAW) {
						winner = Chess::BLANK;
						stage = Stage::GAMEOVER;
					}
					break;
				}
				if (stage == Stage::GAMEOVER) {
					printf_s("[×] 对局已结束，请键入<newblack>/<newwhite>以开始新对局。\n");
					continue;
				}
				if (stage == Stage::DEFAULT) {
					printf_s("[×] 对局未开始，请键入<newblack>/<newwhite>以开始对局。\n");
					continue;
				}
			}
			// <withdraw>
			if (operation == Operation::WITHDRAW) {
				if (stage == Stage::UNDERWAY || stage == Stage::GAMEOVER) {
					// 悔棋 撤销两步以包含所有情况
					status = unMakeMove();
					status = unMakeMove();
					stage = Stage::UNDERWAY;	// 游戏继续
					break;
				}
				if (stage == Stage::DEFAULT) {
					printf_s("[×] 对局未开始，请键入<newblack>/<newwhite>以开始对局。\n");
					continue;
				}
			}
			// <tips>
			if (operation == Operation::TIPS) {
				if (stage == Stage::UNDERWAY) {
					TIMER.start();
					move = getRobotDecision(*this);						// 获取机器决策
					TIME = TIMER.time();
					printf_s("[√] AI建议：(%X, %X)\n", move.x, move.y);
					printf_s("[√] AI决策用时：%.3lf s.\n", TIME);
					TIME = -1;
					continue;
				}
				if (stage == Stage::GAMEOVER) {
					printf_s("[√] 游戏已结束，请键入<newblack>/<newwhite>以开始新对局，或键入<withdraw>以悔棋。\n");
					continue;
				}
				if (stage == Stage::DEFAULT) {
					printf_s("[×] 对局未开始，请键入<newblack>/<newwhite>以开始对局。\n");
					continue;
				}
			}
			// <exit>
			if (operation == Operation::EXIT) {
				printf_s("[√] 已退出游戏。\n");
				exit(0);							// 退出程序
			}
			// <record>
			if (operation == Operation::RECORD) {
				if (stage == Stage::UNDERWAY || stage == Stage::GAMEOVER) {
					if (!printChessRecord(move.x))
						printf_s("[×] 棋局未进行至该步（当前步数：第%d步），请重新输入。\n", getCurrentStep());
					// 追加打印比赛结果
					if (move.x == 0 && stage == Stage::GAMEOVER) {	// 打印获胜信息
						printf_s("%-12s", "【游戏结束】 ");
						if (winner == Chess::BLACK)
							printf_s("○ 获胜！\n");
						else if (winner == Chess::WHITE)
							printf_s("● 获胜！\n");
						else
							printf_s("和棋！\n");
					}
					continue;
				}
				if (stage == Stage::DEFAULT) {
					printf_s("[×] 对局未开始，请键入<newblack>/<newwhite>以开始对局。\n");
					continue;
				}
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
		if (DEBUG_MODE)
			printf_s("@ AI决策前步数：(%d)\n", getCurrentStep());
		TIMER.start();
		move = getRobotDecision(*this);						// 获取机器决策
		TIME = TIMER.time();
		if (DEBUG_MODE)
			printf_s("@ AI决策后步数：(%d)\n", getCurrentStep());
		// 行棋
		if (DEBUG_MODE)
			printf_s("@ AI落子：(%d,%d)\n", move.x, move.y);
		if (makeMove(move.x, move.y) != Status::S_OK) {	
			printf_s("[×] AI决策故障，程序已终止。\n");
			exit(1);
		}
		// 胜负判断
		status = gameOver();
		if (status == Status::G_BLACK) {
			winner = Chess::BLACK;
			stage = Stage::GAMEOVER;
		}
		else if (status == Status::G_WHITE) {
			winner = Chess::WHITE;
			stage = Stage::GAMEOVER;
		}
		else if (status == Status::G_DRAW) {
			winner = Chess::BLANK;
			stage = Stage::GAMEOVER;
		}
	}
	return;
}

/***************
* [函数] 描述函数 --- 输出帮助信息
***************/
void Game::describe() {
	printf_s("************* 五子棋人机对弈  Ver 0.1 *************\n\n");
	printf_s("<newblack>: 电脑持黑先手  <move x y>: 落子  <tips>: 提示\n");
	printf_s("<newwhite>: 电脑持白后手  <withdraw>: 悔棋  <exit>: 退出\n");
	printf_s("<record x>: 输出棋局记录（正数-正数步数 负数-倒数步数 0-所有记录）\n\n");
	return;
}
