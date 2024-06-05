#pragma once

typedef struct Vector2Int
{
	int x;
	int y;
} Vector2Int;

typedef struct Vector3Int
{
	int x;
	int y;
	int z;
} Vector3Int;

enum GameResult
{
	WhiteWins,
	BlackWins,
	Draw,
	WhiteTimeout,
	BlackTimeout,
	DrawByArbiter,
	WhiteIllegalMove,
	BlackIllegalMove
};
