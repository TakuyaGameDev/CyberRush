#include "PodAI.h"
#include "Enemy.h"
#include "ControlledPlayer.h"
#include "WeaponBase.h"

PodAI::PodAI(Enemy& enemy):
	AIBase(enemy)
{
	// 初期化
	Initialize();
}

PodAI::~PodAI()
{
}

void PodAI::Update(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾀﾞﾒｰｼﾞを食らっていたら
	if (me_.GetOnDamaged())
	{
		// hitｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("hit");
		// 状態をﾀﾞﾒｰｼﾞ食らい状態に変更
		updater_ = &PodAI::OnDamaged;
	}
	// 味方の攻撃を食らったら
	if (me_.GetFriendlyFireFlag())
	{
		// 即座に死ぬ
		me_.ChangeAnimation("death");
		// 状態をdeathに変更
		updater_ = &PodAI::Death;
	}
	// 状態の更新
	(this->*updater_)(enemies);
}

bool PodAI::Walk(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// 自分がﾌﾟﾚｲﾔｰよりも右にいたら
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// 反転ﾌﾗｸﾞをtrueに
		// DrawRotaGraphの第7引数のX反転ﾌﾗｸﾞと紐づけている
		me_.GetisTurnFlag() = true;
		// 移動ｽﾋﾟｰﾄﾞを歩く際のｽﾋﾟｰﾄﾞに設定
		me_.GetSpeed().x = -2;
	}
	// 自分がﾌﾟﾚｲﾔｰよりも左にいたら
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// 反転ﾌﾗｸﾞをtrueに
		// DrawRotaGraphの第7引数のX反転ﾌﾗｸﾞと紐づけている
		me_.GetisTurnFlag() = false;
		// 移動ｽﾋﾟｰﾄﾞを歩く際のｽﾋﾟｰﾄﾞに設定
		me_.GetSpeed().x = 2;
	}
	// ﾎﾟｼﾞｼｮﾝ移動
	me_.GetPos().x += me_.GetSpeed().x;
	// ここで距離の計算をやって、変数に入れている
	auto distance = me_.GetNearestPlayer()->GetPos().x - me_.GetPos().x;

	// Podとプレイヤーとの距離が一定距離になれば
	// PodはプレイヤーのZ軸に合わせる行動をする
	if (abs(distance) < 150)
	{
		me_.GetAIState() = AIState::ZArrange;
		updater_ = &PodAI::Z_Arrangement;
		return true;
	}
	// 歩いている際に味方のAIColliderに当たったら
	if (IsHitFriends(enemies))
	{
		// 走るﾓｰｼｮﾝに遷移
		me_.GetAIState() = AIState::Run;
		updater_ = &PodAI::Run;
		return true;
	}
}

bool PodAI::Z_Arrangement(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// 左に進む
	if (me_.GetisTurnFlag())
	{
		me_.GetSpeed().x = -1;
	}
	// 右に進む
	else
	{
		me_.GetSpeed().x = 1;
	}
	// ﾌﾟﾚｲﾔｰのZ軸ﾎﾟｼﾞｼｮﾝの取得
	auto playerZ = me_.GetNearestPlayer()->GetZPos();
	// ﾌﾟﾚｲﾔｰより手前に存在すれば
	if (me_.GetZPos() >= playerZ)
	{
		// 手前に移動
		me_.GetZSpeed() = -1;
	}
	else
	{
		// 奥に移動
		me_.GetZSpeed() = 1;
	}
	// Z軸移動
	me_.GetZPos() += me_.GetZSpeed();
	// 味方に当たったら(傍に味方がいれば)
	if (IsHitFriends(enemies))
	{
		// AI状態をRunにする
		me_.GetAIState() = AIState::Run;
		// 自身の状態をRunに変更
		updater_ = &PodAI::Run;
		return true;
	}
	// Z軸がﾌﾟﾚｲﾔｰのZ軸と一致したら
	// 3というのはある程度ﾌﾟﾚｲﾔｰを視認可能な範囲を広げてるので
	if (abs(me_.GetZPos() - playerZ) <= 3)
	{
		// 攻撃準備ｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("attack_prepare");
		// AI状態をAttackに変更
		me_.GetAIState() = AIState::Attack;
		// 自身の状態をAttackに変更
		updater_ = &PodAI::Attack;
		return true;
	}
}

bool PodAI::Attack(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ｱﾆﾒｰｼｮﾝ終了時
	if (me_.GetisAnimEnd())
	{
		// 砲撃ｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("attack_release");
	}
	// 砲撃ｱﾆﾒｰｼｮﾝの場合
	if (me_.GetCurrentAnimation() == "attack_release")
	{
		// 弾が発射する際の火花ｱﾆﾒｰｼｮﾝを再生
		me_.AddMuzzleFlashAnimationCount(0.1f);
		// 火花ｱﾆﾒｰｼｮﾝが最大値になったら
		if (me_.GetmuzzleFlashAnimationCount() >= 5.0f)
		{
			// AIの当たり判定を再度可能にする
			me_.GetisHitAICollider() = false;
			// 火花ｱﾆﾒｰｼｮﾝｶｳﾝﾀｰを0初期化
			me_.GetmuzzleFlashAnimationCount() = 0.0f;
			// walkｱﾆﾒｰｼｮﾝに変更
			me_.ChangeAnimation("walk");
			// AI状態をWalkに変更
			me_.GetAIState() = AIState::Walk;
			// 自身の状態をwalkに変更
			updater_ = &PodAI::Walk;
			return true;
		}
	}
	else
	{
		// 左に移動
		// ﾌﾟﾚｲﾔｰが自分の左に存在したら
		if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
		{
			me_.GetisTurnFlag() = true;
			me_.GetSpeed().x = -2;
		}
		// 右に移動
		// ﾌﾟﾚｲﾔｰが自分の右に存在したら
		if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
		{
			me_.GetisTurnFlag() = false;
			me_.GetSpeed().x = 2;
		}
	}
}

bool PodAI::OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ｱﾆﾒｰｼｮﾝ終了時
	if (me_.GetisAnimEnd())
	{
		// HPが0以下
		if (me_.GetHp() <= 0)
		{
			// 死ぬｱﾆﾒｰｼｮﾝに変更
			me_.ChangeAnimation("death");
			// 自身の状態をdeathに変更
			updater_ = &PodAI::Death;
			return true;
		}
		else
		{
			// まだHPが0以下でなければWalkｱﾆﾒｰｼｮﾝに変更
			me_.ChangeAnimation("walk");
			// 自身の状態をWalkに変更
			updater_ = &PodAI::Walk;
			return true;
		}
	}
	// 1回ﾀﾞﾒｰｼﾞを食らったらfalseにしないと以降ﾀﾞﾒｰｼﾞが食らわないのでfalseにする
	me_.GetOnDamaged() = false;

	return false;
}

bool PodAI::Run(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾌﾟﾚｲﾔｰの向き
	auto playerIsTurnLeft = me_.GetNearestPlayer()->GetisTurnFlag();
	// もしﾌﾟﾚｲﾔｰが右を向いていればﾌﾟﾚｲﾔｰの左側に行くまで直進
	if (!playerIsTurnLeft)
	{
		me_.GetSpeed().x = -2;
	}
	// もしﾌﾟﾚｲﾔｰが左を向いていればﾌﾟﾚｲﾔｰの右側に行くまで直進
	else
	{
		me_.GetSpeed().x = 2;
	}

	// 直進させる
	me_.GetPos().x += me_.GetSpeed().x;

	// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝ
	auto playerPos = me_.GetNearestPlayer()->GetPos();

	if (!playerIsTurnLeft)
	{
		if (me_.GetPos().x <= playerPos.x - 100)
		{
			// AI状態をAttackに変更
			me_.GetAIState() = AIState::Attack;
			// 攻撃準備ｱﾆﾒｰｼｮﾝに変更
			me_.ChangeAnimation("attack_prepare");
			// 自身の行動状態をAttackに変更
			updater_ = &PodAI::Attack;
			return true;
		}
	}
	else
	{
		if (me_.GetPos().x >= playerPos.x + 100)
		{
			// AI状態をAttackに変更
			me_.GetAIState() = AIState::Attack;
			// 攻撃準備ｱﾆﾒｰｼｮﾝに変更
			me_.ChangeAnimation("attack_prepare");
			// 自身の行動状態をAttackに変更
			updater_ = &PodAI::Attack;
			return true;
		}
	}

	return false;
}

bool PodAI::Death(std::list<std::shared_ptr<Enemy>>& enemies)
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

void PodAI::Initialize(void)
{
	// まず最初の状態Walkにする
	updater_ = &PodAI::Walk;
}