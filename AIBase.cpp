#include <DxLib.h>
#include "SoundManager.h"
#include "AIBase.h"
#include "Enemy.h"
#include "PodAI.h"
#include "AICollider.h"
#include "ControlledPlayer.h"
#include "Floor.h"

AIBase::AIBase(Enemy& enemy):
	me_(enemy)
{
	createBulletFlag_ = false;
}

AIBase::~AIBase()
{
}

void AIBase::AIAction(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// 自分が現在ﾌﾟﾚｲﾔｰの左にいるかのﾌﾗｸﾞ
	auto playerIsLeft = me_.SearchNearestPlayer()->GetisTurnFlag();
	// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝ取得
	auto playerPos = me_.SearchNearestPlayer()->GetPos();
	// ﾌﾟﾚｲﾔｰが後ろにいるかのﾌﾗｸﾞ(存在すればtrueしなければfalse)
	auto isBehind = false;
	// ﾌﾟﾚｲﾔｰが左に存在し
	if (playerIsLeft)
	{
		// ﾌﾟﾚｲﾔｰが自分の後ろに存在すれば
		if (me_.GetPos().x > playerPos.x)
		{
			// ﾌﾗｸﾞをtrueに
			isBehind = true;
		}
	}
	// ﾌﾟﾚｲﾔｰが右に存在し
	else
	{
		// ﾌﾟﾚｲﾔｰが自分の後ろに存在すれば
		if (me_.GetPos().x < playerPos.x)
		{
			// ﾌﾗｸﾞをtrueに
			isBehind = true;
		}
	}
	// ﾀﾞﾒｰｼﾞをくらったら
	if (me_.GetOnDamaged())
	{
		// HPが0以下になると
		if (me_.GetHp() <= 0)
		{
			// 爆発音を鳴らす
			lpSound.Play("explosion",DX_PLAYTYPE_BACK);
		}
	}
	// 敵同士が当たったら
	if (me_.GetFriendlyFireFlag())
	{
		// 爆発音を鳴らす
		lpSound.Play("explosion", DX_PLAYTYPE_BACK);
	}

	// 敵のﾌｨｰﾙﾄﾞ移動制御----------------------------
	// x座標の制御
	if (me_.GetPos().x <= 50)
	{
		me_.GetPos().x = 50;
	}
	if (me_.GetPos().x >= floorX - 40)
	{
		me_.GetPos().x = floorX - 40;
	}
	// z座標の制御
	if (me_.GetZPos() <= -floorZ)
	{
		me_.GetZPos() = -floorZ;
	}
	// 敵がﾎﾞｽの場合のz軸移動制御
	if (me_.GetType() == ActorType::Bigboy)
	{
		if (me_.GetZPos() >= -120)
		{
			me_.GetZPos() = -120;
		}
	}
	else
	{
		if (me_.GetZPos() >= 0)
		{
			me_.GetZPos() = 0;
		}
	}
	// --------------------------------------------

	// 敵がﾌﾟﾚｲﾔｰの背後にいるかいないかﾌﾗｸﾞをｾｯﾄ
	me_.SetisBehindPlayer(isBehind);
	// 敵の行動更新
	Update(enemies);
}

bool AIBase::IsHitFriends(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ﾌﾟﾚｲﾔｰの後ろに敵が存在するかのﾌﾗｸﾞ
	auto isBehindPlayer = false;
	// ﾌﾟﾚｲﾔｰの後ろに敵が存在するかのﾁｪｯｸ
	for (auto e1 : enemies)
	{
		isBehindPlayer = e1->GetisBehindPlayer();
	}
	// 敵の自分自身のAIColliderﾎﾟｼﾞｼｮﾝ(X,Y軸)
	auto myColPos = me_.GetAICollider()->GetPos();
	// 敵の自分自身のAIColliderﾎﾟｼﾞｼｮﾝ(Z軸：奥行)
	auto myColPosZ = me_.GetAICollider()->GetZPos();
	// ﾌﾟﾚｲﾔｰの後ろに何もいなければ
	if (!isBehindPlayer)
	{
		for (auto e2 : enemies)
		{
			if (me_.GetId() != e2->GetId())
			{
				if (!e2->GetisHitAICollider() &&
					!me_.GetisHitAICollider())
				{
					// 自分と当たった味方のAIColliderのﾎﾟｼﾞｼｮﾝ
					auto otherColPos
						= Vector2I(e2->GetAICollider()->GetPos().x,
							e2->GetAICollider()->GetPos().y +
							(e2->GetAICollider()->GetZPos() / 2));

					// 味方と自分のAI Colliderが当たっていたらAI起動
					if (abs(otherColPos.x - myColPos.x) <= (size / 2) &&
						abs(otherColPos.y - myColPos.y) <= (size / 2))
					{
						// 自分ではない味方のAIColliderの当たり判定をtrueに
						e2->GetisHitAICollider() = true;
						// 自分のAIColliderの当たり判定をtrueに
						me_.GetisHitAICollider() = true;
						return true;
					}
					else
					{
						// 当たっていない時
						return false;
					}
				}
			}
		}
	}
	return false;
}
