#include "Player.h"

// コンストラクタ
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

// デストラクタ
CPlayer::~CPlayer()
{

}

// 関数名：ViewInfo			情報の表示
// 引　数：consoleManager	コンソールマネージャポインタへのアドレス
// 戻り値：
void CPlayer::ViewInfo(COORD& pos, CConsoleManager** consoleManager) const
{
	char buffer[256];
	std::string str = m_Name;
	std::string tmp = "";
	// 桁数
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

// 関数名：Initialize		初期化
// 引　数：
// 戻り値：
void CPlayer::Initialize()
{
	// ◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	// 最大値を変更する仕様の実装にあわせて調整する必要あり
	// ◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	m_Name				= "SIREN";
	m_Symbol			= "P";
	m_SymbolTXTColor	= H_RED;
	m_InfoBGColor		= L_RED;
	m_InfoTXTColor		= H_YELLOW;
	ResetParameter();
}

// 関数名：ResetParameter		パラメータリセット
// 引　数：
// 戻り値：
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
	// 所持アイテムのリセット
	m_HaveItem.clear();
	// 使用アイテム番号のリセット
	m_UseItemIndex		= UNSPECIFIED;

	// 装備アイテム番号のリセット
	m_WeaponIndex		= UNSPECIFIED;
	m_ShieldIndex		= UNSPECIFIED;
	m_ArmorIndex		= UNSPECIFIED;
	Refresh(ATTACKPOWER);
	Refresh(DEFENSEPOWER);
}

// 関数名：Foot				足踏み(待機)
// 引　数：
// 戻り値：
void CPlayer::Foot(bool isRapid)
{
	if (m_HungryGauge == 0)
	{
		// 空腹ダメージ
		Damage(1);
	}
	else
	{
		// 行動カウンタデクリメント
		if (--m_ActionCount == 0)
		{
			--m_HungryGauge;
			m_ActionCount = m_ACLimit;
		}

		// 体力の回復
		if (m_NowHP < m_MaxHP)
		{
			// 任意回復か自然回復の判定
			int increments;
			if (isRapid == true)
			{
				increments = m_RRSpeed;
			}
			else
			{
				increments = m_NRSpeed;
			}

			// 体力の回判定
			if (m_ActionCount % increments == 0)
			{
				++m_NowHP;
			}
		}
	}
}

// 関数名：Move				移動
// 引　数：
// 戻り値：
void CPlayer::Move(COORD& pos)
{
	m_Position = pos;
	Foot(false);
}

// 関数名：ReceiveAttack	攻撃を受ける
// 引　数：enemyStr			相手の攻撃力
// 戻り値：
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

// 関数名：Heal			体力の回復
// 引　数：healValue	回復量
// 戻り値：実際に回復した量
int CPlayer::Heal(int& healValue)
{
	// 最大値の増加があるか
	if (m_NowHP == m_MaxHP)
	{
		//m_MaxHP += healValue / 10;
		m_MaxHP += 1;
	}

	// 体力の回復
	m_NowHP += healValue;
	if (m_NowHP > m_MaxHP)
	{
		int div = m_NowHP - m_MaxHP;
		m_NowHP = m_MaxHP;
		return healValue - div;
	}
	return healValue;
}

// 関数名：EatFood		満腹度の回復
// 引　数：healValue	回復量
// 戻り値：実際に回復した量
int CPlayer::EatFood(int& healValue)
{
	// 最大値の増加があるか
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

// 関数名：Damage			被ダメージ
// 引　数：damageValue		ダメージ量
// 戻り値：
void CPlayer::Damage(int&& damageValue)
{
	m_NowHP -= damageValue;
	if (m_NowHP <= 0)
	{
		m_NowHP = 0;
	}
	DeadorAlive();
}

// 関数名：DeadorAlive		死亡判定
// 引　数：
// 戻り値：					成功・失敗
void CPlayer::DeadorAlive()
{
	if (m_NowHP == 0)
	{
		m_IsDead = true;
	}
}

// 関数名：GetEXP		経験値の取得
// 引　数：
// 戻り値：
void CPlayer::GetEXP(int value, std::string _str, CConsoleManager** consoleManager)
{
	m_NowEXP += value;
	// 経験値取得ログの出力
	char buffer[256];
	std::string str = m_Name + "は";
	std::string tmp = "";

	::_itoa_s(value, buffer,256 * sizeof(char),10);
	tmp = buffer;
	str += tmp + "の経験値を得た。";
	(*consoleManager)->AddLog(str);

	// レベルアップ判定
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

			// HPの増加
			hpInc += rand() % 3 + 3;

			// STRの増加判定
			seed = rand() % 30; // 3.33%
			if (seed == 0)
			{
				strInc++;
			}

			// DEFの増加判定
			seed = rand() % 100; // 1%
			if (seed == 0)
			{
				defInc++;
			}
		}
		// ステータスアップとログ出力
		tmp = "";
		str = "レベルアップ！PLAYERのレベルが";
		::_itoa_s(m_Level, buffer, 256 * sizeof(char), 10);
		tmp = buffer;
		str += tmp + "になった。";

		m_MaxHP += hpInc;
		::_itoa_s(hpInc, buffer, 256 * sizeof(char), 10);
		tmp = buffer;
		str += "最大HPが" + tmp + "あがった。";
		(*consoleManager)->AddLog(str);

		if (strInc != 0)
		{
			m_Strength += strInc;
			str = "力が強くなった気がした。";
			(*consoleManager)->AddLog(str);
			// STR増加があったので攻撃力を更新
			Refresh(ATTACKPOWER);
		}

		if (defInc != 0)
		{
			m_Toughness += defInc;
			str = "頑健になった気がした。";
			(*consoleManager)->AddLog(str);
			// TOUGHNESS増加があったので防御力を更新
			Refresh(DEFENSEPOWER);
		}
	}
}

// 関数名：EquippedWeapon	武器の装備
// 引　数：
// 戻り値：
void CPlayer::EquippedWeapon(int index)
{
	m_WeaponIndex = index;
	Refresh(ATTACKPOWER);
}

// 関数名：EquippedShield	盾の装備
// 引　数：
// 戻り値：
void CPlayer::EquippedShield(int index)
{
	m_ShieldIndex = index;
	Refresh(DEFENSEPOWER);
}

// 関数名：EquippedArmor	防具の装備
// 引　数：
// 戻り値：
void CPlayer::EquippedArmor(int index)
{
	m_ArmorIndex = index;
	Refresh(DEFENSEPOWER);
}

// 関数名：UndressedWeapon	武器を外す
// 引　数：
// 戻り値：
void CPlayer::UndressedWeapon()
{
	m_WeaponIndex = UNSPECIFIED;
	Refresh(ATTACKPOWER);
}

// 関数名：UndressedShield	盾を外す
// 引　数：
// 戻り値：
void CPlayer::UndressedShield()
{
	m_ShieldIndex = UNSPECIFIED;
	Refresh(DEFENSEPOWER);
}

// 関数名：UndressedArmor	防具を外す
// 引　数：
// 戻り値：
void CPlayer::UndressedArmor()
{
	m_ShieldIndex = UNSPECIFIED;
	Refresh(DEFENSEPOWER);
}

// 関数名：Refresh			状態の更新
// 引　数：type				アイテムの種類
// 戻り値：
void CPlayer::Refresh(PARAMETER type)
{
	switch (type)
	{
	case ATTACKPOWER:
		if (m_WeaponIndex == UNSPECIFIED)
		{
			// 攻撃力は筋力のみで計算
			m_AttackPower = m_Strength;
		}
		else
		{
			// 攻撃力は筋力+武器攻撃力
			m_AttackPower = m_Strength + (*m_ppItemBook)[m_WeaponIndex].value;
		}
		break;
	case DEFENSEPOWER:
		if ((m_ShieldIndex == UNSPECIFIED)
		 && (m_ArmorIndex == UNSPECIFIED))
		{
			// 防御力は頑健さのみ
			m_DefensePower = m_Toughness;
		}
		else if (m_ArmorIndex == UNSPECIFIED)
		{
			// 防御力は頑健+防具の防御力
			m_DefensePower = m_Toughness + (*m_ppItemBook)[m_ShieldIndex].value;
		}
		else if (m_ShieldIndex == UNSPECIFIED)
		{
			// 防御力は頑健+盾の防御力
			m_DefensePower = m_Toughness + (*m_ppItemBook)[m_ArmorIndex].value;
		}
		else
		{
			// 防御力は頑健+防具の防御力
			m_DefensePower = m_Toughness + (*m_ppItemBook)[m_ShieldIndex].value + (*m_ppItemBook)[m_ArmorIndex].value;
		}
		break;
	}
}
