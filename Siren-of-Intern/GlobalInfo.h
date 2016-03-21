#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#pragma once

#include <Windows.h>
#include <iostream>
#include <utility>
#include <assert.h>
#include <string>
#include <algorithm>

// ---- Define宣言 ----
#define NUMTERRAIN	10

// ---- 列挙体宣言 ----
enum GAMESCENE : char { LOAD, SAVE, TITLE, CONTINUE, DUNGEON, CITY, DEATH, RESULT, CREDIT, };
enum MAP : char { NONE, WALL, GROUND, STAIR, };
enum ENEMYMODE : int { PATROL, TRACKING, WANDERING, ESCAPE, DECIDEROUTE, };
enum DROP : int { NOITEM = INT_MAX, };
enum DUNGEONENDMODE : int { PLAYERDEAD, HIERARCHYCLEAR, DUNGEONCLEAR, CONTINUATION, };
enum ITEMTYPE : int { WEAPON, SHIELD, MEDICINE, ARMOR, FOOD, /* */ };
enum STATE : int { _ACTION = 0x0001, _FOOT = 0x0002, _ATTACK = 0x0004, _PICKUP = 0x0008, _USEITEM = 0x0010, };
enum TARGETITEM : int { UNSPECIFIED = INT_MAX, };
enum PARAMETER : int { ATTACKPOWER, DEFENSEPOWER, };

// ---- 構造体宣言 ----
typedef struct _tagItem_
{
	int itemIndex;		// 登録番号
	std::string name;	// 名前
	int type;			// 種類番号
	int value;			// 使用値、武具なら攻防力、薬なら回復量
}ITEM;

typedef struct _tagFallItemList_
{
	int	X;
	int	Y;
	int index;
}FALLITEMLIST;

typedef std::map<int, int> ItemList;

// ---- 関数宣言 ----
__forceinline COORD MAKECOORD(int x, int y)
{
	COORD coord = {x, y};
	return coord;
}

__forceinline std::string SPACING(int numDigit)
{
	std::string str = "";
	for (int i = 0; i < numDigit; i++)
	{
		str += " ";
	}
	return str;
}

__forceinline int DIGITCOUNT(int value)
{
	int numDigit = 1;
	for (;;)
	{
		value /= 10;
		if (value == 0)
		{
			break;
		}
		numDigit++;
	}
	return numDigit;
}

__forceinline int DISTANCE(COORD& start, COORD end)
{
	int distance = 0;
	int x = abs(static_cast<int>(end.X) - static_cast<int>(start.X));
	int y = abs(static_cast<int>(end.Y) - static_cast<int>(start.Y));
	int diagonalCount = 0;
	for (;;)
	{
		if (x == 0 || y == 0)
		{
			break;
		}
		--x;
		--y;
		++diagonalCount;
	}
	distance += diagonalCount + x + y;
	return distance;
}

__forceinline bool EQUAL(COORD& a, COORD b)
{
	if (a.X == b.X && a.Y == b.Y)
	{
		return true;
	}
	return false;
}

template <typename T>
bool NEARZERO(T value)
{
	if (-0.0001 < value && value < 0.0001)
	{
		return true;
	}
	return false;
}

template <typename T>
T VNORMALIZE(T value)
{
	if (NEARZERO(value))
	{
		return 0;
	}
	if (value < 0)
	{
		return -1;
	}
	return 1;
}

// ---- class 宣言 ----
class trimLeftRightSpace
{
public:
  trimLeftRightSpace(){}
  ~trimLeftRightSpace(){}

  std::string trimSpace(std::string&& s)
  {
    std::string::iterator it_left = std::find_if_not(s.begin(), s.end(), isSpace);
    s.erase(s.begin(), it_left);

    std::string::iterator it_right = std::find_if_not(s.rbegin(), s.rend(), isSpace).base();
    s.erase(it_right, s.end());

    return s;
  }

private:

  static bool isSpace(char c)
  {
    return c == ' ';
  }

};

#endif