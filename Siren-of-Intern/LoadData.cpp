#include "LoadData.h"

// 関数名：GetLoadingInfo		ファイル読み込み
// 引　数：FileName				フロアデータ構造のファイル名
// 　　　：pNumFloor			フロア数を格納する変数アドレス
// 　　　：ppFileName			核フロアデータを保存しているファイル名を格納する変数のアドレスのアドレス
// 　　　：pNumEnter			改行カウンタ
// 　　　：consoleManager		コンソールクラス
// 　　　：bg					背景色
// 　　　：txt					文字色
// 戻り値：						成功、失敗
bool CLoadData::GetLoadingInfo(const std::string& FileName, int* pNumFloor, std::string** ppFileName, int* pNumEnter, CConsoleManager** consoleManager, COLOR bg, COLOR txt)
{
	COORD pos = {0, (*pNumEnter)};
	char buffer[256];
	int delayTime = 25;

	std::string str = "フロア構造データの読み込みを開始";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);

	// ファイルの読み込み
	std::ifstream ifs( FileName );
	if( ifs.fail() )
	{
		std::cout << "ファイルの読み込みに失敗" << std::endl;
		return false;
	}

	// フロア数の読み込み
	std::string LineText;
	std::getline( ifs, LineText );
	std::string number;
	std::istringstream info( LineText );
	std::getline( info, number , ',' );
	(*pNumFloor) = atoi(number.c_str());

	// 2行目(コメント行)をスキップ
	std::getline( ifs, LineText );

	// 要素が0の時
	if ( (*pNumFloor) == 0 )
	{
		str = "読み込みマップ数が0なので終了します。ENTERを押してください。";
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, WAIT, DONOTDELAY);
		std::cin.clear();
		std::cin.seekg(0);
		std::cin.ignore('\0');
		std::getchar();
		return false;
	}
	str = "読み込みマップ数：";
	::_itoa_s((*pNumFloor), buffer, 256 * sizeof(char), 10);
	str += buffer;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);

	// 要素格納変数の生成
	if ((*pNumFloor) == 1) // 行だけの読み込み
	{
		(*ppFileName) = new std::string();
	}
	else
	{
		(*ppFileName) = new std::string[(*pNumFloor)];
	}

	// 変数にデータを格納する
	str = "マップデータファイル名読み込み";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);

	int i = 0;
	while( getline( ifs, LineText ))
	{
		std::string element;
		std::istringstream stream( LineText );
		// 取得した1行のデータを "," で分割して先頭データのみ変数へ格納
		while( getline( stream, element, ',' ))
		{
			// 行の文字列を分解した要素を表示
			str = "Data ";
			::_itoa_s(i + 1, buffer, 256 * sizeof(char), 10);
			str += buffer;
			str += " : " + element;
			(*consoleManager)->CheckClear(pNumEnter);
			pos.Y = (*pNumEnter)++;
			(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);

			// ファイル名を格納
			(*ppFileName)[i++] = element;
			break;
		}
		if (i == (*pNumFloor))
		{
			break;
		}
	}

	str = "読み込み完了。次に進みます。";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, delayTime);
	return true;
}

// 関数名：LoadMapData			ファイル読み込み
// 引　数：FileName				フロアデータ構造のファイル名
// 　　　：pMapData[]			マップデータ格納先
// 　　　：width				画面幅
// 　　　：height				画面高さ
// 戻り値：						成功、失敗
bool CLoadData::LoadMapData(std::string& FileName, char*** pMapData, int width, int height, int* pNumEnter, CConsoleManager** consoleManager, COLOR bg, COLOR txt)
{
	COORD pos = {0, (*pNumEnter)};
	int delayLine = 100;

	// ファイルの読み込み
	std::string str = FileName + " の読み込みを開始";
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
	// 1,2行目のコメントをスキップ
	std::getline( ifs, LineText );
	std::getline( ifs, LineText );

	// 変数にデータを格納する
	int i = 0;
	while( getline( ifs, LineText ))
	{
		std::string element;
		std::istringstream stream( LineText );
		int j = 0;
		// 取得した1行のデータを "," で分割して取り出して文字列データに連結
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

	str = " -> 完了";
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	(*pNumEnter)++;
	(*consoleManager)->CheckClear(pNumEnter);

	return true;
}

// 関数名：LoadEnemyData		敵データの読み込み
// 引　数：FileName				敵データのファイル名
// 　　　：pNumEnemyType		敵種類数へのアドレス
// 　　　：ppEnemyList			敵データ格納用ポインタへのアドレス
// 　　　：pNumEnter			改行カウンタ
// 　　　：consoleManager		コンソールクラス
// 　　　：bg					背景色
// 　　　：txt					文字色
// 戻り値：						成功、失敗
bool CLoadData::LoadEnemyData(const std::string& FileName, int* pNumEnemyType, ENEMYLIST** ppEnemyList, int* pNumEnter, CConsoleManager** consoleManager, COLOR bg, COLOR txt)
{
	// トリガー検出
	(*consoleManager)->Trigger();

	COORD pos = {0, (*pNumEnter)};
	char buffer[256];
	int delayLine = 200;

	std::string str = "敵データの読み込みを開始";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// ファイルの読み込み
	std::ifstream ifs( FileName );
	if( ifs.fail() )
	{
		std::cout << "ファイルの読み込みに失敗" << std::endl;
		return false;
	}

	// ==== 敵種類数の読み込み ====
	std::string LineText;
	std::getline( ifs, LineText );
	std::string number;
	std::istringstream info( LineText );
	std::getline( info, number , ',' );
	(*pNumEnemyType) = atoi(number.c_str());

	// 要素が0の時
	if ( (*pNumEnemyType) == 0 )
	{
		str = "読み込み敵データ数が0なので終了します。ENTERを押してください。";
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, WAIT, DONOTDELAY);
		std::cin.clear();
		std::cin.seekg(0);
		std::cin.ignore('\0');
		std::getchar();
		return false;
	}
	str = "読み込みする敵データの数：";
	::_itoa_s((*pNumEnemyType), buffer, 256 * sizeof(char), 10);
	str += buffer;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// コメント行になっている2,3,4,5行目をスキップ
	std::getline( ifs, LineText ); // 2行目
	std::getline( ifs, LineText ); // 3行目
	std::getline( ifs, LineText ); // 4行目
	std::getline( ifs, LineText ); // 5行目

	// 要素格納変数の生成
	if ((*pNumEnemyType) == 1) // 行だけの読み込み
	{
		(*ppEnemyList) = new ENEMYLIST();
	}
	else
	{
		(*ppEnemyList) = new ENEMYLIST[(*pNumEnemyType)];
	}

	// 変数にデータを格納する
	str = "各敵データの読み込み";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// 読み込み項目の表示
	std::string item = LineText;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, item, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// 空白除去用クラス
	trimLeftRightSpace trimmer;

	int row = 0;
	while( getline( ifs, LineText ))
	{
		// トリガー検出
		(*consoleManager)->Trigger();

		// 1行の要素を表示
		str = LineText;
		(*consoleManager)->CheckClear(pNumEnter);
		if ((*pNumEnter) == 0)
		{
			pos.Y = (*pNumEnter)++;
			(*consoleManager)->WriteTextCharacter(bg, txt, pos, item, NOWAIT, DONOTDELAY);
		}
		pos.Y = (*pNumEnter)++;
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);

		// 取得した1行のデータを "," で分割して敵データのメンバに割り当て
		std::string element;
		std::istringstream stream( LineText );
		int column = 0;
		while( getline( stream, element, ',' ))
		{
			// 読み込みデータ列
			//typedef struct _tagEnemyList_
			//{
			//	int			bookListIndex;	// 図鑑番号
			//	std::string name;			// 名称
			//	std::string symbol;			// 表示記号
			//	COLOR		txtColor;		// 文字色
			//	int			maxHP;			// 最大体力値
			//	int			expValue;		// 所持経験値
			//	int			strength;		// 攻撃力
			//	int			deffence;		// 防御力
			//	int			moveTime;		// 行動可能回数
			//	int			mode;			// 初期状態(巡回、追尾、徘徊)
			//	bool		isTimid;		// 逃走の可否(臆病かどうか)
			//	int			searchDistance;	// 索敵距離
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

			// 1行の読み込み終了
			if (++column == 12)
			{
				break;
			}
		}

		// 敵の全データ読み込み終了
		if (++row == (*pNumEnemyType))
		{
			break;
		}
		if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
		{
			Sleep(delayLine);
		}
	}

	// 敵の全データ読み込み数異常(行数が足りないなど)
	if (row != (*pNumEnemyType))
	{
		return false;
	}

	str = "読み込み完了。次に進みます。";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	return true;
}


// 関数名：LoadItemData			アイテムデータの読み込み
// 引　数：FileName				アイテムデータのファイル名
// 　　　：pNumEnemyType		アイテム種類数へのアドレス
// 　　　：ppItemList			アイテムデータ格納用ポインタへのアドレス
// 　　　：pNumEnter			改行カウンタ
// 　　　：consoleManager		コンソールクラス
// 　　　：bg					背景色
// 　　　：txt					文字色
// 戻り値：						成功、失敗
bool CLoadData::LoadItemData(const std::string& FileName, int* pNumItemType, ITEM** ppItemList, int* pNumEnter, CConsoleManager** consoleManager, COLOR bg, COLOR txt)
{
	// トリガー検出
	(*consoleManager)->Trigger();
	
	COORD pos = {0, (*pNumEnter)};
	char buffer[256];
	int delayLine = 200;

	std::string str = "アイテムデータの読み込みを開始";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// ファイルの読み込み
	std::ifstream ifs( FileName );
	if( ifs.fail() )
	{
		std::cout << "ファイルの読み込みに失敗" << std::endl;
		return false;
	}

	// ==== アイテム種類数の読み込み ====
	std::string LineText;
	std::getline( ifs, LineText );
	std::string number;
	std::istringstream info( LineText );
	std::getline( info, number , ',' );
	(*pNumItemType) = atoi(number.c_str());

	// 要素が0の時
	if ( (*pNumItemType) == 0 )
	{
		str = "読み込みアイテム数が0なので終了します。ENTERを押してください。";
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, WAIT, DONOTDELAY);
		std::cin.clear();
		std::cin.seekg(0);
		std::cin.ignore('\0');
		std::getchar();
		return false;
	}
	str = "読み込みするアイテムデータの数：";
	::_itoa_s((*pNumItemType), buffer, 256 * sizeof(char), 10);
	str += buffer;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// コメント行になっている2,3行目をスキップ
	std::getline( ifs, LineText ); // 2行目
	std::getline( ifs, LineText ); // 3行目

	// 要素格納変数の生成
	if ((*pNumItemType) == 1) // 行だけの読み込み
	{
		(*ppItemList) = new ITEM();
	}
	else
	{
		(*ppItemList) = new ITEM[(*pNumItemType)];
	}

	// 変数にデータを格納する
	str = "各アイテムデータの読み込み";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// 読み込み項目の表示
	std::string item = LineText;
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, item, NOWAIT, DONOTDELAY);
	if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
	{
		Sleep(200);
	}

	// 空白除去用クラス
	trimLeftRightSpace trimmer;

	int row = 0;
	while( getline( ifs, LineText ))
	{
		// トリガー検出
		Sleep(0);
		(*consoleManager)->Trigger();
		Sleep(0);

		// 1行の要素を表示
		str = LineText;
		(*consoleManager)->CheckClear(pNumEnter);
		if ((*pNumEnter) == 0)
		{
			pos.Y = (*pNumEnter)++;
			(*consoleManager)->WriteTextCharacter(bg, txt, pos, item, NOWAIT, DONOTDELAY);
		}
		pos.Y = (*pNumEnter)++;
		(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);

		// 取得した1行のデータを "," で分割して敵データのメンバに割り当て
		std::string element;
		std::istringstream stream( LineText );
		int column = 0;
		while( getline( stream, element, ',' ))
		{
			// 読み込みデータ列
			//typedef struct _tagItem_
			//{
			//	int itemIndex;		// 登録番号
			//	std::string name;	// 名前
			//	int type;			// 種類番号
			//	int value;			// 使用値、武具なら攻防力、薬なら回復量
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

			// 1行の読み込み終了
			if (++column == 4)
			{
				break;
			}
		}

		// 敵の全データ読み込み終了
		if (++row == (*pNumItemType))
		{
			break;
		}
		if ( ! (*consoleManager)->Press(KEY::RETURNKEY))
		{
			Sleep(delayLine);
		}
	}

	// 敵の全データ読み込み数異常(行数が足りないなど)
	if (row != (*pNumItemType))
	{
		return false;
	}

	str = "読み込み完了。次に進みます。";
	(*consoleManager)->CheckClear(pNumEnter);
	pos.Y = (*pNumEnter)++;
	(*consoleManager)->WriteTextCharacter(bg, txt, pos, str, NOWAIT, DONOTDELAY);
	return true;
}