#pragma once
#include <list>
#include <vector>
#include <map>
#include <memory>
#include <functional>

#include "ActorType.h"

class ControlledPlayer;
class AIBase;
class Enemy;

class EnemyAIManager
{
public:
	EnemyAIManager(std::list<std::shared_ptr<Enemy>>& friends);
	~EnemyAIManager();

	// �G�̍s���̌���
	void UpDate(const std::shared_ptr<Enemy> enemy);

private:
	// �����ȊO�̖���
	std::list<std::shared_ptr<Enemy>>& friendList_;

	// �e�G��AI�֐���G�^�C�v�ł̘A�z�z��
	std::map<ActorType,std::function<void(void)>> aiFunc_;

};

