#pragma once
#include <EffekseerForDXLib.h>
#include <string>
#include <map>
#include <list>

#include "Geometory.h"

// ｴﾌｪｸﾄを管理するｸﾗｽ
// ｼﾝｸﾞﾙﾄﾝｸﾗｽ

#define lpEffect EffectManager::GetInstance()

class EffectManager
{
public:
	static EffectManager& GetInstance(void)
	{
		static EffectManager s_Instance;
		return s_Instance;
	}
	// 更新
	void UpDate(void);
	// ｴﾌｪｸﾄの描画
	void Draw(void);
	// 全てのｴﾌｪｸﾄの再生を止める
	bool StopAll(void);
	// ｴﾌｪｸﾄの再生
	bool Play(std::string effectName, const Vector2I& pos);
	// effekseerの初期化関連
	bool Init(const Vector2I& size);

	// ｴﾌｪｸﾄが再生中華どうかのﾌﾗｸﾞ取得
	const int& IsPlayingEffect(std::string effectName);
	// ｴﾌｪｸﾄのﾛｰﾄﾞ
	void Load(std::string effectName);
private:
	EffectManager() = default;
	// ｺﾋﾟｰ禁止
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;
	// ﾌﾟﾚｲﾊﾝﾄﾞﾙの取得
	const int& GetHandle(std::string effectName);

	// ｴﾌｪｸﾄﾊﾝﾄﾞﾙ
	std::map<std::string, int> handles_;
	// ﾌﾟﾚｲﾊﾝﾄﾞﾙを格納したﾘｽﾄ
	std::list<int> playList_;
};


