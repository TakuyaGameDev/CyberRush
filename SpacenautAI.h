#pragma once
#include "AIBase.h"

class Enemy;

class SpacenautAI :
	public AIBase
{
public:
	SpacenautAI(Enemy& enemy);
	~SpacenautAI();
	// 更新
	void Update(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾌﾟﾚｲﾔｰの位置情報をｻｰﾁする
	bool Search(std::list<std::shared_ptr<Enemy>>& enemies);
	// 初期行動
	bool Walk(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾌﾟﾚｲﾔｰのZ軸に自分のZ軸を合わせる
	bool Z_Arrangement(std::list<std::shared_ptr<Enemy>>& enemies);
	// Z軸がﾌﾟﾚｲﾔｰとあったら攻撃をする(弾を出す)
	bool Attack(std::list<std::shared_ptr<Enemy>>& enemies);
	// ﾀﾞﾒｰｼﾞ食らい処理
	bool OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies);
	// 死ぬ処理
	bool Death(std::list<std::shared_ptr<Enemy>>& enemies);

private:
	// 初期化
	void Initialize(void);
	// 状態関数ﾎﾟｲﾝﾀ
	bool(SpacenautAI::* updater_)(std::list<std::shared_ptr<Enemy>>&);

	int frame;
	// ﾀﾞﾒｰｼﾞを食らった時のﾌﾚｰﾑｶｳﾝﾄ(自身を赤く点滅させるために使用)
	int damage_anim_frame;
	// ﾌﾟﾚｲﾔｰのZ軸ﾎﾟｼﾞｼｮﾝ
	int target_pos_z;
	// 共に戦うﾎﾞｽのﾎﾟｼﾞｼｮﾝ
	Vector2I partnerPos_;
	// 右に動くﾌﾗｸﾞ
	bool moveRight_;
	// 左に動くﾌﾗｸﾞ
	bool moveLeft_;
	// ｱﾆﾒｰｼｮﾝ再生可能ﾌﾗｸﾞ
	bool damage_anim_flag;
};
