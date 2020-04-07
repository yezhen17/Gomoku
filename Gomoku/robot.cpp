#include "robot.h"

#include <regex>
using namespace std;

int *compute_prefix(char *P, int m)
{
	int *pi = new int[m + 1];
	pi[1] = 0;
	int k = 0;
	for (int q = 2; q <= m; q++)
	{
		while (k > 0 && P[k + 1] != P[q]) k = pi[k];
		if (P[k + 1] == P[q]) k++;
		pi[q] = k;
	}
	return pi;
}
int KMP_matcher(char *P, char*T, int m, int n)
{
	int *pi = compute_prefix(P, m);
	int sum = 0;
	int q = 0;
	for (int i = 1; i <= n; i++)
	{
		while (q > 0 && P[q + 1] != T[i]) q = pi[q];
		if (P[q + 1] == T[i]) q++;
		if (q == m)
		{
			sum++;
			q = pi[q];
		}
	}
	delete []pi;
	return sum;
}

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
	white_regex[0] = std::regex("22222");
	white_regex[1] = std::regex("022220");
	white_regex[2] = std::regex("22220|02222|22022|22202|20222");

	white_regex[3] = std::regex("002220|022200|022020|020220");
	white_regex[4] = std::regex("02220|2(220|202|022)0|0(202|022|220)2");
	white_regex[5] = std::regex("002200|02020|020020");
	// black_regex[6] = std::regex("[2#]11000|[2#]10100|[2#]10010|00011[2#]|00101[2#]|01001[2#]");
	black_p[0] = pattern(".11111", 5);
	black_p[1] = pattern(".011110", 6);
	black_p[2] = pattern(".11110", 5);
	black_p[3] = pattern(".01111", 5);
	black_p[4] = pattern(".11101", 5);
	black_p[5] = pattern(".10111", 5);
	black_p[6] = pattern(".11011", 5);
	black_p[7] = pattern(".001110", 6);
	black_p[8] = pattern(".011100", 6);
	black_p[9] = pattern(".011010", 6);
	black_p[10] = pattern(".010110", 6);
	black_p[11] = pattern(".001100", 6);
	white_p[0] = pattern(".22222", 5);
	white_p[1] = pattern(".022220", 6);
	white_p[2] = pattern(".22220", 5);
	white_p[3] = pattern(".02222", 5);
	white_p[4] = pattern(".22202", 5);
	white_p[5] = pattern(".20222", 5);
	white_p[6] = pattern(".22022", 5);
	white_p[7] = pattern(".002220", 6);
	white_p[8] = pattern(".022200", 6);
	white_p[9] = pattern(".022020", 6);
	white_p[10] = pattern(".020220", 6);
	white_p[11] = pattern(".002200", 6);

	cost_self[0] = 300000; // 五
	cost_self[1] = 10000; 
	cost_self[2] = 1000; 
	cost_self[3] = 1000; 
	cost_self[4] = 1000; 
	cost_self[5] = 1000; 
	cost_self[6] = 1000; 
	cost_self[7] = 1000; 
	cost_self[8] = 1000; 
	cost_self[9] = 1000;
	cost_self[10] = 1000;
	cost_self[11] = 100;

	cost_opp[0] = 300000; // 五
	cost_opp[1] = 10000;
	cost_opp[2] = 1000;
	cost_opp[3] = 1000;
	cost_opp[4] = 1000;
	cost_opp[5] = 1000;
	cost_opp[6] = 1000;
	cost_opp[7] = 1000;
	cost_opp[8] = 1000;
	cost_opp[9] = 1000;
	cost_opp[10] = 1000;
	cost_opp[11] = 100;
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
Move Robot::getRobotDecision(Chessboard& chessboard) {
	// 首步 - 天元
	if (chessboard.getCurrentStep() == 1)
		return Move((GRID_NUM + 1) >> 1, (GRID_NUM + 1) >> 1);
	// 进行搜索
	chess = chessboard.getCurrentChess();	// 记录所执棋色
	return searchMove(chessboard);
}

/***************
* [函数] 获取合法落子点
***************/
vector<Move> Robot::createMoves(Chessboard& chessboard) {
	vector<Move> moves;
	for (int i = 1; i <= GRID_NUM; i++)
		for (int j = GRID_NUM; j >= 1; j--)
			if (chessboard.possibleMoves[i][j] > 0 && chessboard.possibleMoves[i][j] < chessboard.fnum)
				moves.push_back(Move(i, j));

	vector<int> evals;
	Move best;
	Chess cur = chessboard.getCurrentChess();
	int max = 0;
	for (const auto mv : moves) {
		int eval = evaluatePoint(chessboard, mv, cur);
		if (eval > max) {
			max = eval;
			best = mv;
		}
		evals.push_back(eval);
	}
	if (max >= 10000) {
		vector<Move> res;
		res.push_back(best);
		return res;
	}
	for (int i = 0; i < moves.size(); i++) {
		for (int j = 0; j < moves.size() - i - 1; j++) {
			if (evals[j] < evals[j + 1]) {
				Move tmp = moves[j + 1];
				moves[j + 1] = moves[j];
				moves[j] = tmp;
				int tmp2 = evals[j + 1];
				evals[j + 1] = evals[j];
				evals[j] = tmp2;
			}
		}
	}
	if (max >= 2000) {
		for (int i = moves.size() - 1; i >= 0; i--) {
			if (evals[i] < 1000)
				moves.pop_back();
		}
	}
	
	return moves;
}

int Robot::evaluatePoint(Chessboard& chessboard, Move mv, Chess cur, int min)
{
	if (chessboard.getCurrentStep() < 6) {
		return 0;
	}
	char same, oppo;
	if (cur == Chess::BLACK) {
		same = '1';
		oppo = '2';
	}
	else {
		same = '2';
		oppo = '1';
	}
	int x = mv.x, y = mv.y;
	bool left_block[4] = { true, true, true, true };
	bool right_block[4] = { true, true, true, true };
	bool left_block_second[4] = { true, true, true, true };
	bool right_block_second[4] = { true, true, true, true };
	int left_first[4] = { 0, 0, 0, 0 };
	int left_second[4] = { 0, 0, 0, 0 };
	int right_first[4] = { 0, 0, 0, 0 };
	int right_second[4] = { 0, 0, 0, 0 };

	int oppo_whole[4] = { 0, 0, 0, 0 };
	bool oppo_both_open[4] = { true, true, true, true };


	char *c = &chessboard.horizontals[x - 1][y - 1];
	char *d = c + 2;
	char *e = c, *f = d;
	while (*c == same) {
		++left_first[0];
		--c;
	}
	if (*c == '0') {
		left_block[0] = false;
		--c;
		while (*c == same) {
			++left_second[0];
			--c;
		}
		if (*c == '0') {
			left_block_second[0] = false;
		}
	}
	while (*d == same) {
		++right_first[0];
		++d;
	}
	if (*d == '0') {
		right_block[0] = false;
		++d;
		while (*d == same) {
			++right_second[0];
			++d;
		}
		if (*d == '0') {
			right_block_second[0] = false;
		}
	}
	while (*e == oppo) {
		++oppo_whole[0];
		--e;
	}
	if (*e != '0') {
		oppo_both_open[0] = false;
	}
	while (*f == oppo) {
		++oppo_whole[0];
		++f;
	}
	if (*f != '0') {
		oppo_both_open[0] = false;
	}

	c = &chessboard.verticals[y - 1][x - 1];
	d = c + 2;
	e = c, f = d;
	while (*c == same) {
		++left_first[1];
		--c;
	}
	if (*c == '0') {
		left_block[1] = false;
		--c;
		while (*c == same) {
			++left_second[1];
			--c;
		}
		if (*c == '0') {
			left_block_second[1] = false;
		}
	}
	while (*d == same) {
		++right_first[1];
		++d;
	}
	if (*d == '0') {
		right_block[1] = false;
		++d;
		while (*d == same) {
			++right_second[1];
			++d;
		}
		if (*d == '0') {
			right_block_second[1] = false;
		}
	}
	while (*e == oppo) {
		++oppo_whole[1];
		--e;
	}
	if (*e != '0') {
		oppo_both_open[1] = false;
	}
	while (*f == oppo) {
		++oppo_whole[1];
		++f;
	}
	if (*f != '0') {
		oppo_both_open[1] = false;
	}

	if (x + y <= GRID_NUM + 1) {
		c = &chessboard.up_diagonals[x + y - 2][y - 1];
	}
	else {
		c = &chessboard.up_diagonals[x + y - 2][GRID_NUM - x];
	}
	d = c + 2;
	e = c, f = d;
	while (*c == same) {
		++left_first[2];
		--c;
	}
	if (*c == '0') {
		left_block[2] = false;
		--c;
		while (*c == same) {
			++left_second[2];
			--c;
		}
		if (*c == '0') {
			left_block_second[2] = false;
		}
	}
	while (*d == same) {
		++right_first[2];
		++d;
	}
	if (*d == '0') {
		right_block[2] = false;
		++d;
		while (*d == same) {
			++right_second[2];
			++d;
		}
		if (*d == '0') {
			right_block_second[2] = false;
		}
	}
	while (*e == oppo) {
		++oppo_whole[2];
		--e;
	}
	if (*e != '0') {
		oppo_both_open[2] = false;
	}
	while (*f == oppo) {
		++oppo_whole[2];
		++f;
	}
	if (*f != '0') {
		oppo_both_open[2] = false;
	}

	if (y - x <= 0) {
		c = &chessboard.down_diagonals[y - x + GRID_NUM - 1][y - 1];
	}
	else {
		c = &chessboard.down_diagonals[y - x + GRID_NUM - 1][x - 1];
	}
	d = c + 2;
	e = c, f = d;
	while (*c == same) {
		++left_first[3];
		--c;
	}
	if (*c == '0') {
		left_block[3] = false;
		--c;
		while (*c == same) {
			++left_second[3];
			--c;
		}
		if (*c == '0') {
			left_block_second[3] = false;
		}
	}
	while (*d == same) {
		++right_first[3];
		++d;
	}
	if (*d == '0') {
		right_block[3] = false;
		++d;
		while (*d == same) {
			++right_second[3];
			++d;
		}
		if (*d == '0') {
			right_block_second[3] = false;
		}
	}
	while (*e == oppo) {
		++oppo_whole[3];
		--e;
	}
	if (*e != '0') {
		oppo_both_open[3] = false;
	}
	while (*f == oppo) {
		++oppo_whole[3];
		++f;
	}
	if (*f != '0') {
		oppo_both_open[3] = false;
	}

	int sum_self = 0;
	int d4a3 = 0;
	int weak = 0;
	for (int i = 0; i < 4; i++) {
		int seq_same = left_first[i] + right_first[i];
		int seq_gap1_same_left = seq_same + left_second[i];
		int seq_gap1_same_right = seq_same + right_second[i];
		int seq_gap2_same = seq_gap1_same_left + right_second[i];

		bool either_block = left_block[i] || right_block[i];
		bool both_block = left_block[i] && right_block[i];

		//  冲四活三or双活三权值比死4低，不合理，待改进
		if (seq_same == 4) sum_self = 10000000; // 己方5
		else if (oppo_whole[i] == 4) sum_self = 1000000; // 对方5
		else if (seq_gap1_same_left > 2 && seq_gap1_same_right > 2 && seq_same == 2) sum_self = 100000; // 己方1011101
		else if (seq_same == 3 && !either_block) sum_self = 100000; // 己方活4
		else if (oppo_whole[i] == 3 && oppo_both_open[i]) sum_self = 10000; // 对方活4
		else if (seq_same == 3 && !both_block && either_block) ++d4a3; // 己方死4
		else if (seq_same == 2 && (seq_gap1_same_left > 2 || seq_gap1_same_right > 2))  ++d4a3; // 己方死4
		else if (seq_same == 2 && !either_block) ++d4a3; // 己方活3
		else if (seq_same == 1 && (seq_gap1_same_left > 2 && !left_block_second[i] && !right_block[i]
			|| seq_gap1_same_right > 2 && !right_block_second[i] && !left_block[i])) ++d4a3; //己方活3

		// else if (seq_same == 1 && !left_block_second[i] && !right_block_second[i]) ++weak; // 己方活2

	}
	if (sum_self) return sum_self;
	if (d4a3) return d4a3 * 1000;
	return weak * 100;
}

/***************
* [函数] 估值算法
* 根据类变量 chess, 确定当前棋色
***************/
int Robot::evaluate(Chessboard& chessboard) {
	int black_value = 0;
	int white_value = 0;
	int cost_black[20];
	int cost_white[20];
	// 根据此时的下棋方赋值代价数组
	if (chess == Chess::BLACK) {
		memcpy(cost_black, cost_self, 20 * sizeof(int));
		memcpy(cost_white, cost_opp, 20 * sizeof(int));
	}
	else {
		memcpy(cost_black, cost_opp, 20 * sizeof(int));
		memcpy(cost_white, cost_self, 20 * sizeof(int));
	}
	
	std::regex exist_regex("[12]");
	for (int i = 0; i < GRID_NUM; i++) {
		if (!std::regex_search(chessboard.horizontals[i], exist_regex))
			continue;
		for (int j = 0; j < 12; j++)
		{
			black_value += KMP_matcher(black_p[j].P, chessboard.horizontals[i], black_p[j].m, GRID_NUM) * cost_black[j];
			white_value += KMP_matcher(white_p[j].P, chessboard.horizontals[i], white_p[j].m, GRID_NUM) * cost_white[j];
		}
	}
	for (int i = 0; i < GRID_NUM; i++) {
		if (!std::regex_search(chessboard.verticals[i], exist_regex))
			continue;
		for (int j = 0; j < 12; j++)
		{
			black_value += KMP_matcher(black_p[j].P, chessboard.verticals[i], black_p[j].m, GRID_NUM) * cost_black[j];
			white_value += KMP_matcher(white_p[j].P, chessboard.verticals[i], white_p[j].m, GRID_NUM) * cost_white[j];
		}
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		if (!std::regex_search(chessboard.up_diagonals[i], exist_regex))
			continue;
		int len = 15 - abs(14 - i);
		for (int j = 0; j < 12; j++)
		{
			black_value += KMP_matcher(black_p[j].P, chessboard.up_diagonals[i], black_p[j].m, len) * cost_black[j];
			white_value += KMP_matcher(white_p[j].P, chessboard.up_diagonals[i], white_p[j].m, len) * cost_white[j];
		}
	}
	for (int i = 0; i < EFFECTIVE_DIAGONAL_NUM; i++) {
		if (!std::regex_search(chessboard.down_diagonals[i], exist_regex))
			continue;
		int len = 15 - abs(14 - i);
		for (int j = 0; j < 12; j++)
		{
			black_value += KMP_matcher(black_p[j].P, chessboard.down_diagonals[i], black_p[j].m, len) * cost_black[j];
			white_value += KMP_matcher(white_p[j].P, chessboard.down_diagonals[i], white_p[j].m, len) * cost_white[j];
		}
	}
	if (chess == Chess::BLACK)
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
* 使用α-β剪枝
***************/
Move Robot::searchMove(Chessboard& chessboard)  {
	int depth = 1, a = MIN_VALUE, b = MAX_VALUE;
	int max_value = MIN_VALUE, tmp_value = 0;
	Move move(0, 0);
	vector<Move> moves = createMoves(chessboard);
	for (auto m : moves) {

		if (!chessboard.inChessboard(m.x, m.y) || !chessboard.isBlank(m.x, m.y)) {
			printf_s("**** XJ CORE ****\n");
			exit(1);
		}

		if (chessboard.makeMove(m.x, m.y) != Status::S_OK) {
			printf_s("[×] AI搜索故障(makeMove)，程序已终止。\n");
			exit(1);
		}
		tmp_value = minValue(chessboard, depth + 1, a, b);
		if (chessboard.unMakeMove() != Status::S_OK) {
			printf_s("[×] AI搜索故障(unMakeMove)，程序已终止。\n");
			exit(1);
		}
		if (tmp_value > max_value) {
			max_value = tmp_value;
			move = m;
		}
		if (max_value > a)
			a = max_value;
	}
	return move;
}

/***************
* [函数] minimax对抗搜索 - MAX
* 使用α-β剪枝
* 循环不变式: 调用时，此时的局面期待我方落子
* 循环不变式: 调用前后，chessboard状态不变
***************/
int Robot::maxValue(Chessboard& chessboard, int depth, int a, int b) {
	if (depth > MAX_DEPTH)
		return evaluate(chessboard);
	int max_value = MIN_VALUE, tmp_value = 0;
	vector<Move> moves = createMoves(chessboard);
	for (auto m : moves) {

		if (!chessboard.inChessboard(m.x, m.y) || !chessboard.isBlank(m.x, m.y)) {
			printf_s("**** XJ MIN ****\n");
			exit(1);
		}

		if (chessboard.makeMove(m.x, m.y) != Status::S_OK) {
			printf_s("[×] AI搜索故障(makeMove)，程序已终止。\n");
			exit(1);
		}
		tmp_value = minValue(chessboard, depth + 1, a, b);
		if (chessboard.unMakeMove() != Status::S_OK) {
			printf_s("[×] AI搜索故障(unMakeMove)，程序已终止。\n");
			exit(1);
		}
		if (tmp_value > max_value)
			max_value = tmp_value;
		if (max_value >= b)
			return max_value;
		if (max_value > a)
			a = max_value;
	}
	return max_value;
}

/***************
* [函数] minimax对抗搜索 - MIN
* 使用α-β剪枝
* 循环不变式: 调用时，此时的局面期待对方落子
* 循环不变式: 调用前后，chessboard状态不变
***************/
int Robot::minValue(Chessboard& chessboard, int depth, int a, int b) {
	int temp = chessboard.getCurrentStep();
	if (depth > MAX_DEPTH)
		return evaluate(chessboard);
	int min_value = MAX_VALUE, tmp_value = 0;
	vector<Move> moves = createMoves(chessboard);
	for (auto m : moves) {

		if (!chessboard.inChessboard(m.x, m.y) || !chessboard.isBlank(m.x, m.y)) {
			printf_s("**** XJ MIN ****\n");
			exit(1);
		}

		if (chessboard.makeMove(m.x, m.y) != Status::S_OK) {
			printf_s("[×] AI搜索故障(makeMove)，程序已终止。\n");
			exit(1);
		}
		tmp_value = maxValue(chessboard, depth + 1, a, b);
		if (chessboard.unMakeMove() != Status::S_OK) {
			printf_s("[×] AI搜索故障(unMakeMove)，程序已终止。\n");
			exit(1);
		}
		if (tmp_value < min_value)
			min_value = tmp_value;
		if (min_value <= a)
			return min_value;
		if (min_value < b)
			b = min_value;
	}
	return min_value;
}
