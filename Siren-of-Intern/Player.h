#ifndef __PLAYER_H__
#define __PLAYER_H__
#pragma once

// ---- Include File ----
#include <Windows.h>
#include <map>
#include <iterator>
#include "GlobalInfo.h"
#include "ConsoleManager.h"

// ---- クラス宣言 ----
class CPlayer
{
private:
	std::string	m_Name;				// 名称
	std::string	m_Symbol;			// 表示記号
	int			m_NowHP;			// 現在体力値
	int			m_MaxHP;			// 最大体力値
	int			m_InitHP;			// HP初期値
	int			m_HungryGauge;		// 現在満腹度
	int			m_HungryLimit;		// 最大満腹度
	int			m_InitHungry;		// 満腹度初期値
	int			m_Level;			// レベル
	int			m_NowEXP;			// 現在の経験値
	int			m_MaxEXP;			// 経験値の上限
	int			m_InitEXP;			// 経験値初期値
	int			m_Strength;			// 筋力
	int			m_InitStr;			// 筋力初期値
	int			m_Toughness;		// 頑健さ
	int			m_InitDef;			// 頑健さ所期値
	int			m_ActionCount;		// 行動カウンター
	int			m_ACLimit;			// カウンター限界値
	int			m_NRSpeed;			// 自然回復速度(行動カウント値刻み数) (Natural Recovery)
	int			m_RRSpeed;			// 急速回復速度(行動カウント値刻み数)
	int			m_MoveTime;			// 行動可能回数
	COORD		m_Position;			// 現在位置
	ItemList	m_HaveItem;			// 所持アイテム
	int			m_UseItemIndex;		// 使用するアイテムの番号
	bool		m_IsDead;			// 死亡フラグ
	COLOR		m_SymbolTXTColor;	// シンボル表示文字色
	COLOR		m_InfoBGColor;		// インフォ表示背景色
	COLOR		m_InfoTXTColor;		// インフォ表示文字色

	// 武具
	int			m_WeaponIndex;		// 装備中の武器の図鑑番号
	int			m_ShieldIndex;		// 装備中の盾の図鑑番号
	int			m_ArmorIndex;		// 装備中の鎧の図鑑番号
	int			m_AttackPower;		// 総合攻撃力
	int			m_DefensePower;		// 総合防御力
	ITEM**		m_ppItemBook;		// アイテム図鑑ポインタを指すアドレス

public:
	CPlayer(int&&, int&&, int&&, int&&, int&&, int&&, int&&);
	~CPlayer();

	// デバッグ用
	void ViewInfo(COORD&, CConsoleManager**) const;	// 情報表示

	void Initialize();			// 初期化
	void ResetParameter();		// データのリセット
	void Foot(bool);			// 足踏み (1ターン待機)
	void Move(COORD&);			// 移動
	int ReceiveAttack(int&);	// 攻撃を受ける
	int Heal(int&);				// 体力回復
	int EatFood(int&);			// 満腹度回復
	void Damage(int&&);			// 被ダメージ
	void DeadorAlive();			// 死亡判定
	void GetEXP(int, std::string, CConsoleManager**); // 経験値の取得

	void EquippedWeapon(int);	// 武器の装備
	void EquippedShield(int);	// 盾の装備
	void EquippedArmor(int);	// 防具の装備
	void UndressedWeapon();		// 武器を外す
	void UndressedShield();		// 盾を外す
	void UndressedArmor();		// 防具を外す
	void Refresh(PARAMETER);	// 戦闘力の更新

	// 一覧の参照
	ItemList GetItem()			{ return m_HaveItem;				}
	// アイテムの追加
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