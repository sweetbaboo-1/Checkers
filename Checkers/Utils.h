#pragma once
typedef struct IVector2
{
	int x;
	int y;
} IVector2;

typedef struct IVector3
{
	int x;
	int y;
	int z;
} IVector3;

typedef struct IVector4
{
	int x;
	int y;
	int z;
	int w;
} IVector4;

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
