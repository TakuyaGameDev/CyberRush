#include "Enemy.h"
#include "ControlledPlayer.h"
#include "SpacenautAI.h"
#include "Floor.h"

SpacenautAI::SpacenautAI(Enemy& enemy) :
	AIBase(enemy)
{
	// ������
	Initialize();
}
SpacenautAI::~SpacenautAI()
{
}

void SpacenautAI::Update(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ�ނ��󂯂��
	if (me_.GetOnDamaged())
	{
		// ��Ұ��ݍĐ��\�׸ނ�true��
		damage_anim_flag = true;
		// ���g�̏�Ԃ���Ұ�ނ�H�������Ԃɂ���
		updater_ = &SpacenautAI::OnDamaged;
	}
	// ��Ұ��ݍĐ��\�׸ނ�true�Ȃ��
	if (damage_anim_flag)
	{
		// ��Ұ�ޱ�Ұ��ݶ���������Z
		// ���̂��ƂŎ��g��Ԃ��_�ł�����
		damage_anim_frame++;
		// �摜��Ԃ��_�ł����邽�߂̱�̧�l(�摜�̓��ߓx)�v�Z
		me_.GetAlpha() = (100 / (((damage_anim_frame / 10) % 2) + 1));
	}
	// ��Ұ�ޱ�Ұ��ݶ���������l�𒴂���
	if (damage_anim_frame >= 60)
	{
		// ��Ұ�ޱ�Ұ����׸ނ�false��
		damage_anim_flag = false;
		// ��Ұ�ޱ�Ұ��ݶ������0��
		damage_anim_frame = 0;
		// ��̧�l(�摜�̓��ߓx)�̏�����
		me_.GetAlpha() = 100.0f;
	}
	// ��Ԃ̍X�V
	(this->*updater_)(enemies);
}

bool SpacenautAI::Search(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��ڲ԰��Z���߼޼��
	target_pos_z = me_.GetNearestPlayer()->GetZPos();
	// ̨���ނɑ��݂���G�S�̂ŉ�
	for (auto enemy : enemies)
	{
		// �޽���G�̒��ɑ��݂��Ă����
		if (enemy->GetType() == ActorType::Bigboy)
		{
			// �����޽���߼޼�݂��i�[
			partnerPos_ = enemy->GetPos();
		}
	}
	// ���g�̏�Ԃ�Walk�ɂ���
	updater_ = &SpacenautAI::Walk;
	return true;
}

bool SpacenautAI::Walk(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ���ɐ���Ă����޽�Ǝ��g���߼޼�݂̋���
	auto distance = partnerPos_.x - me_.GetPos().x;

	// -------�����ł��޽�Ǝ��g�̈ʒu���������鏈��--------
	// ��L�̋�������苗���ɂȂ��
	if (abs(distance) <= 30)
	{
		// ������0�ɂȂ����� = �޽�Ǝ��g�̈ʒu���ꏏ�ɂȂ�����
		if (distance < 0)
		{
			// �E�ړ����׸ނ�true��
			moveRight_ = true;
		}
		else
		{
			// ���ړ��׸ނ�true��
			moveLeft_ = true;
		}
	}
	else
	{
		// ���g�̈ړ�����
		// ̨���ފO�ɍs�����Ȃ�����
		if (me_.GetPos().x < 800 - 68 && me_.GetPos().x > 68)
		{
			// ̨���ޓ��ɋ����
			if (me_.GetPos().x >= 400)
			{
				// �ړ�
				me_.GetSpeed().x = 1;
			}
			else
			{
				// �ړ�
				me_.GetSpeed().x = -1;
			}
		}
		// ̨���ފO�̏ꍇ
		else
		{
			// �ړ������Ȃ��ׂɽ�߰�ނ�0��
			me_.GetSpeed().x = 0;
			// ���g�̏�Ԃ�Z�������킹��s���ɂ���
			updater_ = &SpacenautAI::Z_Arrangement;
		}
		// �߼޼�݈ړ�
		me_.GetPos().x += me_.GetSpeed().x;
	}
	//----------------------------------------------------------

	// ��ڲ԰�����g�����E�ɂ���ꍇ
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// �E�����ɂ���
		me_.GetisTurnFlag() = true;
	}
	// ��ڲ԰�����g�������ɂ���ꍇ
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// �������ɂ���
		me_.GetisTurnFlag() = false;
	}
	// �E�ړ��׸ނ�true����
	if (moveRight_)
	{
		// �E�ړ��������邽�߂���׽�l�ɂ���
		me_.GetSpeed().x = 1;
	}
	// ���ړ��׸ނ�true����
	if (moveLeft_)
	{
		// ���ړ��������邽�߂�ϲŽ�l�ɂ���
		me_.GetSpeed().x = -1;
	}
	// �߼޼�݈ړ�(X)
	me_.GetPos().x += me_.GetSpeed().x;
	// �E�ړ��׸ނ�true�̏ꍇ
	if (moveRight_)
	{
		// ̨���ނ̉E�[�ȏ�ɂȂ�����
		if (me_.GetPos().x >= floorX - 40)
		{
			// ̨���ނ̉E�[����ɂ͍s�����Ȃ�
			me_.GetPos().x = floorX - 40;
			// �E�ړ��׸ނ�false��
			moveRight_ = false;
			// ��Ԃ���ڲ԰��Z�������킹��s���̏�Ԃɂ���
			updater_ = &SpacenautAI::Z_Arrangement;
		}
	}
	// ���ړ��׸ނ�true�̏ꍇ
	if (moveLeft_)
	{
		// ̨���ނ̍��[�ɂȂ�����
		if (me_.GetPos().x <= 50)
		{
			// ̨���ނ̍��[����ɂ͍s�����Ȃ�
			me_.GetPos().x = 50;
			// ���ړ��׸ނ�false��
			moveLeft_ = false;
			// ��Ԃ���ڲ԰��Z�������킹��s���̏�Ԃɂ���
			updater_ = &SpacenautAI::Z_Arrangement;
		}
	}

	return true;
}

bool SpacenautAI::Z_Arrangement(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��ڲ԰��Z���߼޼��
	target_pos_z = me_.GetNearestPlayer()->GetZPos();
	// ��ڲ԰�����g����O�ɂ���ꍇ
	if (me_.GetZPos() >= target_pos_z)
	{
		// ��O�Ɉړ�������
		me_.GetZSpeed() = -1;
	}
	else
	{
		// ���Ɉړ�������
		me_.GetZSpeed() = 1;
	}
	// Z���ړ�
	me_.GetZPos() += me_.GetZSpeed();
	// ���g��Z������ڲ԰��Z���̋�����3�ȉ��Őڒn���Ă�����
	// �Ȃ�3�ȉ�?? -> ������x�]�T���������Ă������Ƃ���ڲ԰�Ɏ��g�̍U����������悤�ɂ���
	if (abs(me_.GetZPos() - target_pos_z) <= 3 && me_.OnFloor() == true)
	{
		// ���g�̏�Ԃ�Attack��Ԃɂ���
		updater_ = &SpacenautAI::Attack;
	}
	return true;
}

bool SpacenautAI::Attack(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// �U�����Ă��鶳��������Z����
	frame++;
	// �������60�ڰшȓ����ƍU������������
	if (frame <= 60)
	{
		// Attack��Ұ��݂ɕύX
		me_.ChangeAnimation("attack");
	}
	else
	{
		// �U�����Ă��鶳�����0��
		frame = 0;
		// Walk��Ұ��݂ɕύX
		me_.ChangeAnimation("walk");
		// ���g�̏�Ԃ���ڲ԰��T���Ă����Ԃɂ���
		updater_ = &SpacenautAI::Search;
	}
	return true;
}

bool SpacenautAI::OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// HP��0�ȉ��ɂȂ��
	if (me_.GetHp() <= 0)
	{
		// death��Ұ��݂ɕύX
		me_.ChangeAnimation("death");
		// ���g�̏�Ԃ�death�ɂ���
		updater_ = &SpacenautAI::Death;
	}
	else
	{
		// Walk��Ұ��݂ɕύX
		me_.ChangeAnimation("walk");
		// ���g�̏�Ԃ���ڲ԰��T���Ă����Ԃɂ���
		updater_ = &SpacenautAI::Search;
	}
	// 1����Ұ�ނ�H�������false�ɂ��Ȃ��ƈȍ~��Ұ�ނ��H���Ȃ��̂�false�ɂ���
	me_.GetOnDamaged() = false;
	return false;
}

bool SpacenautAI::Death(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ��ݏI����
	if (me_.GetisAnimEnd())
	{
		// ���g���폜����
		me_.Delete();
		return true;
	}
	return false;
}

void SpacenautAI::Initialize(void)
{
	// �U��������̏�����
	frame = 0;
	// ��Ұ�ޱ�Ұ��ݶ�����̏�����
	damage_anim_frame = 0;
	// ���ɐ키�޽���߼޼�ݏ�����
	partnerPos_ = { 0,0 };
	// ���ړ��׸ނ̏�����
	moveLeft_ = false;
	// �E�ړ��׸ނ̏�����
	moveRight_ = false;
	// ��Ұ�ޱ�Ұ����׸ނ̏�����
	damage_anim_flag = false;
	// ���g�̏�����Ԃ�Search�ɐݒ�
	updater_ = &SpacenautAI::Search;
}
