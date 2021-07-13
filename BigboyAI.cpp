#include <EffekseerForDXLib.h>

#include "BigboyAI.h"
#include "Enemy.h"
#include "ControlledPlayer.h"
#include "Floor.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "Collision.h"

BigboyAI::BigboyAI(Enemy& enemy):
	AIBase(enemy)
{
	// 初期化
	Initialize();
}

BigboyAI::~BigboyAI()
{
}

void BigboyAI::Update(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾀﾞﾒｰｼﾞを食らったら
	if (me_.GetOnDamaged())
	{
		// hitｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("hit");
		// 自身の状態をﾀﾞﾒｰｼﾞを食らった状態にする
		updater_ = &BigboyAI::OnDamaged;
	}
	// 味方からの攻撃を食らった場合
	if (me_.GetFriendlyFireFlag())
	{
		// hitｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("hit");
		// 自身の状態をﾀﾞﾒｰｼﾞを食らった状態にする
		updater_ = &BigboyAI::OnDamaged;
	}
	// ｼﾞｬﾝﾌﾟしていないと
	if (!isJumpAction_)
	{
		// ｼﾞｬﾝﾌﾟをする間隔を加算していく
		jumpInterval_++;
		// ｼﾞｬﾝﾌﾟをする間隔ｶｳﾝﾄが一定以上になったら
		if (jumpInterval_ >= 2000)
		{
			// ｼﾞｬﾝﾌﾟ間隔ｶｳﾝﾄを0に
			jumpInterval_ = 0;
			// ｼﾞｬﾝﾌﾟをしているﾌﾗｸﾞをtrueにする
			isJumpAction_ = true;
		}
	}
	// 自身の状態毎の更新
	(this->*updater_)(enemies);
}

void BigboyAI::Initialize(void)
{
	// 初期状態にObserve(観察=ﾌﾟﾚｲﾔｰの動向を見ている)状態に設定
	updater_ = &BigboyAI::Observe;
	// 歩きｶｳﾝﾀｰの初期化
	walkFrame_ = 0;
	// ｼﾞｬﾝﾌﾟしているﾌﾗｸﾞの初期化
	isJumpAction_ = false;
	// ｼﾞｬﾝﾌﾟｽﾋﾟｰﾄﾞの初期化
	jumpSp_ = 0;
	// ｼﾞｬﾝﾌﾟ初速度の初期化
	// 結構高くｼﾞｬﾝﾌﾟさせたいので-45.0fと設定
	jumpFirstSp_ = -45.0f;
	// ｼﾞｬﾝﾌﾟ力の初期化
	jumpForce_ = 0.0f;
	// ｼﾞｬﾝﾌﾟ間隔ｶｳﾝﾄの初期化
	jumpInterval_ = 0;
}

bool BigboyAI::Observe(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾌﾟﾚｲﾔｰのZ軸ﾎﾟｼﾞｼｮﾝ
	auto playerZ = me_.SearchNearestPlayer()->GetZPos();
	// ﾌﾟﾚｲﾔｰのx,y軸ﾎﾟｼﾞｼｮﾝ
	auto playerPos = me_.GetNearestPlayer()->GetPos();
	// 自身とﾌﾟﾚｲﾔｰのZ軸の差
	auto zdiff = playerZ - me_.GetZPos();
	// 自身とﾌﾟﾚｲﾔｰとのx,y軸の距離(当たり判定に使用)
	auto distance = playerPos - me_.GetPos();
	// ﾌﾟﾚｲﾔｰが自身よりも左にいる場合
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// 自身のx軸のｽﾋﾟｰﾄﾞを設定(左向きになるようにﾏｲﾅｽ値)
		me_.GetSpeed().x = -7;
		// 左向きにする
		me_.GetisTurnFlag() = false;
	}
	// ﾌﾟﾚｲﾔｰが自身よりも右にいる場合
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// 自身のx軸のｽﾋﾟｰﾄﾞを設定(右向きになるようにﾌﾟﾗｽ値)
		me_.GetSpeed().x = 7;
		// 右向きにする
		me_.GetisTurnFlag() = true;
	}
	// ｼﾞｬﾝﾌﾟｱｸｼｮﾝでない時
	if (!isJumpAction_)
	{
		// 自身とﾌﾟﾚｲﾔｰの距離が一定以上だと
		if (abs(distance.x) >= 170)
		{
			// 自身のｼﾞｬﾝﾌﾟしているﾌﾗｸﾞをtrueに
			me_.IsJumping() = true;
			// ｼﾞｬﾝﾌﾟｱｸｼｮﾝをしているﾌﾗｸﾞをtrueに
			isJumpAction_ = true;
			// 自身の状態をJump状態にする
			updater_ = &BigboyAI::Jump;
			return true;
		}
	}
	else
	{
		// walkｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("walk");
		// 自身の状態をZ軸を合わせる行動の状態に変更
		updater_ = &BigboyAI::ZArrange;
	}

	return true;

}

bool BigboyAI::Walk(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// 歩いているｶｳﾝﾄを加算
	walkFrame_++;
	// ﾌﾟﾚｲﾔｰが自身よりも左にいる場合
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// 左向きにする
		me_.GetisTurnFlag() = false;
		// x軸ｽﾋﾟｰﾄﾞを左向きｽﾋﾟｰﾄﾞに
		me_.GetSpeed().x = -1;
	}
	// ﾌﾟﾚｲﾔｰが自身よりも右にいる場合
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// 右向きにする
		me_.GetisTurnFlag() = true;
		// x軸ｽﾋﾟｰﾄﾞを右向きｽﾋﾟｰﾄﾞに
		me_.GetSpeed().x = 1;
	}
	// ﾌﾟﾚｲﾔｰのZ軸ﾎﾟｼﾞｼｮﾝ
	auto playerZ = me_.GetNearestPlayer()->GetZPos();
	// ﾌﾟﾚｲﾔｰが自身よりも手前にいる場合
	if (me_.GetZPos() >= playerZ)
	{
		// Z軸ｽﾋﾟｰﾄﾞをﾏｲﾅｽ値に
		me_.GetZSpeed() = -1;
	}
	else
	{
		// Z軸ｽﾋﾟｰﾄﾞをﾌﾟﾗｽ値に
		me_.GetZSpeed() = 1;
	}
	// 歩きｶｳﾝﾀｰが一定以下だと
	if (walkFrame_ <= 100)
	{
		// ﾎﾟｼﾞｼｮﾝ移動
		me_.GetPos().x += me_.GetSpeed().x;
		// Z軸ﾎﾟｼﾞｼｮﾝ移動
		me_.GetZPos() += me_.GetZSpeed();
	}
	// 歩きｶｳﾝﾄが一定以上になると
	else
	{
		// 歩きｶｳﾝﾄを0にする
		walkFrame_ = 0.0f;
		// Idleｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("idle");
		// 自身の状態をObserve状態にする
		updater_ = &BigboyAI::Observe;
	}

	return true;
}

bool BigboyAI::ZArrange(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾌﾟﾚｲﾔｰのZ軸ﾎﾟｼﾞｼｮﾝ
	auto playerZ = me_.GetNearestPlayer()->GetZPos();
	// ﾌﾟﾚｲﾔｰが自身より手前にいる場合
	if (me_.GetZPos() >= playerZ)
	{
		// 手前に移動するためにﾏｲﾅｽ値のｾｯﾄ
		me_.GetZSpeed() = -1;
	}
	else
	{
		// 奥に移動するためにﾌﾟﾗｽ値のｾｯﾄ
		me_.GetZSpeed() = 1;
	}
	// Z軸の移動
	me_.GetZPos() += me_.GetZSpeed();
	// ﾌﾟﾚｲﾔｰのZ軸ﾎﾟｼﾞｼｮﾝと自身のZ軸ﾎﾟｼﾞｼｮﾝの差分計算
	auto zdiff = playerZ - me_.GetZPos();
	// 上記の差分が5以下だと
	// なぜ5? -> Z軸当たり判定に少し余分を持って範囲を広くすることで
	//           きちんと敵の攻撃も当たるようにしている
	if (abs(zdiff) <= 5)
	{
		// Attackｱﾆﾒｰｼｮﾝの変更
		me_.ChangeAnimation("attack");
		// 自身の状態をLaserAttackに変更
		updater_ = &BigboyAI::LaserAttack;
	}

	return false;
}

bool BigboyAI::Attack(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾌﾟﾚｲﾔｰが自身よりも左にいる場合
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// 方向を左向きに
		me_.GetisTurnFlag() = false;
		// 左向きｽﾋﾟｰﾄﾞに設定
		me_.GetSpeed().x = -1;
	}
	// ﾌﾟﾚｲﾔｰが自身よりも右にいる場合
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// 方向を右向きに
		me_.GetisTurnFlag() = true;
		// 右向きｽﾋﾟｰﾄﾞに設定
		me_.GetSpeed().x = 1;
	}
	// ｱﾆﾒｰｼｮﾝ終了時
	if (me_.GetisAnimEnd())
	{
		// Idleｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("idle");
		// 状態をObserve状態に変更
		updater_ = &BigboyAI::Observe;
	}
	return true;
}

bool BigboyAI::Jump(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ｼﾞｬﾝﾌﾟ力を加算
	jumpForce_++;
	// ｼﾞｬﾝﾌﾟ力 = ｼﾞｬﾝﾌﾟ初速度 + ｼﾞｬﾝﾌﾟ力
	// 従って、-45.0fからだんだん0になっていく
	jumpSp_ = jumpFirstSp_ + jumpForce_;
	// 方向に対するx軸移動
	me_.GetPos().x += me_.GetSpeed().x;
	// y軸移動(ｼﾞｬﾝﾌﾟ)
	me_.GetPos().y += jumpSp_;
	// 接地している場合
	if (me_.OnFloor())
	{
		// ｼﾞｬﾝﾌﾟｽﾋﾟｰﾄﾞを0に
		jumpSp_ = 0;
		// ｼﾞｬﾝﾌﾟ初速度を0に
		jumpFirstSp_ = -45.0f;
		// ｼﾞｬﾝﾌﾟ力を0に
		jumpForce_ = 0.0f;
		// 自身のｼﾞｬﾝﾌﾟﾌﾗｸﾞをfalseに
		me_.IsJumping() = false;
		// Idleｱﾆﾒｰｼｮﾝの変更
		me_.ChangeAnimation("idle");
		// 自身の状態をObserve状態にする
		updater_ = &BigboyAI::Observe;
	}
	return false;
}

bool BigboyAI::OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ｱﾆﾒｰｼｮﾝ終了時
	if (me_.GetisAnimEnd())
	{
		// HPが0以下になったら
		if (me_.GetHp() <= 0)
		{
			// deathｱﾆﾒｰｼｮﾝに変更
			me_.ChangeAnimation("death");
			// 自身の状態をDeathにする
			updater_ = &BigboyAI::Death;
			return true;
		}
		else
		{
			// Idleｱﾆﾒｰｼｮﾝに変更
			me_.ChangeAnimation("idle");
			// 自身の状態をObserveに変更
			updater_ = &BigboyAI::Observe;
			return true;
		}
	}
	// 1回ﾀﾞﾒｰｼﾞを食らったらfalseにしないと以降ﾀﾞﾒｰｼﾞが食らわないのでfalseにする
	me_.GetOnDamaged() = false;

	return false;
}

bool BigboyAI::Death(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ｱﾆﾒｰｼｮﾝが終了したら
	if (me_.GetisAnimEnd())
	{
		// 自身を削除
		me_.Delete();
		return true;
	}
	return false;
}

bool BigboyAI::LaserAttack(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ｱﾆﾒｰｼｮﾝ終了時
	if (me_.GetisAnimEnd())
	{
		// Idleｱﾆﾒｰｼｮﾝに変更する
		me_.ChangeAnimation("idle");
		// 自身の状態をObserve状態にする
		updater_ = &BigboyAI::Observe;
	}

	return false;
}
