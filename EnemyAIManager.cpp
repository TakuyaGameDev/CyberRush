#include "EnemyAIManager.h"
#include "Enemy.h"
#include "PodAI.h"

EnemyAIManager::EnemyAIManager(std::list<std::shared_ptr<Enemy>>& friends):
friendList_(friends)
{

}

EnemyAIManager::~EnemyAIManager()
{

}

void EnemyAIManager::UpDate(const std::shared_ptr<Enemy> enemy)
{
	// AIBase‚ðe‚Æ‚µ‚Ä‚¢‚éPodAI“™‚ÌŠe“G·¬×‚É‰ž‚¶‚½AI‚Ì‹N“®
	// ‚»‚Ìæ‚Å‚ÍŠÖ”Îß²ÝÀ‚ð‰ñ‚µ‚Ä‚¢‚é‚Ì‚ÅAðŒ‚É‡’v‚µ‚½s“®‚ðŽÀs
	// friendList_ : –¡•û‚Ì“G
	enemy->GetAISystem()->AIAction(friendList_);
}


