#include "Enemy.h"

// デストラクタ
CEnemy::~CEnemy()
{

}

// 関数名：ViewInfo			情報の表示
// 引　数：consoleManager	コンソールマネージャポインタへのアドレス
// 戻り値：
void CEnemy::ViewInfo(COORD& pos, CConsoleManager** consoleManager) const
{
	char buffer[256];
	std::string str = "";
	std::string tmp = "";
	str += "ENEMY Name:" + m_Name;
	::_itoa_s(m_Mode, buffer, 256 * sizeof(char),10);
	tmp = buffer;
	str += " Mode:" + tmp;
	::_itoa_s(m_DropItemIndex, buffer, 256 * sizeof(char),10);
	tmp = buffer;
	str += " DII:" + tmp;
	::_itoa_s(m_NowHP, buffer, 256 * sizeof(char),10);
	tmp = buffer;
	str += " HP:" + tmp;
	::_itoa_s(m_MaxHP, buffer, 256 * sizeof(char),10);
	tmp = buffer;
	str += " MAX HP:" + tmp;
	::_itoa_s(m_Strength, buffer, 256 * sizeof(char),10);
	tmp = buffer;
	str += " Str:" + tmp;
	::_itoa_s(m_Deffence, buffer, 256 * sizeof(char),10);
	tmp = buffer;
	str += " Def:" + tmp;
	::_itoa_s(m_MoveTime, buffer, 256 * sizeof(char),10);
	tmp = buffer;
	str += " MT:" + tmp;
	::_itoa_s(m_Position.X + 1, buffer, 256 * sizeof(char),10);
	tmp = buffer;
	str += " Pos:(" + tmp;
	::_itoa_s(m_Position.Y + 1, buffer, 256 * sizeof(char),10);
	tmp = buffer;
	str += "," + tmp;
	str += ")";

	(*consoleManager)->WriteTextCharacter(H_RED, H_WHITE, pos, str, NOWAIT, DONOTDELAY );
}

// 関数名：Initialize		初期化
// 引　数：
// 戻り値：
void CEnemy::Initialize()
{
	m_IsDead	= false;
	m_NowHP		= m_MaxHP;
}

// 関数名：DecideAction		行動決定
// 引　数：player			プレイヤクラス
// 　　　：enemys			全ての敵
// 　　　：numEnemy			敵の数
// 　　　：map				マップ情報
// 　　　：width			マップの幅
// 　　　：height			マップの高さ
// 　　　：consoleManager	コンソールクラス
// 戻り値：
void CEnemy::DecideAction(CPlayer** player, std::vector<CEnemy>* enemys, int numEnemy, char*** map, int width, int height, CConsoleManager** consoleManager)
{
	switch (m_Mode)
	{
	case DECIDEROUTE:
		{
			// 復帰後のルート計算
			// 巡回点方式の場合
			// 　最短距離の巡回点を求める
			// 通路と部屋の定義を作った場合
			// 　室内最近接の出口に向かい
			// 　規定の手順で通路を巡回する
			m_Mode = PATROL;
		}
		// NOT BREAK;
	case PATROL:
		{
			// 補足できる距離にいた場合、追跡を始める
			COORD playerPos = (*player)->GetPosition();
			if (m_SearchDistance >= DISTANCE(playerPos, m_Position))
			{
				m_Mode = TRACKING;
				break;
			}

			// 規定点ないし経路を巡回


		}
		break;
	case TRACKING:	// 追跡
	case ESCAPE:	// 逃走
	case WANDERING:	// 徘徊
		{
			const int numDirection = 9;
			enum DIRECTION : int { UL, U, UR, L, R, DL, D, DR, C };
			COORD playerPos = (*player)->GetPosition();
			// PLAYERの方向を求める
			// ※ ノーマライズで求めた方向が斜め[例として(1,1)]の時
			// 　 実際のX.Y座標の差分に偏りがある場合[(-15,-2)等]に
			// 　 (1,0),(0,1)に丸めるかはAIの行動最適化が
			// 　 必要となった場合に再度考察をする
			COORD direction =
			{
				VNORMALIZE(playerPos.X - m_Position.X),
				VNORMALIZE(playerPos.Y - m_Position.Y)
			};
			// ESCAPEの場合は逆方向
			if (m_Mode == ESCAPE)
			{
				direction.X *= -1;
				direction.Y *= -1;
			}
			// WANDERINGの場合はランダム
			if (m_Mode == WANDERING)
			{
				int number = rand() % numDirection;
				switch (number)
				{
				case UL:
					direction.X = -1;
					direction.Y = -1;
					break;
				case U:
					direction.X = +0;
					direction.Y = -1;
					break;
				case UR:
					direction.X = +1;
					direction.Y = -1;
					break;
				case L:
					direction.X = -1;
					direction.Y = +0;
					break;
				case R:
					direction.X = +1;
					direction.Y = +0;
					break;
				case DL:
					direction.X = -1;
					direction.Y = +1;
					break;
				case D:
					direction.X = +0;
					direction.Y = +1;
					break;
				case DR:
					direction.X = +1;
					direction.Y = +1;
					break;
				case C:
					direction.X = +0;
					direction.Y = +0;
				}
			}

			COORD targetPos = {m_Position.X + direction.X, m_Position.Y + direction.Y};

			// ==== 移動と攻撃 ====
			// TRACKING: ESCAPE:
			// 　移動可能距離にPLAYERがいたら攻撃、いなければ接近移動
			// WANDERING:
			// 　ランダムで移動方向を選定
			// 　移動方向にPLAYERがいたら3分の2で攻撃
			if (EQUAL(playerPos, targetPos))
			{
				// WANDERINGの場合は隣接時3分の2で攻撃判定
				// 3分の1は待機して終了
				if (m_Mode == WANDERING)
				{
					int seed = rand() % 3;
					if (seed == 0)
					{
						// 何もせず終了
						Move(m_Position);
						return;
					}
				}

				// 攻撃
				int damage = (*player)->ReceiveAttack(m_Strength);

				// ログを表示
				std::string cleaner( (*consoleManager)->GetScreenSize().Right, ' ');
				COORD infoPos = { 0, m_Index + 1 }; 
				char buffer[256];

				std::string str = "[" + m_Symbol + "]" + m_Name;
				str += "の攻撃！";
				if (damage > 0)
				{
					str += (*player)->GetName() + "に";
					::_itoa_s(damage, buffer, 256 * sizeof(char),10);
					str += buffer;
					str += "のダメージ！";
				}
				else
				{
					str += (*player)->GetName() + "はダメージを食らわなかった。";
				}
				(*consoleManager)->AddLog(str);
			}
			else
			{
				// TRACKING,ESCAPEの場合は移動候補の分だけ判定する
				if (m_Mode == TRACKING || m_Mode == ESCAPE)
				{
					// 移動
					char dir[3][3] =
					{
						{UL, U, UR},
						{ L, C,  R},
						{DL, D, DR}
					};

					// 巡回への遷移フラグ
					bool backPat = false;

					// 最優先方向への移動を試し
					// 移動できない場合はそれらの横を次の対象にする
					// 向いている方向に対して右1、左1、右2、左2の順に走査
					const int numDirection = 5;
					COORD target[numDirection] =
					{
						{targetPos.X, targetPos.Y},
						{targetPos.X, targetPos.Y},
						{targetPos.X, targetPos.Y},
						{targetPos.X, targetPos.Y},
						{targetPos.X, targetPos.Y}
					};


					switch (dir[direction.Y + 1][direction.X + 1])
					{
					case UL:
						target[1].X++;
						target[2].Y++;
						target[3].X += 2;
						target[4].Y += 2;
						break;
					case U:
						target[1].X++;
						target[2].X--;
						target[3].X++;
						target[3].Y++;
						target[4].X--;
						target[4].Y++;
						break;
					case UR:
						target[1].Y++;
						target[2].X--;
						target[3].Y += 2;
						target[4].X -= 2;
						break;
					case L:
						target[1].Y--;
						target[2].Y++;
						target[3].X++;
						target[3].Y--;
						target[4].X++;
						target[4].Y++;
						break;
					case R:
						target[1].Y++;
						target[2].Y--;
						target[3].X--;
						target[3].Y++;
						target[4].X--;
						target[4].Y--;
						break;
					case DL:
						target[1].Y--;
						target[2].X++;
						target[3].Y -= 2;
						target[4].X += 2;
						break;
					case D:
						target[1].X--;
						target[2].X++;
						target[3].X--;
						target[3].Y--;
						target[4].X++;
						target[4].Y--;
						break;
					case DR:
						target[1].X--;
						target[2].Y--;
						target[3].X -= 2;
						target[4].Y -= 2;
						break;
					case C: // WANDERINGのみ
						// 移動しない
						Move(m_Position);
						return;
					default:
						// 想定外の経路を選択
						assert(false);
						break;
					}
					// マップエリア外を指定しないように座標をクランプ
					for (int i = 0; i < numDirection; i++)
					{
						if (target[i].X < 0)
						{
							target[i].X = 0;
						}
						if (target[i].Y < 0)
						{
							target[i].Y = 0;
						}
						if (target[i].X > width)
						{
							target[i].X = width;
						}
						if (target[i].Y > height)
						{
							target[i].Y = height;
						}

					}
					for (int i = 0; i < numDirection; i++)
					{
						COORD otherEnemyPos;
						// 自機と重ならないように
						if (EQUAL(target[i], playerPos))
						{
							continue;
						}

						// 他の敵と重ならないように
						bool isEmpty = true;
						std::vector<CEnemy>::iterator iter;
						for (iter = (*enemys).begin(); iter != (*enemys).end(); iter++)
						{
							if ((*iter).GetIndex() == m_Index)
							{
								continue;
							}
							otherEnemyPos = (*iter).GetPosition();
							if (EQUAL(target[i], otherEnemyPos)
								&& (*iter).GetDeadFlag() == false)
							{
								isEmpty = false;
								break;
							}
						}
						if (isEmpty == false)
						{
							continue;
						}

						if (GROUND == (*map)[target[i].Y][target[i].X])
						{
							// 移動
							Move(target[i]);

							// 追跡の場合は離脱条件を確認する
							if (m_Mode == TRACKING)
							{
								// 自機と3以上離れていたら巡回モードに移る
								if ( m_SearchDistance < DISTANCE(m_Position, playerPos))
								{
									backPat = true;
								}
							}
							break;
						}
					}
					// 巡回モードに移る
					if (backPat == true)
					{
						m_Mode = PATROL;
						break;
					}
				}//end if (m_Mode == TRACKING || m_Mode == ESCAPE)

				// WANDERINGの場合、選定した方向に誰もおらず、移動可能か1度だけ見る
				if (m_Mode == WANDERING)
				{
					COORD otherEnemyPos;
					// 自機と重ならないように
					if (EQUAL(targetPos, playerPos))
					{
						// 移動候補位置がPLAYERの位置になる場合は
						// 攻撃判定側でなされている
						return;
					}

					// 他の敵と重ならないように
					bool isEmpty = true;
					std::vector<CEnemy>::iterator iter;
					for (iter = (*enemys).begin(); iter != (*enemys).end(); iter++)
					{
						if ((*iter).GetIndex() == m_Index)
						{
							continue;
						}
						otherEnemyPos = (*iter).GetPosition();
						if (EQUAL(targetPos, otherEnemyPos)
							&& (*iter).GetDeadFlag() == false)
						{
							isEmpty = false;
							break;
						}
					}
					if (isEmpty == false)
					{
						return;
					}

					if (GROUND == (*map)[targetPos.Y][targetPos.X])
					{
						// 移動
						Move(targetPos);
					}
				}// end if (m_Mode == WANDERING)
			}
		}
		break;
	}
}

// 関数名：Move				移動
// 引　数：
// 戻り値：
void CEnemy::Move(COORD& pos)
{
	m_Position = pos;
}

// 関数名：ReceiveAttack	攻撃を受ける
// 引　数：enemyStr			相手の攻撃力
// 戻り値：
int CEnemy::ReceiveAttack(int& enemyStr)
{
	int impact = enemyStr - m_Deffence;
	if (impact > 0)
	{
		Damage(static_cast<int&&>(impact));
		return impact;
	}
	return 0;
}

// 関数名：Damage			被ダメージ
// 引　数：damageValue		ダメージ量
// 戻り値：
void CEnemy::Damage(int&& damageValue)
{
	m_NowHP -= damageValue;
	DeadorAlive();
}

// 関数名：DeadorAlive		死亡判定
// 引　数：
// 戻り値：					成功・失敗
void CEnemy::DeadorAlive()
{
	if (m_NowHP <= 0)
	{
		m_NowHP = 0;
		m_IsDead = true;
	}
	else if (m_NowHP <= 2 && m_Mode == TRACKING && m_IsTimid == true)
	{
		m_Mode = ESCAPE;
	}
}

// 関数名：SetBookData		図鑑データの登録
// 引　数：list				指定インデックスの図鑑データ
// 戻り値：
void CEnemy::SetBookData(ENEMYLIST list)
{
	m_BookListIndex = list.bookListIndex;
	m_Name			= list.name;
	m_Symbol		= list.symbol;
	m_TXTColor		= list.txtColor;
	m_MaxHP			= list.maxHP;
	m_EXPValue		= list.expValue;
	m_Strength		= list.strength;
	m_Deffence		= list.deffence;
	m_Mode			= list.mode;
	m_IsTimid		= list.isTimid;
	m_MoveTime		= list.moveTime;
	m_SearchDistance = list.searchDistance;
}