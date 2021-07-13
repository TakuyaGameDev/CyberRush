#pragma once
#include <vector>
#include "AIBase.h"

#include "Geometory.h"

class BigboyAI :
	public AIBase
{
public:
	BigboyAI(Enemy& enemy);
	~BigboyAI();
	// 更新
	void Update(std::list<std::shared_ptr<Enemy>>& enemies);
private:
	// 初期化
	void Initialize(void);
	// 関数ﾎﾟｲﾝﾀ
	bool(BigboyAI::* updater_)(std::list<std::shared_ptr<Enemy>>& enemies);

	// 一定時間止まってﾌﾟﾚｲﾔｰの行動を観察する。
	// ここでBigboyの行動を決定する。
	bool Observe(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾌﾟﾚｲﾔｰに向かって歩く
	bool Walk(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾌﾟﾚｲﾔｰとのZ軸を合わせる
	bool ZArrange(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾌﾚｰﾑ単位で攻撃する
	bool Attack(std::list<std::shared_ptr<Enemy>>& enemies);
	// 一定時間経ったら高くｼﾞｬﾝﾌﾟする
	bool Jump(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾀﾞﾒｰｼﾞ
	bool OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies);
	// 死ぬ
	bool Death(std::list<std::shared_ptr<Enemy>>& enemies);
	// タックル
	bool LaserAttack(std::list<std::shared_ptr<Enemy>>& enemies);
	// ｼﾞｬﾝﾌﾟｽﾋﾟｰﾄﾞ
	float jumpSp_;
	// ｼﾞｬﾝﾌﾟ力
	float jumpForce_;
	// ｼﾞｬﾝﾌﾟの初速度
	float jumpFirstSp_;
	//// ﾌﾟﾚｲﾔｰに向かって体当たりする加速度
	//float tackleAccelSp_;
	//// 体当たりするｽﾋﾟｰﾄﾞ
	//float tackleSp_;
	//// 体当たりの初速度
	//float tackleFirstSp_;
	// 奥～手前 手前～奥 に歩くｽﾋﾟｰﾄﾞ
	int walkFrame_;
	// ｼﾞｬﾝﾌﾟしているかのﾌﾗｸﾞ
	bool isJumpAction_;
	// ｼﾞｬﾝﾌﾟをする間隔
	int jumpInterval_;
};

