#pragma once
#include "AIBase.h"

class Enemy;

class SpacenautAI :
	public AIBase
{
public:
	SpacenautAI(Enemy& enemy);
	~SpacenautAI();
	// �X�V
	void Update(std::list<std::shared_ptr<Enemy>>& enemies);
	// ��ڲ԰�̈ʒu���������
	bool Search(std::list<std::shared_ptr<Enemy>>& enemies);
	// �����s��
	bool Walk(std::list<std::shared_ptr<Enemy>>& enemies);
	// ��ڲ԰��Z���Ɏ�����Z�������킹��
	bool Z_Arrangement(std::list<std::shared_ptr<Enemy>>& enemies);
	// Z������ڲ԰�Ƃ�������U��������(�e���o��)
	bool Attack(std::list<std::shared_ptr<Enemy>>& enemies);
	// ��Ұ�ސH�炢����
	bool OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies);
	// ���ʏ���
	bool Death(std::list<std::shared_ptr<Enemy>>& enemies);

private:
	// ������
	void Initialize(void);
	// ��Ԋ֐��߲��
	bool(SpacenautAI::* updater_)(std::list<std::shared_ptr<Enemy>>&);

	int frame;
	// ��Ұ�ނ�H����������ڰѶ���(���g��Ԃ��_�ł����邽�߂Ɏg�p)
	int damage_anim_frame;
	// ��ڲ԰��Z���߼޼��
	int target_pos_z;
	// ���ɐ키�޽���߼޼��
	Vector2I partnerPos_;
	// �E�ɓ����׸�
	bool moveRight_;
	// ���ɓ����׸�
	bool moveLeft_;
	// ��Ұ��ݍĐ��\�׸�
	bool damage_anim_flag;
};
