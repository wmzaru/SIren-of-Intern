#ifndef __LOADDATA_H__
#define __LOADDATA_H__
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "ConsoleManager.h"
#include "Enemy.h"

// ---- クラス宣言 ----
class CLoadData
{
public:
	// マップ構成ファイル読み込み
	static bool GetLoadingInfo(const std::string&, int*, std::string**, int*,  CConsoleManager**, COLOR, COLOR);
	// マップ情報の読み込み
	static bool LoadMapData(std::string&, char***, int width, int height, int*,  CConsoleManager**, COLOR, COLOR);
	// 敵データの読み込み
	static bool LoadEnemyData(const std::string&, int*, ENEMYLIST**, int*, CConsoleManager**, COLOR, COLOR);
	// アイテムデータの読み込み
	static bool LoadItemData(const std::string&, int*, ITEM**, int*, CConsoleManager**, COLOR, COLOR);
};
#endif