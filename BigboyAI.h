#pragma once
#include <vector>
#include "AIBase.h"

#include "Geometory.h"

class BigboyAI :
	public AIBase
{
public:
	BigboyAI(Enemy& enemy);
	~BigboyAI();
	// �X�V
	void Update(std::list<std::shared_ptr<Enemy>>& enemies);
private:
	// ������
	void Initialize(void);
	// �֐��߲��
	bool(BigboyAI::* updater_)(std::list<std::shared_ptr<Enemy>>& enemies);

	// ��莞�Ԏ~�܂�����ڲ԰�̍s�����ώ@����B
	// ������Bigboy�̍s�������肷��B
	bool Observe(std::list<std::shared_ptr<Enemy>>& enemies);
	// ��ڲ԰�Ɍ������ĕ���
	bool Walk(std::list<std::shared_ptr<Enemy>>& enemies);
	// ��ڲ԰�Ƃ�Z�������킹��
	bool ZArrange(std::list<std::shared_ptr<Enemy>>& enemies);
	// �ڰђP�ʂōU������
	bool Attack(std::list<std::shared_ptr<Enemy>>& enemies);
	// ��莞�Ԍo�����獂���ެ��߂���
	bool Jump(std::list<std::shared_ptr<Enemy>>& enemies);
	// ��Ұ��
	bool OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies);
	// ����
	bool Death(std::list<std::shared_ptr<Enemy>>& enemies);
	// �^�b�N��
	bool LaserAttack(std::list<std::shared_ptr<Enemy>>& enemies);
	// �ެ��߽�߰��
	float jumpSp_;
	// �ެ��ߗ�
	float jumpForce_;
	// �ެ��߂̏����x
	float jumpFirstSp_;
	//// ��ڲ԰�Ɍ������đ̓����肷������x
	//float tackleAccelSp_;
	//// �̓����肷���߰��
	//float tackleSp_;
	//// �̓�����̏����x
	//float tackleFirstSp_;
	// ���`��O ��O�`�� �ɕ�����߰��
	int walkFrame_;
	// �ެ��߂��Ă��邩���׸�
	bool isJumpAction_;
	// �ެ��߂�����Ԋu
	int jumpInterval_;
};

