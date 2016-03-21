#ifndef __ENEMY_H__
#define __ENEMY_H__
#pragma once

#include <Windows.h>
#include <vector>
#include <map>
#include <iterator>
#include "GlobalInfo.h"
#include "ConsoleManager.h"
#include "Player.h"

// ---- �\���̐錾 ----
typedef struct _tagEnemyList_
{
	int			bookListIndex;	// �}�Ӕԍ�
	std::string name;			// ����
	std::string symbol;			// �\���L��
	COLOR		txtColor;		// �����F
	int			maxHP;			// �ő�̗͒l
	int			expValue;		// �����o���l
	int			strength;		// �U����
	int			deffence;		// �h���
	int			moveTime;		// �s���\��
	int			mode;			// �������(����A�ǔ��A�p�j)
	bool		isTimid;		// �����̉�(���a���ǂ���)
	int			searchDistance;	// ���G����

}ENEMYLIST;

// ---- �N���X�錾 ----
class CEnemy
{
private:
	std::string m_Name;			// ����
	int			m_NowHP;		// ���ݑ̗͒l
	int			m_MaxHP;		// �ő�̗͒l
	int			m_EXPValue;		// �����o���l
	int			m_Strength;		// �U����
	int			m_Deffence;		// �h���
	int			m_MoveTime;		// �s���\��
	COORD		m_Position;		// ���݈ʒu
	bool		m_CanDrop;		// �A�C�e�������̗L��
	int			m_DropItemIndex;// �h���b�v�A�C�e���ԍ�
	int			m_Mode;			// ���݂̏��(����A�ǔ��A�p�j)
	bool		m_IsTimid;		// �����̉�(���a���ǂ���)
	int			m_SearchDistance;// ���G����
	int			m_BookListIndex; // �}�Ӕԍ�
	std::string m_Symbol;		// �\���L��
	COLOR		m_TXTColor;		// �����F
	int			m_Index;		// ���ʔԍ�
	bool		m_IsDead;		// ���S�t���O

public:
	CEnemy(){}
	~CEnemy();

	// �f�o�b�O�p
	void ViewInfo(COORD&, CConsoleManager**) const;	// ���\��

	void Initialize();					// ������
	void DecideAction(CPlayer**, std::vector<CEnemy>*, int, char***, int, int, CConsoleManager**); // �s������(AI)
	void Move(COORD&);						// �ړ�
	int ReceiveAttack(int&);				// �U����������
	void Damage(int&&);						// ��_���[�W
	void DeadorAlive();						// ���S����
	void SetBookData(ENEMYLIST);			// �}�Ӄf�[�^�̓o�^

	void SetName(std::string name)	{		 m_Name = name;			}
	int GetHP() const				{ return m_NowHP;				}
	void SetHP(int value)			{		 m_NowHP = value;		}
	bool GetDeadFlag()				{ return m_IsDead;				}
	COORD GetPosition()	const		{ return m_Position;			}
	void SetPosition(COORD& pos)	{		 m_Position = pos;		}
	void SetPosition(int& x, int& y)
	{
		COORD pos = {x, y};
		m_Position = pos;
	}
	int GetNumAction()				{ return m_MoveTime;			}
	std::string GetName()			{ return m_Name;				}
	int GetIndex()					{ return m_Index;				}
	void SetIndex(int value)		{		 m_Index = value;		}
	int GetEXPValue()				{ return m_EXPValue;			}
	std::string GetSymbol()			{ return m_Symbol;				}
	COLOR GetTXTColor()				{ return m_TXTColor;			}
	void SetItemHaving(bool flag, int value)
	{
		m_CanDrop = flag;
		m_DropItemIndex = value;
	}
	bool CanDropItem()				{ return m_CanDrop;				}
	int GetItemIndex()				{ return m_DropItemIndex;		}
};

#endif