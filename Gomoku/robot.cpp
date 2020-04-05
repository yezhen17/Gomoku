#include "robot.h"
#include <regex>
using namespace std;

/***************
* [函数] 构造函数
***************/
Robot::Robot() {
	black_regex[0] = std::regex("11111");
	black_regex[1] = std::regex("011110");
	black_regex[2] = std::regex("11110|01111|11011|11101|10111");

	black_regex[3] = std::regex("001110|011100|011010|010110");
	black_regex[4] = std::regex("01110|1(110|101|011)0|0(101|011|110)1");
	black_regex[5] = std::regex("001100|01010|010010");
	// black_regex[6] = std::regex("[2#]11000|[2#]10100|[2#]10010|00011[2#]|00101[2#]|01001[2#]");

	cost_self[0] = 300000; // 五
	cost_self[1] = 10000; //活四
	cost_self[2] = 1000; //死四
	cost_self[3] = 1000; // 活三
	cost_self[4] = 100; // 死三
	cost_self[5] = 100; // 活二
	cost_self[6] = 10; // 死二
	cost_self[7] = 10; // 活一
	cost_self[8] = 1; // 死一

	cost_opp[0] = 300000; // 五
	cost_opp[1] = 10000; //活四
	cost_opp[2] = 1000; //死四
	cost_opp[3] = 1000; // 活三
	cost_opp[4] = 100; // 死三
	cost_opp[5] = 100; // 活二
	cost_opp[6] = 10; // 死二
	cost_opp[7] = 10; // 活一
	cost_opp[8] = 1; // 死一
	return;
}

/***************
* [函数] 析构函数
***************/
Robot::~Robot() {
	return;
}

/***************
* [函数] 获取机器决策 --- 接口
* 参数  Chessboard& chessboard: 棋盘, Chess chess: 棋色
***************/
Move Robot::getRobotDecision(Chessboard& chessboard, Chess chess) {
	// TODO
	return Move(1, 1);

}

/***************
* [函数] 获取合法落子点
***************/
vector<Move> Robot::createMoves(Chessboard& chessboard) {
	// TODO
	return vector<Move>();
}

/***************
* [函数] 估值算法
***************/
int Robot::evaluate(Chessboard& chessboard) {
	int black_value = 0;
	int white_value = 0;
	int cost_black[10];
	int cost_white[10];

	// 根据此时的下棋方赋值代价数组
	if (chessboard.getCurrentChess == Chess::BLACK) {
		memcpy(cost_black, cost_self, 10 * sizeof(int));
		memcpy(cost_white, cost_opp, 10 * sizeof(int));
	}
	else {
		memcpy(cost_black, cost_opp, 10 * sizeof(int));
		memcpy(cost_white, cost_self, 10 * sizeof(int));
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		for (int j = 0; j < 6; j++)
		{
			int blacksum = 0, whitesum = 0;
			std::cmatch cm;
			if (i < GRID_NUM) {
				std::regex_search(chessboard.horizontals[i], cm, black_regex[i]);
				blacksum += cm.size();
				std::regex_search(chessboard.verticals[i], cm, black_regex[i]);
				blacksum += cm.size();
				std::regex_search(chessboard.horizontals[i], cm, white_regex[i]);
				whitesum += cm.size();
				std::regex_search(chessboard.verticals[i], cm, white_regex[i]);
				whitesum += cm.size();
			}
			
			std::regex_search(chessboard.up_diagonals[i], cm, black_regex[i]);
			blacksum += cm.size();
			std::regex_search(chessboard.down_diagonals[i], cm, black_regex[i]);
			blacksum += cm.size();
			std::regex_search(chessboard.up_diagonals[i], cm, white_regex[i]);
			whitesum += cm.size();
			std::regex_search(chessboard.down_diagonals[i], cm, white_regex[i]);
			whitesum += cm.size();

			black_value += cm.size() * cost_black[i];
			white_value += cm.size() * cost_white[i];
		}
	}
	/*
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 6; j++)
		{
			std::cmatch cm;
			std::regex_search(chessboard.verticals[i], cm, black_regex[i]);
			black_value += cm.size() * cost_self[i];
		}
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		for (int j = 0; j < 6; j++)
		{
			std::cmatch cm;
			std::regex_search(chessboard.up_diagonals[i], cm, black_regex[i]);
			black_value += cm.size() * cost_self[i];
		}
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		for (int j = 0; j < 6; j++)
		{
			std::cmatch cm;
			std::regex_search(chessboard.down_diagonals[i], cm, black_regex[i]);
			black_value += cm.size() * cost_self[i];
		}
	}*/
	if (chessboard.getCurrentChess == Chess::BLACK)
	    return (black_value - white_value);
	else
	    return (white_value - black_value);
}
//由于五子棋搜索分支庞大，通常无法直接搜索到胜负终局，当搜索到一定深度时需要根据局面返回搜索结果。
//参考资料：
//张明亮, 吴俊, 李凡长. 五子棋机器博弈系统评估函数的设计[J]. 计算机应用, 2012, 32(07):1969-1972.
//裴博文. 五子棋人工智能权重估值算法[J]. 电脑编程技巧与维护, 2008(6):69-75.
//https://www.cnblogs.com/maxuewei2/p/4825520.html


/***************
* [函数] 搜索算法
***************/
int Robot::searchMove(Chessboard& chessboard)  {
	return 0;
}
