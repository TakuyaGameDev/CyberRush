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
	// ©•ª‚ªŒ»İÌßÚ²Ô°‚Ì¶‚É‚¢‚é‚©‚ÌÌ×¸Ş
	auto playerIsLeft = me_.SearchNearestPlayer()->GetisTurnFlag();
	// ÌßÚ²Ô°‚ÌÎß¼Ş¼®İæ“¾
	auto playerPos = me_.SearchNearestPlayer()->GetPos();
	// ÌßÚ²Ô°‚ªŒã‚ë‚É‚¢‚é‚©‚ÌÌ×¸Ş(‘¶İ‚·‚ê‚Îtrue‚µ‚È‚¯‚ê‚Îfalse)
	auto isBehind = false;
	// ÌßÚ²Ô°‚ª¶‚É‘¶İ‚µ
	if (playerIsLeft)
	{
		// ÌßÚ²Ô°‚ª©•ª‚ÌŒã‚ë‚É‘¶İ‚·‚ê‚Î
		if (me_.GetPos().x > playerPos.x)
		{
			// Ì×¸Ş‚ğtrue‚É
			isBehind = true;
		}
	}
	// ÌßÚ²Ô°‚ª‰E‚É‘¶İ‚µ
	else
	{
		// ÌßÚ²Ô°‚ª©•ª‚ÌŒã‚ë‚É‘¶İ‚·‚ê‚Î
		if (me_.GetPos().x < playerPos.x)
		{
			// Ì×¸Ş‚ğtrue‚É
			isBehind = true;
		}
	}
	// ÀŞÒ°¼Ş‚ğ‚­‚ç‚Á‚½‚ç
	if (me_.GetOnDamaged())
	{
		// HP‚ª0ˆÈ‰º‚É‚È‚é‚Æ
		if (me_.GetHp() <= 0)
		{
			// ”š”­‰¹‚ğ–Â‚ç‚·
			lpSound.Play("explosion",DX_PLAYTYPE_BACK);
		}
	}
	// “G“¯m‚ª“–‚½‚Á‚½‚ç
	if (me_.GetFriendlyFireFlag())
	{
		// ”š”­‰¹‚ğ–Â‚ç‚·
		lpSound.Play("explosion", DX_PLAYTYPE_BACK);
	}

	// “G‚ÌÌ¨°ÙÄŞˆÚ“®§Œä----------------------------
	// xÀ•W‚Ì§Œä
	if (me_.GetPos().x <= 50)
	{
		me_.GetPos().x = 50;
	}
	if (me_.GetPos().x >= floorX - 40)
	{
		me_.GetPos().x = floorX - 40;
	}
	// zÀ•W‚Ì§Œä
	if (me_.GetZPos() <= -floorZ)
	{
		me_.GetZPos() = -floorZ;
	}
	// “G‚ªÎŞ½‚Ìê‡‚Ìz²ˆÚ“®§Œä
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

	// “G‚ªÌßÚ²Ô°‚Ì”wŒã‚É‚¢‚é‚©‚¢‚È‚¢‚©Ì×¸Ş‚ğ¾¯Ä
	me_.SetisBehindPlayer(isBehind);
	// “G‚Ìs“®XV
	Update(enemies);
}

bool AIBase::IsHitFriends(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ÌßÚ²Ô°‚ÌŒã‚ë‚É“G‚ª‘¶İ‚·‚é‚©‚ÌÌ×¸Ş
	auto isBehindPlayer = false;
	// ÌßÚ²Ô°‚ÌŒã‚ë‚É“G‚ª‘¶İ‚·‚é‚©‚ÌÁª¯¸
	for (auto e1 : enemies)
	{
		isBehindPlayer = e1->GetisBehindPlayer();
	}
	// “G‚Ì©•ª©g‚ÌAIColliderÎß¼Ş¼®İ(X,Y²)
	auto myColPos = me_.GetAICollider()->GetPos();
	// “G‚Ì©•ª©g‚ÌAIColliderÎß¼Ş¼®İ(Z²F‰œs)
	auto myColPosZ = me_.GetAICollider()->GetZPos();
	// ÌßÚ²Ô°‚ÌŒã‚ë‚É‰½‚à‚¢‚È‚¯‚ê‚Î
	if (!isBehindPlayer)
	{
		for (auto e2 : enemies)
		{
			if (me_.GetId() != e2->GetId())
			{
				if (!e2->GetisHitAICollider() &&
					!me_.GetisHitAICollider())
				{
					// ©•ª‚Æ“–‚½‚Á‚½–¡•û‚ÌAICollider‚ÌÎß¼Ş¼®İ
					auto otherColPos
						= Vector2I(e2->GetAICollider()->GetPos().x,
							e2->GetAICollider()->GetPos().y +
							(e2->GetAICollider()->GetZPos() / 2));

					// –¡•û‚Æ©•ª‚ÌAI Collider‚ª“–‚½‚Á‚Ä‚¢‚½‚çAI‹N“®
					if (abs(otherColPos.x - myColPos.x) <= (size / 2) &&
						abs(otherColPos.y - myColPos.y) <= (size / 2))
					{
						// ©•ª‚Å‚Í‚È‚¢–¡•û‚ÌAICollider‚Ì“–‚½‚è”»’è‚ğtrue‚É
						e2->GetisHitAICollider() = true;
						// ©•ª‚ÌAICollider‚Ì“–‚½‚è”»’è‚ğtrue‚É
						me_.GetisHitAICollider() = true;
						return true;
					}
					else
					{
						// “–‚½‚Á‚Ä‚¢‚È‚¢
						return false;
					}
				}
			}
		}
	}
	return false;
}
