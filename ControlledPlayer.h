#pragma once
#include <list>

#include "Actor.h"
#include "PLAYER.h"

//class Enemy;
class WeaponBase;
class Item;

class ControlledPlayer :
	public Actor
{
public:
	ControlledPlayer(Vector2I pos, int z, const ActorType& type,std::list<std::shared_ptr<Item>>& itemList);
	~ControlledPlayer();

	void UpDate(void);

	const std::shared_ptr<WeaponBase>& GetCurrentWeapon(void)
	{
		return currentWeapon_;
	}

	virtual bool& GetisOnFloor(void)
	{
		return isOnFloor_;
	}

	void Draw_(void);

private:
	// ��P�Ȃ̂���Actor�׽�Œm���悤�ɂ��邽�߂ɂ����ɋL��
	static int player_;
	// ����P�Ȃ̂��̏��
	PLAYER playerNo_ = PLAYER::MAX;

	bool Initialize(void);
	void Walk(const Vector2I& speed, const int& zSp);
	void Jump(void);
	void Fire(void);
	// ���ю擾
	void GetItems(void);

	// ��ڲ԰�̍s���Ɋւ���֐��߲��
	void(ControlledPlayer::* updater_)(void);
	// �G��ؽ�
	//std::list<std::shared_ptr<Enemy>>& enemies_;
	std::vector<std::shared_ptr<WeaponBase>> weapons_;
	std::shared_ptr<WeaponBase> currentWeapon_;

	// �����ݑ������Ă��镐���list������������Ă������̂�
	// ���ݑ������Ă��镐��i���o�[
	int currentWeaponNo_;

	// �������ڲ԰�̎茳�ɍ��킹�邽�߂̒����p�ϐ�
	Vector2I weaponsArrangementAmount_;

	// ���̉��͈͂���ڲ԰�����邩���׸�
	bool isOnFloor_;
	Vector2I weaponsUIPos_;

	Vector2I bulletNumPos_;
	// ���Z�l
	int addBlendval_;
	bool addBlendFlag_;

	std::list<std::shared_ptr<Item>>& items_;

	std::list<std::pair<std::pair<Vector2I,int>,int>> getBulletNumPos_;
};

