#include <DxLib.h>
#include <algorithm>
#include "Spawner.h"
#include "PlayScene.h"
#include "ActorType.h"
#include "Enemy.h"
#include "Pod.h"
#include "Exoskeleton.h"
#include "Spacenaut.h"
#include "Bigboy.h"
#include "AIBase.h"
#include "Floor.h"
#include "BulletBase.h"

Spawner::Spawner()
{
	Initialize();
}

Spawner::~Spawner()
{
}

void Spawner::MakeClone(std::list<std::shared_ptr<Enemy>>& enemies,
	std::vector<std::shared_ptr<ControlledPlayer>>& player,
	const Wave& wave,const bool& flg)
{
	// �G������
	switch (wave)
	{
	case Wave::FirstWave:
		//if (bossFlag_)
		//{
		//	enemyType_ = ActorType::Bigboy;
		//	// �X�|�[���߼޼��
		//	spawnPos_ = Vector2I(floorX - 100, 0);
		//	// z���߼޼��
		//	spawnPosZ_ = -floorZ + 80;
		//	bossFlag_ = false;
		//}
		//else
		//{
		//	enemyType_ = ActorType::Pod/* + GetRand(static_cast<int>(ActorType::Pod))*/;
		//	// �X�|�[���߼޼��
		//	spawnPos_ = Vector2I(10 + GetRand(floorX - 10), 0);
		//	// z���߼޼��
		//	spawnPosZ_ = -floorZ + 30 + GetRand(floorZ);
		//}
		enemyType_ = ActorType::Pod;
		// �X�|�[���߼޼��
		spawnPos_ = Vector2I(10 + GetRand(floorX - 10), 0);
		// z���߼޼��
		spawnPosZ_ = -floorZ + GetRand(floorZ);
		// �X�|�[���߼޼��
		spawnPos_ = Vector2I(floorX - 100, 0);
		// z���߼޼��
		spawnPosZ_ = -floorZ + 80;

		enemyInstanceFunc_[enemyType_](enemies, player, spawnPos_, spawnPosZ_, enemyType_);

		break;
	case Wave::SecondWave:
		enemyType_ = ActorType::Pod + GetRand(static_cast<int>(ActorType::Pod));
		// �X�|�[���߼޼��
		spawnPos_ = Vector2I(10 + GetRand(floorX - 10), 0);
		// z���߼޼��
		spawnPosZ_ = -floorZ + GetRand(floorZ);
		enemyInstanceFunc_[enemyType_](enemies, player, spawnPos_, spawnPosZ_, enemyType_);

		break;
	case Wave::ThirdWave:
		if (bossFlag_)
		{
			enemyType_ = ActorType::Bigboy;
			// �X�|�[���߼޼��
			spawnPos_ = Vector2I(floorX - 100, 0);
			// z���߼޼��
			spawnPosZ_ = -floorZ + 80;
			bossFlag_ = false;
			enemyInstanceFunc_[enemyType_](enemies, player, spawnPos_, spawnPosZ_, enemyType_);
		}
		else
		{
			if (flg)
			{
				enemyType_ = ActorType::Spacenaut;
				// �X�|�[���߼޼��
				spawnPos_ = Vector2I(10 + GetRand(floorX - 10), 0);
				// z���߼޼��
				spawnPosZ_ = -floorZ + 50 + GetRand(floorZ);
				bossFlag_ = false;
				enemyInstanceFunc_[enemyType_](enemies, player, spawnPos_, spawnPosZ_, enemyType_);
			}
		}
		//else
		//{
		//	enemyType_ = ActorType::Pod + GetRand(static_cast<int>(ActorType::Pod));
		//	// �X�|�[���߼޼��
		//	spawnPos_ = Vector2I(10 + GetRand(floorX - 10), 0);
		//	// z���߼޼��
		//	spawnPosZ_ = -floorZ + GetRand(floorZ);
		//}
		break;
	default:
		break;
	}

}

bool Spawner::Initialize(void)
{
	bossFlag_ = true;
	enemyInstanceFunc_.try_emplace(ActorType::Pod,
		[&](std::list<std::shared_ptr<Enemy>>& enemies,
			std::vector<std::shared_ptr<ControlledPlayer>>& player,
			Vector2I pos, int z, ActorType type) {

				enemies.emplace_back(std::make_shared<Pod>(pos, z, type, player));

		});
	enemyInstanceFunc_.try_emplace(ActorType::Exoskeleton,
		[&](std::list<std::shared_ptr<Enemy>>& enemies,
			std::vector<std::shared_ptr<ControlledPlayer>>& player,
			Vector2I pos, int z, ActorType type) {

				enemies.emplace_back(std::make_shared<Exoskeleton>(pos, z, type, player));
		});
	enemyInstanceFunc_.try_emplace(ActorType::Bigboy,
		[&](std::list<std::shared_ptr<Enemy>>& enemies,
			std::vector<std::shared_ptr<ControlledPlayer>>& player,
			Vector2I pos, int z, ActorType type) {

				enemies.emplace_back(std::make_shared<Bigboy>(pos, z, type, player));
		});
	enemyInstanceFunc_.try_emplace(ActorType::Spacenaut,
		[&](std::list<std::shared_ptr<Enemy>>& enemies,
			std::vector<std::shared_ptr<ControlledPlayer>>& player,
			Vector2I pos, int z, ActorType type) {
				enemies.emplace_back(std::make_shared<Spacenaut>(pos, z, type, player));
		});
	/*enemyInstanceFunc_.try_emplace(ActorType::Pod,
		[&](std::list<std::shared_ptr<Enemy>>& enemies,
			std::vector<std::shared_ptr<ControlledPlayer>>& player,
			Vector2I pos, int z, ActorType type) {

				enemies.emplace_back(std::make_shared<Pod>(pos, z, type, player));
		});
	enemyInstanceFunc_.try_emplace(ActorType::Pod,
		[&](std::list<std::shared_ptr<Enemy>>& enemies,
			std::vector<std::shared_ptr<ControlledPlayer>>& player,
			Vector2I pos, int z, ActorType type) {

				enemies.emplace_back(std::make_shared<Pod>(pos, z, type, player));
		});*/

	return true;
}
