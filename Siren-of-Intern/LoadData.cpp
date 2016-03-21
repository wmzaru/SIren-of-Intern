#include "LoadData.h"

// �֐����FGetLoadingInfo		�t�@�C���ǂݍ���
// ���@���FFileName				�t���A�f�[�^�\���̃t�@�C����
// �@�@�@�FpNumFloor			�t���A�����i�[����ϐ��A�h���X
// �@�@�@�FppFileName			�j�t���A�f�[�^��ۑ����Ă���t�@�C�������i�[����ϐ��̃A�h���X�̃A�h���X
// �@�@�@�FpNumEnter			���s�J�E���^
// �@�@�@�FconsoleManager		�R���\�[���N���X
// �@�@�@�Fbg					�w�i�F
// �@�@�@�Ftxt					�����F
// �߂�l�F						�����A���s
bool CLoadData::GetLoadingInfo(const std::string& FileName, int* pNumFloor, std::string** ppFileName, int* pNumEnter, CConsoleManager** consoleManager, COLOR bg, COLOR txt)
{
	COORD pos = {0, (*pNumEnter)};
	char buffer[256];
	int delayTime = 25;

	std::string str = "�t���A�\���f�[�^�̓ǂݍ��݂��J�n";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);

	// �t�@�C���̓ǂݍ���
	std::ifstream ifs( FileName );
	if( ifs.fail() )
	{
		std::cout << "�t�@�C���̓ǂݍ��݂Ɏ��s" << std::endl;
		return false;
	}

	// �t���A���̓ǂݍ���
	std::string LineText;
	std::getline( ifs, LineText );
	std::string number;
	std::istringstream info( LineText );
	std::getline( info, number , ',' );
	(*pNumFloor) = atoi(number.c_str());

	// 2�s��(�R�����g�s)���X�L�b�v
	std::getline( ifs, LineText );

	// �v�f��0�̎�
	if ( (*pNumFloor) == 0 )
	{
		str = "�ǂݍ��݃}�b�v����0�Ȃ̂ŏI�����܂��BENTER�������Ă��������B";
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, WAIT, DONOTDELAY);
		std::cin.clear();
		std::cin.seekg(0);
		std::cin.ignore('\0');
		std::getchar();
		return false;
	}
	str = "�ǂݍ��݃}�b�v���F";
	::_itoa_s((*pNumFloor), buffer, 256 * sizeof(char), 10);
	str += buffer;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);

	// �v�f�i�[�ϐ��̐���
	if ((*pNumFloor) == 1) // �s�����̓ǂݍ���
	{
		(*ppFileName) = new std::string();
	}
	else
	{
		(*ppFileName) = new std::string[(*pNumFloor)];
	}

	// �ϐ��Ƀf�[�^���i�[����
	str = "�}�b�v�f�[�^�t�@�C�����ǂݍ���";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);

	int i = 0;
	while( getline( ifs, LineText ))
	{
		std::string element;
		std::istringstream stream( LineText );
		// �擾����1�s�̃f�[�^�� "," �ŕ������Đ擪�f�[�^�̂ݕϐ��֊i�[
		while( getline( stream, element, ',' ))
		{
			// �s�̕�����𕪉������v�f��\��
			str = "Data ";
			::_itoa_s(i + 1, buffer, 256 * sizeof(char), 10);
			str += buffer;
			str += " : " + element;
			(*consoleManager)->CheckClear(pNumEnter);
			pos.Y = (*pNumEnter)++;
			(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);

			// �t�@�C�������i�[
			(*ppFileName)[i++] = element;
			break;
		}
		if (i == (*pNumFloor))
		{
			break;
		}
	}

	str = "�ǂݍ��݊����B���ɐi�݂܂��B";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);
	return true;
}

// �֐����FLoadMapData			�t�@�C���ǂݍ���
// ���@���FFileName				�t���A�f�[�^�\���̃t�@�C����
// �@�@�@�FpMapData[]			�}�b�v�f�[�^�i�[��
// �@�@�@�Fwidth				��ʕ�
// �@�@�@�Fheight				��ʍ���
// �߂�l�F						�����A���s
bool CLoadData::LoadMapData(std::string& FileName, char*** pMapData, int width, int height, int* pNumEnter, CConsoleManager** consoleManager, COLOR bg, COLOR txt)
{
	COORD pos = {0, (*pNumEnter)};
	int delayLine = 100;

	// �t�@�C���̓ǂݍ���
	std::string str = FileName + " �̓ǂݍ��݂��J�n";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter);
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	pos.X += str.size();

	std::ifstream ifs( FileName );
	if( ifs.fail() )
	{
		return false;
	}

	std::string LineText;
	// 1,2�s�ڂ̃R�����g���X�L�b�v
	std::getline( ifs, LineText );
	std::getline( ifs, LineText );

	// �ϐ��Ƀf�[�^���i�[����
	int i = 0;
	while( getline( ifs, LineText ))
	{
		std::string element;
		std::istringstream stream( LineText );
		int j = 0;
		// �擾����1�s�̃f�[�^�� "," �ŕ������Ď��o���ĕ�����f�[�^�ɘA��
		while( getline( stream, element, ',' ))
		{
			(*pMapData)[i][j] = atoi(element.c_str());
			if (++j == width)
			{
				break;
			}
		}
		if (++i == height)
		{
			break;
		}
	}

	str = " -> ����";
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	(*pNumEnter)++;
	(*consoleManager)->CheckClear(pNumEnter);

	return true;
}

// �֐����FLoadEnemyData		�G�f�[�^�̓ǂݍ���
// ���@���FFileName				�G�f�[�^�̃t�@�C����
// �@�@�@�FpNumEnemyType		�G��ސ��ւ̃A�h���X
// �@�@�@�FppEnemyList			�G�f�[�^�i�[�p�|�C���^�ւ̃A�h���X
// �@�@�@�FpNumEnter			���s�J�E���^
// �@�@�@�FconsoleManager		�R���\�[���N���X
// �@�@�@�Fbg					�w�i�F
// �@�@�@�Ftxt					�����F
// �߂�l�F						�����A���s
bool CLoadData::LoadEnemyData(const std::string& FileName, int* pNumEnemyType, ENEMYLIST** ppEnemyList, int* pNumEnter, CConsoleManager** consoleManager, COLOR bg, COLOR txt)
{
	// �g���K�[���o
	(*consoleManager)->Trigger();

	COORD pos = {0, (*pNumEnter)};
	char buffer[256];
	int delayLine = 200;

	std::string str = "�G�f�[�^�̓ǂݍ��݂��J�n";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// �t�@�C���̓ǂݍ���
	std::ifstream ifs( FileName );
	if( ifs.fail() )
	{
		std::cout << "�t�@�C���̓ǂݍ��݂Ɏ��s" << std::endl;
		return false;
	}

	// ==== �G��ސ��̓ǂݍ��� ====
	std::string LineText;
	std::getline( ifs, LineText );
	std::string number;
	std::istringstream info( LineText );
	std::getline( info, number , ',' );
	(*pNumEnemyType) = atoi(number.c_str());

	// �v�f��0�̎�
	if ( (*pNumEnemyType) == 0 )
	{
		str = "�ǂݍ��ݓG�f�[�^����0�Ȃ̂ŏI�����܂��BENTER�������Ă��������B";
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, WAIT, DONOTDELAY);
		std::cin.clear();
		std::cin.seekg(0);
		std::cin.ignore('\0');
		std::getchar();
		return false;
	}
	str = "�ǂݍ��݂���G�f�[�^�̐��F";
	::_itoa_s((*pNumEnemyType), buffer, 256 * sizeof(char), 10);
	str += buffer;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// �R�����g�s�ɂȂ��Ă���2,3,4,5�s�ڂ��X�L�b�v
	std::getline( ifs, LineText ); // 2�s��
	std::getline( ifs, LineText ); // 3�s��
	std::getline( ifs, LineText ); // 4�s��
	std::getline( ifs, LineText ); // 5�s��

	// �v�f�i�[�ϐ��̐���
	if ((*pNumEnemyType) == 1) // �s�����̓ǂݍ���
	{
		(*ppEnemyList) = new ENEMYLIST();
	}
	else
	{
		(*ppEnemyList) = new ENEMYLIST[(*pNumEnemyType)];
	}

	// �ϐ��Ƀf�[�^���i�[����
	str = "�e�G�f�[�^�̓ǂݍ���";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// �ǂݍ��ݍ��ڂ̕\��
	std::string item = LineText;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, item, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// �󔒏����p�N���X
	trimLeftRightSpace trimmer;

	int row = 0;
	while( getline( ifs, LineText ))
	{
		// �g���K�[���o
		(*consoleManager)->Trigger();

		// 1�s�̗v�f��\��
		str = LineText;
		(*consoleManager)->CheckClear(pNumEnter);
		if ((*pNumEnter) == 0)
		{
			pos.Y = (*pNumEnter)++;
			(*consoleManager)->WriteTextCharacter(bg, txt, pos, item, NOWAIT, DONOTDELAY);
		}
		pos.Y = (*pNumEnter)++;
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);

		// �擾����1�s�̃f�[�^�� "," �ŕ������ēG�f�[�^�̃����o�Ɋ��蓖��
		std::string element;
		std::istringstream stream( LineText );
		int column = 0;
		while( getline( stream, element, ',' ))
		{
			// �ǂݍ��݃f�[�^��
			//typedef struct _tagEnemyList_
			//{
			//	int			bookListIndex;	// �}�Ӕԍ�
			//	std::string name;			// ����
			//	std::string symbol;			// �\���L��
			//	COLOR		txtColor;		// �����F
			//	int			maxHP;			// �ő�̗͒l
			//	int			expValue;		// �����o���l
			//	int			strength;		// �U����
			//	int			deffence;		// �h���
			//	int			moveTime;		// �s���\��
			//	int			mode;			// �������(����A�ǔ��A�p�j)
			//	bool		isTimid;		// �����̉�(���a���ǂ���)
			//	int			searchDistance;	// ���G����
			//
			//}ENEMYLIST;

			switch (column)
			{
			case 0: // index
				(*ppEnemyList)[row].bookListIndex = atoi(element.c_str());
				break;
			case 1: // name
				str = element;
				str = trimmer.trimSpace(std::move(str));
				(*ppEnemyList)[row].name = str;
				break;
			case 2: // symbol
				str = element;
				str = trimmer.trimSpace(std::move(str));
				(*ppEnemyList)[row].symbol = str;
				break;
			case 3: // COLOR
				(*ppEnemyList)[row].txtColor = static_cast<COLOR>(atoi(element.c_str()));
				break;
			case 4: // maxHP
				(*ppEnemyList)[row].maxHP = atoi(element.c_str());
				break;
			case 5: // EXP
				(*ppEnemyList)[row].expValue = atoi(element.c_str());
				break;
			case 6: // STR
				(*ppEnemyList)[row].strength = atoi(element.c_str());
				break;
			case 7: // DEF
				(*ppEnemyList)[row].deffence = atoi(element.c_str());
				break;
			case 8: // MOVETIME
				(*ppEnemyList)[row].moveTime = atoi(element.c_str());
				break;
			case 9: // MODE
				(*ppEnemyList)[row].mode = atoi(element.c_str());
				break;
			case 10: // TIMID FLAG
				{
					int flagNumber = atoi(element.c_str());
					bool flag = (flagNumber == 1) ? true : false;
					(*ppEnemyList)[row].isTimid = flag;
				}
				break;
			case 11: // SEARCH DISTANCE
				(*ppEnemyList)[row].searchDistance = atoi(element.c_str());
				break;
			}

			// 1�s�̓ǂݍ��ݏI��
			if (++column == 12)
			{
				break;
			}
		}

		// �G�̑S�f�[�^�ǂݍ��ݏI��
		if (++row == (*pNumEnemyType))
		{
			break;
		}
		if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
		{
			Sleep(delayLine);
		}
	}

	// �G�̑S�f�[�^�ǂݍ��ݐ��ُ�(�s��������Ȃ��Ȃ�)
	if (row != (*pNumEnemyType))
	{
		return false;
	}

	str = "�ǂݍ��݊����B���ɐi�݂܂��B";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	return true;
}


// �֐����FLoadItemData			�A�C�e���f�[�^�̓ǂݍ���
// ���@���FFileName				�A�C�e���f�[�^�̃t�@�C����
// �@�@�@�FpNumEnemyType		�A�C�e����ސ��ւ̃A�h���X
// �@�@�@�FppItemList			�A�C�e���f�[�^�i�[�p�|�C���^�ւ̃A�h���X
// �@�@�@�FpNumEnter			���s�J�E���^
// �@�@�@�FconsoleManager		�R���\�[���N���X
// �@�@�@�Fbg					�w�i�F
// �@�@�@�Ftxt					�����F
// �߂�l�F						�����A���s
bool CLoadData::LoadItemData(const std::string& FileName, int* pNumItemType, ITEM** ppItemList, int* pNumEnter, CConsoleManager** consoleManager, COLOR bg, COLOR txt)
{
	// �g���K�[���o
	(*consoleManager)->Trigger();
	
	COORD pos = {0, (*pNumEnter)};
	char buffer[256];
	int delayLine = 200;

	std::string str = "�A�C�e���f�[�^�̓ǂݍ��݂��J�n";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// �t�@�C���̓ǂݍ���
	std::ifstream ifs( FileName );
	if( ifs.fail() )
	{
		std::cout << "�t�@�C���̓ǂݍ��݂Ɏ��s" << std::endl;
		return false;
	}

	// ==== �A�C�e����ސ��̓ǂݍ��� ====
	std::string LineText;
	std::getline( ifs, LineText );
	std::string number;
	std::istringstream info( LineText );
	std::getline( info, number , ',' );
	(*pNumItemType) = atoi(number.c_str());

	// �v�f��0�̎�
	if ( (*pNumItemType) == 0 )
	{
		str = "�ǂݍ��݃A�C�e������0�Ȃ̂ŏI�����܂��BENTER�������Ă��������B";
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, WAIT, DONOTDELAY);
		std::cin.clear();
		std::cin.seekg(0);
		std::cin.ignore('\0');
		std::getchar();
		return false;
	}
	str = "�ǂݍ��݂���A�C�e���f�[�^�̐��F";
	::_itoa_s((*pNumItemType), buffer, 256 * sizeof(char), 10);
	str += buffer;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// �R�����g�s�ɂȂ��Ă���2,3�s�ڂ��X�L�b�v
	std::getline( ifs, LineText ); // 2�s��
	std::getline( ifs, LineText ); // 3�s��

	// �v�f�i�[�ϐ��̐���
	if ((*pNumItemType) == 1) // �s�����̓ǂݍ���
	{
		(*ppItemList) = new ITEM();
	}
	else
	{
		(*ppItemList) = new ITEM[(*pNumItemType)];
	}

	// �ϐ��Ƀf�[�^���i�[����
	str = "�e�A�C�e���f�[�^�̓ǂݍ���";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// �ǂݍ��ݍ��ڂ̕\��
	std::string item = LineText;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, item, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// �󔒏����p�N���X
	trimLeftRightSpace trimmer;

	int row = 0;
	while( getline( ifs, LineText ))
	{
		// �g���K�[���o
		Sleep(0);
		(*consoleManager)->Trigger();
		Sleep(0);

		// 1�s�̗v�f��\��
		str = LineText;
		(*consoleManager)->CheckClear(pNumEnter);
		if ((*pNumEnter) == 0)
		{
			pos.Y = (*pNumEnter)++;
			(*consoleManager)->WriteTextCharacter(bg, txt, pos, item, NOWAIT, DONOTDELAY);
		}
		pos.Y = (*pNumEnter)++;
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);

		// �擾����1�s�̃f�[�^�� "," �ŕ������ēG�f�[�^�̃����o�Ɋ��蓖��
		std::string element;
		std::istringstream stream( LineText );
		int column = 0;
		while( getline( stream, element, ',' ))
		{
			// �ǂݍ��݃f�[�^��
			//typedef struct _tagItem_
			//{
			//	int itemIndex;		// �o�^�ԍ�
			//	std::string name;	// ���O
			//	int type;			// ��ޔԍ�
			//	int value;			// �g�p�l�A����Ȃ�U�h�́A��Ȃ�񕜗�
			//}ITEM;

			switch (column)
			{
			case 0: // index
				(*ppItemList)[row].itemIndex = atoi(element.c_str());
				break;
			case 1: // name
				str = element;
				str = trimmer.trimSpace(std::move(str));
				(*ppItemList)[row].name = str;
				break;
			case 2: // type
				(*ppItemList)[row].type = atoi(element.c_str());
				break;
			case 3: // value
				(*ppItemList)[row].value = atoi(element.c_str());
				break;
			}

			// 1�s�̓ǂݍ��ݏI��
			if (++column == 4)
			{
				break;
			}
		}

		// �G�̑S�f�[�^�ǂݍ��ݏI��
		if (++row == (*pNumItemType))
		{
			break;
		}
		if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
		{
			Sleep(delayLine);
		}
	}

	// �G�̑S�f�[�^�ǂݍ��ݐ��ُ�(�s��������Ȃ��Ȃ�)
	if (row != (*pNumItemType))
	{
		return false;
	}

	str = "�ǂݍ��݊����B���ɐi�݂܂��B";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	return true;
}