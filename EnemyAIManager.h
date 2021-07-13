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

	// 敵の行動の決定
	void UpDate(const std::shared_ptr<Enemy> enemy);

private:
	// 自分以外の味方
	std::list<std::shared_ptr<Enemy>>& friendList_;

	// 各敵のAI関数を敵タイプでの連想配列
	std::map<ActorType,std::function<void(void)>> aiFunc_;

};

