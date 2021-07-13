#pragma once
#include <memory>
#include "AIBase.h"

class Enemy;

class PodAI :
	public AIBase
{
public:
	PodAI(Enemy& enemy);
	~PodAI();
	// 更新関数
	void Update(std::list<std::shared_ptr<Enemy>>& enemies);

	// 初期行動
	bool Walk(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾌﾟﾚｲﾔｰとのZ軸を合わせる
	bool Z_Arrangement(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾌﾟﾚｲﾔｰとのZ軸があったら攻撃
	bool Attack(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾀﾞﾒｰｼﾞを食らったときの行動
	bool OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾌﾟﾚｲﾔｰの背後に回り込む
	bool Run(std::list<std::shared_ptr<Enemy>>& enemies);
	// 死ぬ
	bool Death(std::list<std::shared_ptr<Enemy>>& enemies);

private:
	// 初期化
	void Initialize(void);
	// 状態遷移関数ﾎﾟｲﾝﾀ
	bool(PodAI::* updater_)(std::list<std::shared_ptr<Enemy>>&);

};

