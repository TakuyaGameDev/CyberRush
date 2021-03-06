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
	// AIBaseを親としているPodAI等の各敵ｷｬﾗに応じたAIの起動
	// その先では関数ﾎﾟｲﾝﾀを回しているので、条件に合致した行動を実行
	// friendList_ : 味方の敵
	enemy->GetAISystem()->AIAction(friendList_);
}


