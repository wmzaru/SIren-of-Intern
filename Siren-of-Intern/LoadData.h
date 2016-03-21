#ifndef __LOADDATA_H__
#define __LOADDATA_H__
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "ConsoleManager.h"
#include "Enemy.h"

// ---- �N���X�錾 ----
class CLoadData
{
public:
	// �}�b�v�\���t�@�C���ǂݍ���
	static bool GetLoadingInfo(const std::string&, int*, std::string**, int*,  CConsoleManager**, COLOR, COLOR);
	// �}�b�v���̓ǂݍ���
	static bool LoadMapData(std::string&, char***, int width, int height, int*,  CConsoleManager**, COLOR, COLOR);
	// �G�f�[�^�̓ǂݍ���
	static bool LoadEnemyData(const std::string&, int*, ENEMYLIST**, int*, CConsoleManager**, COLOR, COLOR);
	// �A�C�e���f�[�^�̓ǂݍ���
	static bool LoadItemData(const std::string&, int*, ITEM**, int*, CConsoleManager**, COLOR, COLOR);
};
#endif