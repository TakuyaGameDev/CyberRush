#include "Enemy.h"
#include "ControlledPlayer.h"
#include "SpacenautAI.h"
#include "Floor.h"

SpacenautAI::SpacenautAI(Enemy& enemy) :
	AIBase(enemy)
{
	// 初期化
	Initialize();
}
SpacenautAI::~SpacenautAI()
{
}

void SpacenautAI::Update(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾀﾞﾒｰｼﾞを受けると
	if (me_.GetOnDamaged())
	{
		// ｱﾆﾒｰｼｮﾝ再生可能ﾌﾗｸﾞをtrueに
		damage_anim_flag = true;
		// 自身の状態をﾀﾞﾒｰｼﾞを食らった状態にする
		updater_ = &SpacenautAI::OnDamaged;
	}
	// ｱﾆﾒｰｼｮﾝ再生可能ﾌﾗｸﾞがtrueならば
	if (damage_anim_flag)
	{
		// ﾀﾞﾒｰｼﾞｱﾆﾒｰｼｮﾝｶｳﾝﾀｰを加算
		// このことで自身を赤く点滅させる
		damage_anim_frame++;
		// 画像を赤く点滅させるためのｱﾙﾌｧ値(画像の透過度)計算
		me_.GetAlpha() = (100 / (((damage_anim_frame / 10) % 2) + 1));
	}
	// ﾀﾞﾒｰｼﾞｱﾆﾒｰｼｮﾝｶｳﾝﾀｰが一定値を超すと
	if (damage_anim_frame >= 60)
	{
		// ﾀﾞﾒｰｼﾞｱﾆﾒｰｼｮﾝﾌﾗｸﾞをfalseに
		damage_anim_flag = false;
		// ﾀﾞﾒｰｼﾞｱﾆﾒｰｼｮﾝｶｳﾝﾀｰを0に
		damage_anim_frame = 0;
		// ｱﾙﾌｧ値(画像の透過度)の初期化
		me_.GetAlpha() = 100.0f;
	}
	// 状態の更新
	(this->*updater_)(enemies);
}

bool SpacenautAI::Search(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾌﾟﾚｲﾔｰのZ軸ﾎﾟｼﾞｼｮﾝ
	target_pos_z = me_.GetNearestPlayer()->GetZPos();
	// ﾌｨｰﾙﾄﾞに存在する敵全体で回す
	for (auto enemy : enemies)
	{
		// ﾎﾞｽが敵の中に存在していれば
		if (enemy->GetType() == ActorType::Bigboy)
		{
			// そのﾎﾞｽのﾎﾟｼﾞｼｮﾝを格納
			partnerPos_ = enemy->GetPos();
		}
	}
	// 自身の状態をWalkにする
	updater_ = &SpacenautAI::Walk;
	return true;
}

bool SpacenautAI::Walk(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// 共に戦っているﾎﾞｽと自身のﾎﾟｼﾞｼｮﾝの距離
	auto distance = partnerPos_.x - me_.GetPos().x;

	// -------ここではﾎﾞｽと自身の位置交換させる処理--------
	// 上記の距離が一定距離になれば
	if (abs(distance) <= 30)
	{
		// 距離が0になったら = ﾎﾞｽと自身の位置が一緒になったら
		if (distance < 0)
		{
			// 右移動のﾌﾗｸﾞをtrueに
			moveRight_ = true;
		}
		else
		{
			// 左移動ﾌﾗｸﾞをtrueに
			moveLeft_ = true;
		}
	}
	else
	{
		// 自身の移動制御
		// ﾌｨｰﾙﾄﾞ外に行かせない処理
		if (me_.GetPos().x < 800 - 68 && me_.GetPos().x > 68)
		{
			// ﾌｨｰﾙﾄﾞ内に居れば
			if (me_.GetPos().x >= 400)
			{
				// 移動
				me_.GetSpeed().x = 1;
			}
			else
			{
				// 移動
				me_.GetSpeed().x = -1;
			}
		}
		// ﾌｨｰﾙﾄﾞ外の場合
		else
		{
			// 移動させない為にｽﾋﾟｰﾄﾞを0に
			me_.GetSpeed().x = 0;
			// 自身の状態をZ軸を合わせる行動にする
			updater_ = &SpacenautAI::Z_Arrangement;
		}
		// ﾎﾟｼﾞｼｮﾝ移動
		me_.GetPos().x += me_.GetSpeed().x;
	}
	//----------------------------------------------------------

	// ﾌﾟﾚｲﾔｰが自身よりも右にいる場合
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// 右向きにする
		me_.GetisTurnFlag() = true;
	}
	// ﾌﾟﾚｲﾔｰが自身よりも左にいる場合
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// 左向きにする
		me_.GetisTurnFlag() = false;
	}
	// 右移動ﾌﾗｸﾞがtrueだと
	if (moveRight_)
	{
		// 右移動をさせるためにﾌﾟﾗｽ値にする
		me_.GetSpeed().x = 1;
	}
	// 左移動ﾌﾗｸﾞがtrueだと
	if (moveLeft_)
	{
		// 左移動をさせるためにﾏｲﾅｽ値にする
		me_.GetSpeed().x = -1;
	}
	// ﾎﾟｼﾞｼｮﾝ移動(X)
	me_.GetPos().x += me_.GetSpeed().x;
	// 右移動ﾌﾗｸﾞがtrueの場合
	if (moveRight_)
	{
		// ﾌｨｰﾙﾄﾞの右端以上になったら
		if (me_.GetPos().x >= floorX - 40)
		{
			// ﾌｨｰﾙﾄﾞの右端より先には行かせない
			me_.GetPos().x = floorX - 40;
			// 右移動ﾌﾗｸﾞをfalseに
			moveRight_ = false;
			// 状態をﾌﾟﾚｲﾔｰのZ軸を合わせる行動の状態にする
			updater_ = &SpacenautAI::Z_Arrangement;
		}
	}
	// 左移動ﾌﾗｸﾞがtrueの場合
	if (moveLeft_)
	{
		// ﾌｨｰﾙﾄﾞの左端になったら
		if (me_.GetPos().x <= 50)
		{
			// ﾌｨｰﾙﾄﾞの左端より先には行かせない
			me_.GetPos().x = 50;
			// 左移動ﾌﾗｸﾞをfalseに
			moveLeft_ = false;
			// 状態をﾌﾟﾚｲﾔｰのZ軸を合わせる行動の状態にする
			updater_ = &SpacenautAI::Z_Arrangement;
		}
	}

	return true;
}

bool SpacenautAI::Z_Arrangement(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾌﾟﾚｲﾔｰのZ軸ﾎﾟｼﾞｼｮﾝ
	target_pos_z = me_.GetNearestPlayer()->GetZPos();
	// ﾌﾟﾚｲﾔｰが自身より手前にいる場合
	if (me_.GetZPos() >= target_pos_z)
	{
		// 手前に移動させる
		me_.GetZSpeed() = -1;
	}
	else
	{
		// 奥に移動させる
		me_.GetZSpeed() = 1;
	}
	// Z軸移動
	me_.GetZPos() += me_.GetZSpeed();
	// 自身のZ軸とﾌﾟﾚｲﾔｰのZ軸の距離が3以下で接地していたら
	// なぜ3以下?? -> ある程度余裕を持たせておくことでﾌﾟﾚｲﾔｰに自身の攻撃が当たるようにする
	if (abs(me_.GetZPos() - target_pos_z) <= 3 && me_.OnFloor() == true)
	{
		// 自身の状態をAttack状態にする
		updater_ = &SpacenautAI::Attack;
	}
	return true;
}

bool SpacenautAI::Attack(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// 攻撃しているｶｳﾝﾀｰを加算する
	frame++;
	// ｶｳﾝﾀｰが60ﾌﾚｰﾑ以内だと攻撃をし続ける
	if (frame <= 60)
	{
		// Attackｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("attack");
	}
	else
	{
		// 攻撃しているｶｳﾝﾀｰを0に
		frame = 0;
		// Walkｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("walk");
		// 自身の状態をﾌﾟﾚｲﾔｰを探している状態にする
		updater_ = &SpacenautAI::Search;
	}
	return true;
}

bool SpacenautAI::OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// HPが0以下になると
	if (me_.GetHp() <= 0)
	{
		// deathｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("death");
		// 自身の状態をdeathにする
		updater_ = &SpacenautAI::Death;
	}
	else
	{
		// Walkｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("walk");
		// 自身の状態をﾌﾟﾚｲﾔｰを探している状態にする
		updater_ = &SpacenautAI::Search;
	}
	// 1回ﾀﾞﾒｰｼﾞを食らったらfalseにしないと以降ﾀﾞﾒｰｼﾞが食らわないのでfalseにする
	me_.GetOnDamaged() = false;
	return false;
}

bool SpacenautAI::Death(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ｱﾆﾒｰｼｮﾝ終了時
	if (me_.GetisAnimEnd())
	{
		// 自身を削除する
		me_.Delete();
		return true;
	}
	return false;
}

void SpacenautAI::Initialize(void)
{
	// 攻撃ｶｳﾝﾀｰの初期化
	frame = 0;
	// ﾀﾞﾒｰｼﾞｱﾆﾒｰｼｮﾝｶｳﾝﾀｰの初期化
	damage_anim_frame = 0;
	// 共に戦うﾎﾞｽのﾎﾟｼﾞｼｮﾝ初期化
	partnerPos_ = { 0,0 };
	// 左移動ﾌﾗｸﾞの初期化
	moveLeft_ = false;
	// 右移動ﾌﾗｸﾞの初期化
	moveRight_ = false;
	// ﾀﾞﾒｰｼﾞｱﾆﾒｰｼｮﾝﾌﾗｸﾞの初期化
	damage_anim_flag = false;
	// 自身の初期状態をSearchに設定
	updater_ = &SpacenautAI::Search;
}
