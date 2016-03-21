#ifndef __PLAYER_H__
#define __PLAYER_H__
#pragma once

// ---- Include File ----
#include <Windows.h>
#include <map>
#include <iterator>
#include "GlobalInfo.h"
#include "ConsoleManager.h"

// ---- �N���X�錾 ----
class CPlayer
{
private:
	std::string	m_Name;				// ����
	std::string	m_Symbol;			// �\���L��
	int			m_NowHP;			// ���ݑ̗͒l
	int			m_MaxHP;			// �ő�̗͒l
	int			m_InitHP;			// HP�����l
	int			m_HungryGauge;		// ���ݖ����x
	int			m_HungryLimit;		// �ő喞���x
	int			m_InitHungry;		// �����x�����l
	int			m_Level;			// ���x��
	int			m_NowEXP;			// ���݂̌o���l
	int			m_MaxEXP;			// �o���l�̏��
	int			m_InitEXP;			// �o���l�����l
	int			m_Strength;			// �ؗ�
	int			m_InitStr;			// �ؗ͏����l
	int			m_Toughness;		// �挒��
	int			m_InitDef;			// �挒�������l
	int			m_ActionCount;		// �s���J�E���^�[
	int			m_ACLimit;			// �J�E���^�[���E�l
	int			m_NRSpeed;			// ���R�񕜑��x(�s���J�E���g�l���ݐ�) (Natural Recovery)
	int			m_RRSpeed;			// �}���񕜑��x(�s���J�E���g�l���ݐ�)
	int			m_MoveTime;			// �s���\��
	COORD		m_Position;			// ���݈ʒu
	ItemList	m_HaveItem;			// �����A�C�e��
	int			m_UseItemIndex;		// �g�p����A�C�e���̔ԍ�
	bool		m_IsDead;			// ���S�t���O
	COLOR		m_SymbolTXTColor;	// �V���{���\�������F
	COLOR		m_InfoBGColor;		// �C���t�H�\���w�i�F
	COLOR		m_InfoTXTColor;		// �C���t�H�\�������F

	// ����
	int			m_WeaponIndex;		// �������̕���̐}�Ӕԍ�
	int			m_ShieldIndex;		// �������̏��̐}�Ӕԍ�
	int			m_ArmorIndex;		// �������̊Z�̐}�Ӕԍ�
	int			m_AttackPower;		// �����U����
	int			m_DefensePower;		// �����h���
	ITEM**		m_ppItemBook;		// �A�C�e���}�Ӄ|�C���^���w���A�h���X

public:
	CPlayer(int&&, int&&, int&&, int&&, int&&, int&&, int&&);
	~CPlayer();

	// �f�o�b�O�p
	void ViewInfo(COORD&, CConsoleManager**) const;	// ���\��

	void Initialize();			// ������
	void ResetParameter();		// �f�[�^�̃��Z�b�g
	void Foot(bool);			// ������ (1�^�[���ҋ@)
	void Move(COORD&);			// �ړ�
	int ReceiveAttack(int&);	// �U�����󂯂�
	int Heal(int&);				// �̗͉�
	int EatFood(int&);			// �����x��
	void Damage(int&&);			// ��_���[�W
	void DeadorAlive();			// ���S����
	void GetEXP(int, std::string, CConsoleManager**); // �o���l�̎擾

	void EquippedWeapon(int);	// ����̑���
	void EquippedShield(int);	// ���̑���
	void EquippedArmor(int);	// �h��̑���
	void UndressedWeapon();		// ������O��
	void UndressedShield();		// �����O��
	void UndressedArmor();		// �h����O��
	void Refresh(PARAMETER);	// �퓬�͂̍X�V

	// �ꗗ�̎Q��
	ItemList GetItem()			{ return m_HaveItem;				}
	// �A�C�e���̒ǉ�
	void AddItem(int index)
	{
		std::map<int, int>::iterator iter = m_HaveItem.find(index);
		if (iter != m_HaveItem.end())
		{
			iter->second++;
		}
		else
		{
			m_HaveItem.insert(std::pair<int, int>(index, 1));
		}
	}
	void SetItemList(ItemList list){	 m_HaveItem = list;			}
	void SetUseItemIndex(int value){	 m_UseItemIndex = value;	}
	int GetUseItemIndex()		{ return m_UseItemIndex;			}
	std::string GetName()		{ return m_Name;					}
	std::string GetSymbol()		{ return m_Symbol;					}
	int GetMaxHP() const		{ return m_MaxHP;					}
	int GetHungryLimit() const	{ return m_HungryLimit;				}
	int GetHP() const			{ return m_NowHP;					}
	void SetHP(int value)		{		 m_NowHP = value;			}
	int GetATK()				{ return m_AttackPower;				}
	int GetDEF()				{ return m_DefensePower;			}
	COORD GetPosition()	const	{ return m_Position;				}
	void SetPosition(COORD& pos){		 m_Position = pos;			}
	void SetPosition(int& x, int& y)
	{
		COORD pos = {x, y};
		m_Position = pos;
	}
	bool IsDead()				{ return m_IsDead;					}
	COLOR GetSymbolTXTColor()	{ return m_SymbolTXTColor;			}
	void SetInfoBGColor(COLOR value){	 m_InfoBGColor = value;		}
	void SetInfoTXTColor(COLOR value){	 m_InfoTXTColor = value;	}

	void SetItemBook(ITEM** book) {		 m_ppItemBook = book;		}
};

#endif