#pragma once
#include <list>
#include <memory>

class Enemy;
class AICollider;

class AIBase
{
public:
	AIBase(Enemy& enemy);
	~AIBase();
	// 各敵のAI行動一括実行
	virtual void AIAction(std::list<std::shared_ptr<Enemy>>& enemies);
	// 各敵のAI思考ﾙｰﾁﾝの更新
	virtual void Update(std::list<std::shared_ptr<Enemy>>& enemies) = 0;
	// 初期化
	virtual void Initialize(void) = 0;
	// 各ｺﾗｲﾀﾞｰの当たり判定(自分自身と自分以外の味方で比較)
	bool IsHitFriends(std::list<std::shared_ptr<Enemy>>& enemies);
protected:
	// 自分自身
	Enemy& me_;
	// 1回銃弾をｲﾝｽﾀﾝｽしたらtrueに
	bool createBulletFlag_;
};

