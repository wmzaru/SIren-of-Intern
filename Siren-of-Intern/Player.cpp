#include "Player.h"

// �R���X�g���N�^
CPlayer::CPlayer(int&& hp, int&& hungry, int&& exp, int&& str, int&& def, int&& move, int&& act)
{
	m_InitHP		= hp;
	m_InitHungry	= hungry;
	m_InitStr		= str;
	m_InitDef		= def;
	m_InitEXP		= exp;
	m_MoveTime		= move;
	m_ACLimit		= act;
	m_ppItemBook	= nullptr;
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{

}

// �֐����FViewInfo			���̕\��
// ���@���FconsoleManager	�R���\�[���}�l�[�W���|�C���^�ւ̃A�h���X
// �߂�l�F
void CPlayer::ViewInfo(COORD& pos, CConsoleManager** consoleManager) const
{
	char buffer[256];
	std::string str = m_Name;
	std::string tmp = "";
	// ����
	int maxDigit;
	int nowDigit;
	std::string space = "";

	maxDigit = 3;
	nowDigit = DIGITCOUNT(m_Level);
	::_itoa_s(m_Level, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += " LEVEL:" + SPACING(maxDigit - nowDigit) + tmp;

	maxDigit = DIGITCOUNT(m_MaxHP);
	nowDigit = DIGITCOUNT(m_NowHP);
	::_itoa_s(m_NowHP, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += " HP:" + SPACING(maxDigit - nowDigit) + tmp;

	::_itoa_s(m_MaxHP, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += "/" + tmp;

	maxDigit = DIGITCOUNT(m_HungryLimit);
	nowDigit = DIGITCOUNT(m_HungryGauge);
	::_itoa_s(m_HungryGauge, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += " Hungry:" + SPACING(maxDigit - nowDigit) + tmp;

	::_itoa_s(m_HungryLimit, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += "/" + tmp;

	::_itoa_s(m_AttackPower, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += " (ATK/DEF):(" + tmp;

	::_itoa_s(m_DefensePower, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += "/" + tmp;
	str += ")";
/*
	maxDigit = DIGITCOUNT(m_ACLimit);
	nowDigit = DIGITCOUNT(m_ActionCount);
	::_itoa_s(m_ActionCount, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += " AC:" + SPACING(maxDigit - nowDigit) + tmp;

	::_itoa_s(m_ACLimit, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += "/" + tmp;

	::_itoa_s(m_MoveTime, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += " MoveTime:" + tmp;

	maxDigit = 3;
	nowDigit = DIGITCOUNT(m_Position.X);
	::_itoa_s(m_Position.X + 1, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += " Pos:(" + SPACING(maxDigit - nowDigit) + tmp;

	maxDigit = 2;
	nowDigit = DIGITCOUNT(m_Position.Y);
	::_itoa_s(m_Position.Y + 1, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += "," + SPACING(maxDigit - nowDigit) + tmp;
	str += ")";
*/

	(*consoleManager)->WriteTextCharacter(m_InfoBGColor, m_InfoTXTColor, pos, str, NOWAIT, DONOTDELAY );
}

// �֐����FInitialize		������
// ���@���F
// �߂�l�F
void CPlayer::Initialize()
{
	// ��������������������������������������������
	// �ő�l��ύX����d�l�̎����ɂ��킹�Ē�������K�v����
	// ��������������������������������������������
	m_Name				= "SIREN";
	m_Symbol			= "P";
	m_SymbolTXTColor	= H_RED;
	m_InfoBGColor		= L_RED;
	m_InfoTXTColor		= H_YELLOW;
	ResetParameter();
}

// �֐����FResetParameter		�p�����[�^���Z�b�g
// ���@���F
// �߂�l�F
void CPlayer::ResetParameter()
{
	m_Level				= 1;
	m_NowHP				= m_MaxHP		= m_InitHP;
	m_HungryGauge		= m_HungryLimit = m_InitHungry;
	m_NowEXP			= 0;
	m_MaxEXP			= m_InitEXP;
	m_Strength			= m_InitStr;
	m_Toughness			= m_InitDef;
	m_ActionCount		= m_ACLimit;
	m_MoveTime			= 1;
	m_NRSpeed			= 5;
	m_RRSpeed			= 2;
	m_IsDead			= false;
	// �����A�C�e���̃��Z�b�g
	m_HaveItem.clear();
	// �g�p�A�C�e���ԍ��̃��Z�b�g
	m_UseItemIndex		= UNSPECIFIED;

	// �����A�C�e���ԍ��̃��Z�b�g
	m_WeaponIndex		= UNSPECIFIED;
	m_ShieldIndex		= UNSPECIFIED;
	m_ArmorIndex		= UNSPECIFIED;
	Refresh(ATTACKPOWER);
	Refresh(DEFENSEPOWER);
}

// �֐����FFoot				������(�ҋ@)
// ���@���F
// �߂�l�F
void CPlayer::Foot(bool isRapid)
{
	if (m_HungryGauge == 0)
	{
		// �󕠃_���[�W
		Damage(1);
	}
	else
	{
		// �s���J�E���^�f�N�������g
		if (--m_ActionCount == 0)
		{
			--m_HungryGauge;
			m_ActionCount = m_ACLimit;
		}

		// �̗͂̉�
		if (m_NowHP < m_MaxHP)
		{
			// �C�Ӊ񕜂����R�񕜂̔���
			int increments;
			if (isRapid == true)
			{
				increments = m_RRSpeed;
			}
			else
			{
				increments = m_NRSpeed;
			}

			// �̗͂̉񔻒�
			if (m_ActionCount % increments == 0)
			{
				++m_NowHP;
			}
		}
	}
}

// �֐����FMove				�ړ�
// ���@���F
// �߂�l�F
void CPlayer::Move(COORD& pos)
{
	m_Position = pos;
	Foot(false);
}

// �֐����FReceiveAttack	�U�����󂯂�
// ���@���FenemyStr			����̍U����
// �߂�l�F
int CPlayer::ReceiveAttack(int& enemyStr)
{
	int impact = enemyStr - m_DefensePower;
	if (impact > 0)
	{
		Damage(static_cast<int&&>(impact));
		return impact;
	}
	return 0;
}

// �֐����FHeal			�̗͂̉�
// ���@���FhealValue	�񕜗�
// �߂�l�F���ۂɉ񕜂�����
int CPlayer::Heal(int& healValue)
{
	// �ő�l�̑��������邩
	if (m_NowHP == m_MaxHP)
	{
		//m_MaxHP += healValue / 10;
		m_MaxHP += 1;
	}

	// �̗͂̉�
	m_NowHP += healValue;
	if (m_NowHP > m_MaxHP)
	{
		int div = m_NowHP - m_MaxHP;
		m_NowHP = m_MaxHP;
		return healValue - div;
	}
	return healValue;
}

// �֐����FEatFood		�����x�̉�
// ���@���FhealValue	�񕜗�
// �߂�l�F���ۂɉ񕜂�����
int CPlayer::EatFood(int& healValue)
{
	// �ő�l�̑��������邩
	if (m_HungryGauge == m_HungryLimit)
	{
		m_HungryLimit += healValue / 10;
		//m_HungryLimit += 1;
	}

	m_HungryGauge += healValue;
	if (m_HungryGauge > m_HungryLimit)
	{
		int div = m_HungryGauge - m_HungryLimit;
		m_HungryGauge = m_HungryLimit;
		return healValue - div;
	}
	return healValue;
}

// �֐����FDamage			��_���[�W
// ���@���FdamageValue		�_���[�W��
// �߂�l�F
void CPlayer::Damage(int&& damageValue)
{
	m_NowHP -= damageValue;
	if (m_NowHP <= 0)
	{
		m_NowHP = 0;
	}
	DeadorAlive();
}

// �֐����FDeadorAlive		���S����
// ���@���F
// �߂�l�F					�����E���s
void CPlayer::DeadorAlive()
{
	if (m_NowHP == 0)
	{
		m_IsDead = true;
	}
}

// �֐����FGetEXP		�o���l�̎擾
// ���@���F
// �߂�l�F
void CPlayer::GetEXP(int value, std::string _str, CConsoleManager** consoleManager)
{
	m_NowEXP += value;
	// �o���l�擾���O�̏o��
	char buffer[256];
	std::string str = m_Name + "��";
	std::string tmp = "";

	::_itoa_s(value, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += tmp + "�̌o���l�𓾂��B";
	(*consoleManager)->AddLog(str);

	// ���x���A�b�v����
	if (m_NowEXP >= m_MaxEXP)
	{
		int hpInc = 0;
		int strInc = 0;
		int defInc = 0;
		int seed = 0;
		while (m_NowEXP >= m_MaxEXP)
		{
			m_Level++;
			m_NowEXP -= m_MaxEXP;
			m_MaxEXP += m_Level * (5 + m_Level);

			// HP�̑���
			hpInc += rand() % 3 + 3;

			// STR�̑�������
			seed = rand() % 30; // 3.33%
			if (seed == 0)
			{
				strInc++;
			}

			// DEF�̑�������
			seed = rand() % 100; // 1%
			if (seed == 0)
			{
				defInc++;
			}
		}
		// �X�e�[�^�X�A�b�v�ƃ��O�o��
		tmp = "";
		str = "���x���A�b�v�IPLAYER�̃��x����";
		::_itoa_s(m_Level, buffer, 256 * sizeof(char), 10);
		tmp = buffer;
		str += tmp + "�ɂȂ����B";

		m_MaxHP += hpInc;
		::_itoa_s(hpInc, buffer, 256 * sizeof(char), 10);
		tmp = buffer;
		str += "�ő�HP��" + tmp + "���������B";
		(*consoleManager)->AddLog(str);

		if (strInc != 0)
		{
			m_Strength += strInc;
			str = "�͂������Ȃ����C�������B";
			(*consoleManager)->AddLog(str);
			// STR�������������̂ōU���͂��X�V
			Refresh(ATTACKPOWER);
		}

		if (defInc != 0)
		{
			m_Toughness += defInc;
			str = "�挒�ɂȂ����C�������B";
			(*consoleManager)->AddLog(str);
			// TOUGHNESS�������������̂Ŗh��͂��X�V
			Refresh(DEFENSEPOWER);
		}
	}
}

// �֐����FEquippedWeapon	����̑���
// ���@���F
// �߂�l�F
void CPlayer::EquippedWeapon(int index)
{
	m_WeaponIndex = index;
	Refresh(ATTACKPOWER);
}

// �֐����FEquippedShield	���̑���
// ���@���F
// �߂�l�F
void CPlayer::EquippedShield(int index)
{
	m_ShieldIndex = index;
	Refresh(DEFENSEPOWER);
}

// �֐����FEquippedArmor	�h��̑���
// ���@���F
// �߂�l�F
void CPlayer::EquippedArmor(int index)
{
	m_ArmorIndex = index;
	Refresh(DEFENSEPOWER);
}

// �֐����FUndressedWeapon	������O��
// ���@���F
// �߂�l�F
void CPlayer::UndressedWeapon()
{
	m_WeaponIndex = UNSPECIFIED;
	Refresh(ATTACKPOWER);
}

// �֐����FUndressedShield	�����O��
// ���@���F
// �߂�l�F
void CPlayer::UndressedShield()
{
	m_ShieldIndex = UNSPECIFIED;
	Refresh(DEFENSEPOWER);
}

// �֐����FUndressedArmor	�h����O��
// ���@���F
// �߂�l�F
void CPlayer::UndressedArmor()
{
	m_ShieldIndex = UNSPECIFIED;
	Refresh(DEFENSEPOWER);
}

// �֐����FRefresh			��Ԃ̍X�V
// ���@���Ftype				�A�C�e���̎��
// �߂�l�F
void CPlayer::Refresh(PARAMETER type)
{
	switch (type)
	{
	case ATTACKPOWER:
		if (m_WeaponIndex == UNSPECIFIED)
		{
			// �U���݂͂͋ؗ͂̂Ōv�Z
			m_AttackPower = m_Strength;
		}
		else
		{
			// �U���͂͋ؗ�+����U����
			m_AttackPower = m_Strength + (*m_ppItemBook)[m_WeaponIndex].value;
		}
		break;
	case DEFENSEPOWER:
		if ((m_ShieldIndex == UNSPECIFIED)
		 && (m_ArmorIndex == UNSPECIFIED))
		{
			// �h��͂͊挒���̂�
			m_DefensePower = m_Toughness;
		}
		else if (m_ArmorIndex == UNSPECIFIED)
		{
			// �h��͂͊挒+�h��̖h���
			m_DefensePower = m_Toughness + (*m_ppItemBook)[m_ShieldIndex].value;
		}
		else if (m_ShieldIndex == UNSPECIFIED)
		{
			// �h��͂͊挒+���̖h���
			m_DefensePower = m_Toughness + (*m_ppItemBook)[m_ArmorIndex].value;
		}
		else
		{
			// �h��͂͊挒+�h��̖h���
			m_DefensePower = m_Toughness + (*m_ppItemBook)[m_ShieldIndex].value + (*m_ppItemBook)[m_ArmorIndex].value;
		}
		break;
	}
}
