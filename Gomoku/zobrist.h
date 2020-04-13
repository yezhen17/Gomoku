#pragma once
#ifndef ZOBRIST_H
#define ZOBRIST_H

#define hashfEXACT 0
#define hashfALPHA 1
#define hashfBETA 2

typedef long long u64;
const int TABLE_SIZE_MASK = (1 << 20) - 1;  // table大小为2的倍数，因为&运算比除法快
const int VAL_UNKNOWN = 1;
const int GRID_NUM_ = 15;

/*
Hash table
*/
struct BoardHash {
	u64 key;
	int depth;
	int flags;
	int val;
};

/*
Zobrist structure
*/
class Zobrist
{
public:
	Zobrist() { init(); }
	~Zobrist() {}
	void update(int x, int y, int role);
	u64 getKey() { return key; }
private:
	void init();
	u64 rand_64();
	u64 board[2][GRID_NUM_ + 1][GRID_NUM_ + 1];
	u64 key;
};

/*
Board status cache
*/
class Cache
{
public:
	int getCache(int depth, int alpha, int beta);
	void setCache(int val, int flags, int depth);
	void resetCache();
	void update(int x, int y, int role) { zobrist.update(x, y, role); }

private:
	Zobrist zobrist;
};
	

#endif