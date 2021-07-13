#include <EffekseerForDXLib.h>

#include "BigboyAI.h"
#include "Enemy.h"
#include "ControlledPlayer.h"
#include "Floor.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "Collision.h"

BigboyAI::BigboyAI(Enemy& enemy):
	AIBase(enemy)
{
	// ������
	Initialize();
}

BigboyAI::~BigboyAI()
{
}

void BigboyAI::Update(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ�ނ�H�������
	if (me_.GetOnDamaged())
	{
		// hit��Ұ��݂ɕύX
		me_.ChangeAnimation("hit");
		// ���g�̏�Ԃ���Ұ�ނ�H�������Ԃɂ���
		updater_ = &BigboyAI::OnDamaged;
	}
	// ��������̍U����H������ꍇ
	if (me_.GetFriendlyFireFlag())
	{
		// hit��Ұ��݂ɕύX
		me_.ChangeAnimation("hit");
		// ���g�̏�Ԃ���Ұ�ނ�H�������Ԃɂ���
		updater_ = &BigboyAI::OnDamaged;
	}
	// �ެ��߂��Ă��Ȃ���
	if (!isJumpAction_)
	{
		// �ެ��߂�����Ԋu�����Z���Ă���
		jumpInterval_++;
		// �ެ��߂�����Ԋu���Ă����ȏ�ɂȂ�����
		if (jumpInterval_ >= 2000)
		{
			// �ެ��ߊԊu���Ă�0��
			jumpInterval_ = 0;
			// �ެ��߂����Ă����׸ނ�true�ɂ���
			isJumpAction_ = true;
		}
	}
	// ���g�̏�Ԗ��̍X�V
	(this->*updater_)(enemies);
}

void BigboyAI::Initialize(void)
{
	// ������Ԃ�Observe(�ώ@=��ڲ԰�̓��������Ă���)��Ԃɐݒ�
	updater_ = &BigboyAI::Observe;
	// ����������̏�����
	walkFrame_ = 0;
	// �ެ��߂��Ă����׸ނ̏�����
	isJumpAction_ = false;
	// �ެ��߽�߰�ނ̏�����
	jumpSp_ = 0;
	// �ެ��ߏ����x�̏�����
	// ���\�����ެ��߂��������̂�-45.0f�Ɛݒ�
	jumpFirstSp_ = -45.0f;
	// �ެ��ߗ͂̏�����
	jumpForce_ = 0.0f;
	// �ެ��ߊԊu���Ă̏�����
	jumpInterval_ = 0;
}

bool BigboyAI::Observe(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��ڲ԰��Z���߼޼��
	auto playerZ = me_.SearchNearestPlayer()->GetZPos();
	// ��ڲ԰��x,y���߼޼��
	auto playerPos = me_.GetNearestPlayer()->GetPos();
	// ���g����ڲ԰��Z���̍�
	auto zdiff = playerZ - me_.GetZPos();
	// ���g����ڲ԰�Ƃ�x,y���̋���(�����蔻��Ɏg�p)
	auto distance = playerPos - me_.GetPos();
	// ��ڲ԰�����g�������ɂ���ꍇ
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// ���g��x���̽�߰�ނ�ݒ�(�������ɂȂ�悤��ϲŽ�l)
		me_.GetSpeed().x = -7;
		// �������ɂ���
		me_.GetisTurnFlag() = false;
	}
	// ��ڲ԰�����g�����E�ɂ���ꍇ
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// ���g��x���̽�߰�ނ�ݒ�(�E�����ɂȂ�悤����׽�l)
		me_.GetSpeed().x = 7;
		// �E�����ɂ���
		me_.GetisTurnFlag() = true;
	}
	// �ެ��߱���݂łȂ���
	if (!isJumpAction_)
	{
		// ���g����ڲ԰�̋��������ȏゾ��
		if (abs(distance.x) >= 170)
		{
			// ���g�̼ެ��߂��Ă����׸ނ�true��
			me_.IsJumping() = true;
			// �ެ��߱���݂����Ă����׸ނ�true��
			isJumpAction_ = true;
			// ���g�̏�Ԃ�Jump��Ԃɂ���
			updater_ = &BigboyAI::Jump;
			return true;
		}
	}
	else
	{
		// walk��Ұ��݂ɕύX
		me_.ChangeAnimation("walk");
		// ���g�̏�Ԃ�Z�������킹��s���̏�ԂɕύX
		updater_ = &BigboyAI::ZArrange;
	}

	return true;

}

bool BigboyAI::Walk(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// �����Ă��鶳�Ă����Z
	walkFrame_++;
	// ��ڲ԰�����g�������ɂ���ꍇ
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// �������ɂ���
		me_.GetisTurnFlag() = false;
		// x����߰�ނ���������߰�ނ�
		me_.GetSpeed().x = -1;
	}
	// ��ڲ԰�����g�����E�ɂ���ꍇ
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// �E�����ɂ���
		me_.GetisTurnFlag() = true;
		// x����߰�ނ��E������߰�ނ�
		me_.GetSpeed().x = 1;
	}
	// ��ڲ԰��Z���߼޼��
	auto playerZ = me_.GetNearestPlayer()->GetZPos();
	// ��ڲ԰�����g������O�ɂ���ꍇ
	if (me_.GetZPos() >= playerZ)
	{
		// Z����߰�ނ�ϲŽ�l��
		me_.GetZSpeed() = -1;
	}
	else
	{
		// Z����߰�ނ���׽�l��
		me_.GetZSpeed() = 1;
	}
	// ��������������ȉ�����
	if (walkFrame_ <= 100)
	{
		// �߼޼�݈ړ�
		me_.GetPos().x += me_.GetSpeed().x;
		// Z���߼޼�݈ړ�
		me_.GetZPos() += me_.GetZSpeed();
	}
	// �������Ă����ȏ�ɂȂ��
	else
	{
		// �������Ă�0�ɂ���
		walkFrame_ = 0.0f;
		// Idle��Ұ��݂ɕύX
		me_.ChangeAnimation("idle");
		// ���g�̏�Ԃ�Observe��Ԃɂ���
		updater_ = &BigboyAI::Observe;
	}

	return true;
}

bool BigboyAI::ZArrange(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��ڲ԰��Z���߼޼��
	auto playerZ = me_.GetNearestPlayer()->GetZPos();
	// ��ڲ԰�����g����O�ɂ���ꍇ
	if (me_.GetZPos() >= playerZ)
	{
		// ��O�Ɉړ����邽�߂�ϲŽ�l�̾��
		me_.GetZSpeed() = -1;
	}
	else
	{
		// ���Ɉړ����邽�߂���׽�l�̾��
		me_.GetZSpeed() = 1;
	}
	// Z���̈ړ�
	me_.GetZPos() += me_.GetZSpeed();
	// ��ڲ԰��Z���߼޼�݂Ǝ��g��Z���߼޼�݂̍����v�Z
	auto zdiff = playerZ - me_.GetZPos();
	// ��L�̍�����5�ȉ�����
	// �Ȃ�5? -> Z�������蔻��ɏ����]���������Ĕ͈͂��L�����邱�Ƃ�
	//           ������ƓG�̍U����������悤�ɂ��Ă���
	if (abs(zdiff) <= 5)
	{
		// Attack��Ұ��݂̕ύX
		me_.ChangeAnimation("attack");
		// ���g�̏�Ԃ�LaserAttack�ɕύX
		updater_ = &BigboyAI::LaserAttack;
	}

	return false;
}

bool BigboyAI::Attack(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��ڲ԰�����g�������ɂ���ꍇ
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// ��������������
		me_.GetisTurnFlag() = false;
		// ��������߰�ނɐݒ�
		me_.GetSpeed().x = -1;
	}
	// ��ڲ԰�����g�����E�ɂ���ꍇ
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// �������E������
		me_.GetisTurnFlag() = true;
		// �E������߰�ނɐݒ�
		me_.GetSpeed().x = 1;
	}
	// ��Ұ��ݏI����
	if (me_.GetisAnimEnd())
	{
		// Idle��Ұ��݂ɕύX
		me_.ChangeAnimation("idle");
		// ��Ԃ�Observe��ԂɕύX
		updater_ = &BigboyAI::Observe;
	}
	return true;
}

bool BigboyAI::Jump(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// �ެ��ߗ͂����Z
	jumpForce_++;
	// �ެ��ߗ� = �ެ��ߏ����x + �ެ��ߗ�
	// �]���āA-45.0f���炾�񂾂�0�ɂȂ��Ă���
	jumpSp_ = jumpFirstSp_ + jumpForce_;
	// �����ɑ΂���x���ړ�
	me_.GetPos().x += me_.GetSpeed().x;
	// y���ړ�(�ެ���)
	me_.GetPos().y += jumpSp_;
	// �ڒn���Ă���ꍇ
	if (me_.OnFloor())
	{
		// �ެ��߽�߰�ނ�0��
		jumpSp_ = 0;
		// �ެ��ߏ����x��0��
		jumpFirstSp_ = -45.0f;
		// �ެ��ߗ͂�0��
		jumpForce_ = 0.0f;
		// ���g�̼ެ����׸ނ�false��
		me_.IsJumping() = false;
		// Idle��Ұ��݂̕ύX
		me_.ChangeAnimation("idle");
		// ���g�̏�Ԃ�Observe��Ԃɂ���
		updater_ = &BigboyAI::Observe;
	}
	return false;
}

bool BigboyAI::OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ��ݏI����
	if (me_.GetisAnimEnd())
	{
		// HP��0�ȉ��ɂȂ�����
		if (me_.GetHp() <= 0)
		{
			// death��Ұ��݂ɕύX
			me_.ChangeAnimation("death");
			// ���g�̏�Ԃ�Death�ɂ���
			updater_ = &BigboyAI::Death;
			return true;
		}
		else
		{
			// Idle��Ұ��݂ɕύX
			me_.ChangeAnimation("idle");
			// ���g�̏�Ԃ�Observe�ɕύX
			updater_ = &BigboyAI::Observe;
			return true;
		}
	}
	// 1����Ұ�ނ�H�������false�ɂ��Ȃ��ƈȍ~��Ұ�ނ��H���Ȃ��̂�false�ɂ���
	me_.GetOnDamaged() = false;

	return false;
}

bool BigboyAI::Death(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ��݂��I��������
	if (me_.GetisAnimEnd())
	{
		// ���g���폜
		me_.Delete();
		return true;
	}
	return false;
}

bool BigboyAI::LaserAttack(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ��ݏI����
	if (me_.GetisAnimEnd())
	{
		// Idle��Ұ��݂ɕύX����
		me_.ChangeAnimation("idle");
		// ���g�̏�Ԃ�Observe��Ԃɂ���
		updater_ = &BigboyAI::Observe;
	}

	return false;
}
