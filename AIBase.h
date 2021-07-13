#pragma once
#include <list>
#include <memory>

class Enemy;
class AICollider;

class AIBase
{
public:
	AIBase(Enemy& enemy);
	~AIBase();
	// �e�G��AI�s���ꊇ���s
	virtual void AIAction(std::list<std::shared_ptr<Enemy>>& enemies);
	// �e�G��AI�v�lٰ�݂̍X�V
	virtual void Update(std::list<std::shared_ptr<Enemy>>& enemies) = 0;
	// ������
	virtual void Initialize(void) = 0;
	// �e�ײ�ް�̓����蔻��(�������g�Ǝ����ȊO�̖����Ŕ�r)
	bool IsHitFriends(std::list<std::shared_ptr<Enemy>>& enemies);
protected:
	// �������g
	Enemy& me_;
	// 1��e�e��ݽ�ݽ������true��
	bool createBulletFlag_;
};

