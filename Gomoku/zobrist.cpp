#include <random>
#include <iostream>
#include "zobrist.h"

BoardHash hashTable[TABLE_SIZE_MASK + 1];

void Zobrist::init()
{
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j <= GRID_NUM_; j++) {
			for (int k = 1; k <= GRID_NUM_; k++) {
				board[i][j][k] = rand_64();
			}
		}
	}
	key = rand_64();
}

void Zobrist::update(int x, int y, int role) {
	key ^= board[role][x][y];
}

u64 Zobrist::rand_64() {
	return rand() ^ ((u64)rand() << 15) ^ ((u64)rand() << 30) ^ ((u64)rand() << 45) ^ ((u64)rand() << 60);
}

int Cache::getCache(int depth, int alpha, int beta) {
	BoardHash *hsh = &hashTable[zobrist.getKey() & TABLE_SIZE_MASK];
	if (hsh->key == zobrist.getKey()) {
		if (hsh->depth >= depth) {
			if (hsh->flags == hashfEXACT) {
				return hsh->val;
			}
			if ((hsh->flags == hashfALPHA) && (hsh->val <= alpha)) {
				return alpha;
			}
			if ((hsh->flags == hashfBETA) && (hsh->val >= beta)) {
				return beta;
			}
		}
	}
	return VAL_UNKNOWN;
}

void Cache::setCache(int val, int flags, int depth) {
	BoardHash *hsh = &hashTable[zobrist.getKey() & TABLE_SIZE_MASK];
	hsh->key = zobrist.getKey();
	hsh->val = val;
	hsh->flags = flags;
	hsh->depth = depth;
}

void Cache::resetCache() {
	memset(hashTable, 0, sizeof(hashTable));
}