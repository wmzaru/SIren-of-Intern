#include "Enemy.h"

// �f�X�g���N�^
CEnemy::~CEnemy()
{

}

// �֐����FViewInfo			���̕\��
// ���@���FconsoleManager	�R���\�[���}�l�[�W���|�C���^�ւ̃A�h���X
// �߂�l�F
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

// �֐����FInitialize		������
// ���@���F
// �߂�l�F
void CEnemy::Initialize()
{
	m_IsDead	= false;
	m_NowHP		= m_MaxHP;
}

// �֐����FDecideAction		�s������
// ���@���Fplayer			�v���C���N���X
// �@�@�@�Fenemys			�S�Ă̓G
// �@�@�@�FnumEnemy			�G�̐�
// �@�@�@�Fmap				�}�b�v���
// �@�@�@�Fwidth			�}�b�v�̕�
// �@�@�@�Fheight			�}�b�v�̍���
// �@�@�@�FconsoleManager	�R���\�[���N���X
// �߂�l�F
void CEnemy::DecideAction(CPlayer** player, std::vector<CEnemy>* enemys, int numEnemy, char*** map, int width, int height, CConsoleManager** consoleManager)
{
	switch (m_Mode)
	{
	case DECIDEROUTE:
		{
			// ���A��̃��[�g�v�Z
			// ����_�����̏ꍇ
			// �@�ŒZ�����̏���_�����߂�
			// �ʘH�ƕ����̒�`��������ꍇ
			// �@�����ŋߐڂ̏o���Ɍ�����
			// �@�K��̎菇�ŒʘH�����񂷂�
			m_Mode = PATROL;
		}
		// NOT BREAK;
	case PATROL:
		{
			// �⑫�ł��鋗���ɂ����ꍇ�A�ǐՂ��n�߂�
			COORD playerPos = (*player)->GetPosition();
			if (m_SearchDistance >= DISTANCE(playerPos, m_Position))
			{
				m_Mode = TRACKING;
				break;
			}

			// �K��_�Ȃ����o�H������


		}
		break;
	case TRACKING:	// �ǐ�
	case ESCAPE:	// ����
	case WANDERING:	// �p�j
		{
			const int numDirection = 9;
			enum DIRECTION : int { UL, U, UR, L, R, DL, D, DR, C };
			COORD playerPos = (*player)->GetPosition();
			// PLAYER�̕��������߂�
			// �� �m�[�}���C�Y�ŋ��߂��������΂�[��Ƃ���(1,1)]�̎�
			// �@ ���ۂ�X.Y���W�̍����ɕ΂肪����ꍇ[(-15,-2)��]��
			// �@ (1,0),(0,1)�Ɋۂ߂邩��AI�̍s���œK����
			// �@ �K�v�ƂȂ����ꍇ�ɍēx�l�@������
			COORD direction =
			{
				VNORMALIZE(playerPos.X - m_Position.X),
				VNORMALIZE(playerPos.Y - m_Position.Y)
			};
			// ESCAPE�̏ꍇ�͋t����
			if (m_Mode == ESCAPE)
			{
				direction.X *= -1;
				direction.Y *= -1;
			}
			// WANDERING�̏ꍇ�̓����_��
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

			// ==== �ړ��ƍU�� ====
			// TRACKING: ESCAPE:
			// �@�ړ��\������PLAYER��������U���A���Ȃ���ΐڋ߈ړ�
			// WANDERING:
			// �@�����_���ňړ�������I��
			// �@�ړ�������PLAYER��������3����2�ōU��
			if (EQUAL(playerPos, targetPos))
			{
				// WANDERING�̏ꍇ�͗אڎ�3����2�ōU������
				// 3����1�͑ҋ@���ďI��
				if (m_Mode == WANDERING)
				{
					int seed = rand() % 3;
					if (seed == 0)
					{
						// ���������I��
						Move(m_Position);
						return;
					}
				}

				// �U��
				int damage = (*player)->ReceiveAttack(m_Strength);

				// ���O��\��
				std::string cleaner( (*consoleManager)->GetScreenSize().Right, ' ');
				COORD infoPos = { 0, m_Index + 1 }; 
				char buffer[256];

				std::string str = "[" + m_Symbol + "]" + m_Name;
				str += "�̍U���I";
				if (damage > 0)
				{
					str += (*player)->GetName() + "��";
					::_itoa_s(damage, buffer, 256 * sizeof(char),10);
					str += buffer;
					str += "�̃_���[�W�I";
				}
				else
				{
					str += (*player)->GetName() + "�̓_���[�W��H���Ȃ������B";
				}
				(*consoleManager)->AddLog(str);
			}
			else
			{
				// TRACKING,ESCAPE�̏ꍇ�͈ړ����̕��������肷��
				if (m_Mode == TRACKING || m_Mode == ESCAPE)
				{
					// �ړ�
					char dir[3][3] =
					{
						{UL, U, UR},
						{ L, C,  R},
						{DL, D, DR}
					};

					// ����ւ̑J�ڃt���O
					bool backPat = false;

					// �ŗD������ւ̈ړ�������
					// �ړ��ł��Ȃ��ꍇ�͂����̉������̑Ώۂɂ���
					// �����Ă�������ɑ΂��ĉE1�A��1�A�E2�A��2�̏��ɑ���
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
					case C: // WANDERING�̂�
						// �ړ����Ȃ�
						Move(m_Position);
						return;
					default:
						// �z��O�̌o�H��I��
						assert(false);
						break;
					}
					// �}�b�v�G���A�O���w�肵�Ȃ��悤�ɍ��W���N�����v
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
						// ���@�Əd�Ȃ�Ȃ��悤��
						if (EQUAL(target[i], playerPos))
						{
							continue;
						}

						// ���̓G�Əd�Ȃ�Ȃ��悤��
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
							// �ړ�
							Move(target[i]);

							// �ǐՂ̏ꍇ�͗��E�������m�F����
							if (m_Mode == TRACKING)
							{
								// ���@��3�ȏ㗣��Ă����珄�񃂁[�h�Ɉڂ�
								if ( m_SearchDistance < DISTANCE(m_Position, playerPos))
								{
									backPat = true;
								}
							}
							break;
						}
					}
					// ���񃂁[�h�Ɉڂ�
					if (backPat == true)
					{
						m_Mode = PATROL;
						break;
					}
				}//end if (m_Mode == TRACKING || m_Mode == ESCAPE)

				// WANDERING�̏ꍇ�A�I�肵�������ɒN�����炸�A�ړ��\��1�x��������
				if (m_Mode == WANDERING)
				{
					COORD otherEnemyPos;
					// ���@�Əd�Ȃ�Ȃ��悤��
					if (EQUAL(targetPos, playerPos))
					{
						// �ړ����ʒu��PLAYER�̈ʒu�ɂȂ�ꍇ��
						// �U�����葤�łȂ���Ă���
						return;
					}

					// ���̓G�Əd�Ȃ�Ȃ��悤��
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
						// �ړ�
						Move(targetPos);
					}
				}// end if (m_Mode == WANDERING)
			}
		}
		break;
	}
}

// �֐����FMove				�ړ�
// ���@���F
// �߂�l�F
void CEnemy::Move(COORD& pos)
{
	m_Position = pos;
}

// �֐����FReceiveAttack	�U�����󂯂�
// ���@���FenemyStr			����̍U����
// �߂�l�F
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

// �֐����FDamage			��_���[�W
// ���@���FdamageValue		�_���[�W��
// �߂�l�F
void CEnemy::Damage(int&& damageValue)
{
	m_NowHP -= damageValue;
	DeadorAlive();
}

// �֐����FDeadorAlive		���S����
// ���@���F
// �߂�l�F					�����E���s
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

// �֐����FSetBookData		�}�Ӄf�[�^�̓o�^
// ���@���Flist				�w��C���f�b�N�X�̐}�Ӄf�[�^
// �߂�l�F
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