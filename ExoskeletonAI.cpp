#include <cmath>
#include "Collision.h"
#include "Enemy.h"
#include "Floor.h"
#include "ControlledPlayer.h"
#include "WeaponBase.h"
#include "ExoskeletonAI.h"

ExoskeletonAI::ExoskeletonAI(Enemy& enemy):
	AIBase(enemy)
{
	// 初期化
	Initialize();
}

ExoskeletonAI::~ExoskeletonAI()
{
}

void ExoskeletonAI::Update(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾌﾟﾚｲﾔｰを探し出すｶｳﾝﾄを加算
	// この値が一定になるとプレイヤーに向かって直進
	searchFrame++;
	// ﾌｨｰﾙﾄﾞとの当たり判定
	if (BoxOutCollision()(Vector2I(me_.GetPos().x,me_.GetZPos()),
		Vector2I(0,-320),Vector2I(floorX,floorZ)))
	{
		// 地形に当たっていたらdeathｱﾆﾒｰｼｮﾝに変更
		me_.ChangeAnimation("death");
		// 自身をdeath状態に変更
		updater_ = &ExoskeletonAI::Death;
	}
	// 円の当たり判定
	if (CircleCollision()(me_.GetType(),
		me_.SearchNearestPlayer()->GetPos() - me_.GetPos(),
		me_.GetSize() + me_.GetNearestPlayer()->GetSize(),
		me_.GetZPos() - me_.GetNearestPlayer()->GetZPos()))
	{
		// deathｱﾆﾒｰｼｮﾝの変更
		me_.ChangeAnimation("death");
		// ﾌﾟﾚｲﾔｰがﾀﾞﾒｰｼﾞを食らったﾌﾗｸﾞをtrueにする
		// ﾌﾟﾚｲﾔｰにﾀﾞﾒｰｼﾞを与える
		me_.GetNearestPlayer()->GetOnDamaged() = true;
		// 自身の状態をdeathに変更
		updater_ = &ExoskeletonAI::Death;
	}
	// ﾌｨｰﾙﾄﾞに存在する敵分回す
	for (auto enemy : enemies)
	{
		// 現在回している敵が自分ではいけないのでこのように記述
		if (me_.GetId() != enemy->GetId())
		{
			// 味方との当たり判定
			if (CircleCollision()(enemy->GetType(),
				enemy->GetPos() - me_.GetPos(), enemy->GetSize() + me_.GetSize(),
				enemy->GetZPos() - me_.GetZPos()))
			{
				// 味方への攻撃が当たったﾌﾗｸﾞをtrueに
				enemy->GetFriendlyFireFlag() = true;
				// deathｱﾆﾒｰｼｮﾝに変更
				me_.ChangeAnimation("death");
				// 自身の状態をdeathに変更
				updater_ = &ExoskeletonAI::Death;
			}
		}
	}
	// 画像の方向変更
	// 左向き
	if (rad >= 0)
	{
		me_.GetisTurnFlag() = false;
	}
	// 右向き
	else
	{
		me_.GetisTurnFlag() = true;
	}
	(this->*updater_)(enemies);
}

bool ExoskeletonAI::Run(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// 接地していたら
	if (me_.OnFloor() == true)
	{
		// ﾌﾟﾚｲﾔｰを検知する値が59になる = ﾌﾟﾚｲﾔｰに直進する
		if ((searchFrame / 2) % 60 == 59)
		{
			player_pos = Vector2I(me_.GetNearestPlayer()->GetPos().x,
								  me_.GetNearestPlayer()->GetZPos());
			rad = atan2(player_pos.y - me_.GetZPos(),
						player_pos.x - me_.GetPos().x);
		}
		me_.GetPos().x += 5 * cos(rad);
		me_.GetZPos() += 5 * sin(rad);
	}
	// ﾌﾟﾚｲﾔｰを検知する値が59以下
	// 直進はしないで常にﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝを検索
	// 直進するまでﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝを更新し続ける
	else
	{
		player_pos = Vector2I(me_.GetNearestPlayer()->GetPos().x,
							  me_.GetNearestPlayer()->GetZPos());
			rad = atan2(player_pos.y - me_.GetZPos(),
						player_pos.x - me_.GetPos().x);
	}

	return false;
}

bool ExoskeletonAI::Death(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ｱﾆﾒｰｼｮﾝ終了時
	if (me_.GetisAnimEnd())
	{
		// 自身を削除
		me_.Delete();
		return true;
	}
	return false;
}

void ExoskeletonAI::Initialize(void)
{
	// ﾌﾟﾚｲﾔｰを検知するｶｳﾝﾄ
	searchFrame = 0;
	// 初期状態としてRunに設定
	updater_ = &ExoskeletonAI::Run;
}
