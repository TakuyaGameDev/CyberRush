#include "PodAI.h"
#include "Enemy.h"
#include "ControlledPlayer.h"
#include "WeaponBase.h"

PodAI::PodAI(Enemy& enemy):
	AIBase(enemy)
{
	// ������
	Initialize();
}

PodAI::~PodAI()
{
}

void PodAI::Update(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ�ނ�H����Ă�����
	if (me_.GetOnDamaged())
	{
		// hit��Ұ��݂ɕύX
		me_.ChangeAnimation("hit");
		// ��Ԃ���Ұ�ސH�炢��ԂɕύX
		updater_ = &PodAI::OnDamaged;
	}
	// �����̍U����H�������
	if (me_.GetFriendlyFireFlag())
	{
		// �����Ɏ���
		me_.ChangeAnimation("death");
		// ��Ԃ�death�ɕύX
		updater_ = &PodAI::Death;
	}
	// ��Ԃ̍X�V
	(this->*updater_)(enemies);
}

bool PodAI::Walk(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��������ڲ԰�����E�ɂ�����
	if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
	{
		// ���]�׸ނ�true��
		// DrawRotaGraph�̑�7������X���]�׸ނƕR�Â��Ă���
		me_.GetisTurnFlag() = true;
		// �ړ���߰�ނ�����ۂ̽�߰�ނɐݒ�
		me_.GetSpeed().x = -2;
	}
	// ��������ڲ԰�������ɂ�����
	if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
	{
		// ���]�׸ނ�true��
		// DrawRotaGraph�̑�7������X���]�׸ނƕR�Â��Ă���
		me_.GetisTurnFlag() = false;
		// �ړ���߰�ނ�����ۂ̽�߰�ނɐݒ�
		me_.GetSpeed().x = 2;
	}
	// �߼޼�݈ړ�
	me_.GetPos().x += me_.GetSpeed().x;
	// �����ŋ����̌v�Z������āA�ϐ��ɓ���Ă���
	auto distance = me_.GetNearestPlayer()->GetPos().x - me_.GetPos().x;

	// Pod�ƃv���C���[�Ƃ̋�������苗���ɂȂ��
	// Pod�̓v���C���[��Z���ɍ��킹��s��������
	if (abs(distance) < 150)
	{
		me_.GetAIState() = AIState::ZArrange;
		updater_ = &PodAI::Z_Arrangement;
		return true;
	}
	// �����Ă���ۂɖ�����AICollider�ɓ���������
	if (IsHitFriends(enemies))
	{
		// ����Ӱ��݂ɑJ��
		me_.GetAIState() = AIState::Run;
		updater_ = &PodAI::Run;
		return true;
	}
}

bool PodAI::Z_Arrangement(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ���ɐi��
	if (me_.GetisTurnFlag())
	{
		me_.GetSpeed().x = -1;
	}
	// �E�ɐi��
	else
	{
		me_.GetSpeed().x = 1;
	}
	// ��ڲ԰��Z���߼޼�݂̎擾
	auto playerZ = me_.GetNearestPlayer()->GetZPos();
	// ��ڲ԰����O�ɑ��݂����
	if (me_.GetZPos() >= playerZ)
	{
		// ��O�Ɉړ�
		me_.GetZSpeed() = -1;
	}
	else
	{
		// ���Ɉړ�
		me_.GetZSpeed() = 1;
	}
	// Z���ړ�
	me_.GetZPos() += me_.GetZSpeed();
	// �����ɓ���������(�T�ɖ����������)
	if (IsHitFriends(enemies))
	{
		// AI��Ԃ�Run�ɂ���
		me_.GetAIState() = AIState::Run;
		// ���g�̏�Ԃ�Run�ɕύX
		updater_ = &PodAI::Run;
		return true;
	}
	// Z������ڲ԰��Z���ƈ�v������
	// 3�Ƃ����̂͂�����x��ڲ԰�����F�\�Ȕ͈͂��L���Ă�̂�
	if (abs(me_.GetZPos() - playerZ) <= 3)
	{
		// �U��������Ұ��݂ɕύX
		me_.ChangeAnimation("attack_prepare");
		// AI��Ԃ�Attack�ɕύX
		me_.GetAIState() = AIState::Attack;
		// ���g�̏�Ԃ�Attack�ɕύX
		updater_ = &PodAI::Attack;
		return true;
	}
}

bool PodAI::Attack(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ��ݏI����
	if (me_.GetisAnimEnd())
	{
		// �C����Ұ��݂ɕύX
		me_.ChangeAnimation("attack_release");
	}
	// �C����Ұ��݂̏ꍇ
	if (me_.GetCurrentAnimation() == "attack_release")
	{
		// �e�����˂���ۂ̉ΉԱ�Ұ��݂��Đ�
		me_.AddMuzzleFlashAnimationCount(0.1f);
		// �ΉԱ�Ұ��݂��ő�l�ɂȂ�����
		if (me_.GetmuzzleFlashAnimationCount() >= 5.0f)
		{
			// AI�̓����蔻����ēx�\�ɂ���
			me_.GetisHitAICollider() = false;
			// �ΉԱ�Ұ��ݶ������0������
			me_.GetmuzzleFlashAnimationCount() = 0.0f;
			// walk��Ұ��݂ɕύX
			me_.ChangeAnimation("walk");
			// AI��Ԃ�Walk�ɕύX
			me_.GetAIState() = AIState::Walk;
			// ���g�̏�Ԃ�walk�ɕύX
			updater_ = &PodAI::Walk;
			return true;
		}
	}
	else
	{
		// ���Ɉړ�
		// ��ڲ԰�������̍��ɑ��݂�����
		if (me_.GetPos().x > me_.GetNearestPlayer()->GetPos().x)
		{
			me_.GetisTurnFlag() = true;
			me_.GetSpeed().x = -2;
		}
		// �E�Ɉړ�
		// ��ڲ԰�������̉E�ɑ��݂�����
		if (me_.GetPos().x < me_.GetNearestPlayer()->GetPos().x)
		{
			me_.GetisTurnFlag() = false;
			me_.GetSpeed().x = 2;
		}
	}
}

bool PodAI::OnDamaged(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ��ݏI����
	if (me_.GetisAnimEnd())
	{
		// HP��0�ȉ�
		if (me_.GetHp() <= 0)
		{
			// ���ʱ�Ұ��݂ɕύX
			me_.ChangeAnimation("death");
			// ���g�̏�Ԃ�death�ɕύX
			updater_ = &PodAI::Death;
			return true;
		}
		else
		{
			// �܂�HP��0�ȉ��łȂ����Walk��Ұ��݂ɕύX
			me_.ChangeAnimation("walk");
			// ���g�̏�Ԃ�Walk�ɕύX
			updater_ = &PodAI::Walk;
			return true;
		}
	}
	// 1����Ұ�ނ�H�������false�ɂ��Ȃ��ƈȍ~��Ұ�ނ��H���Ȃ��̂�false�ɂ���
	me_.GetOnDamaged() = false;

	return false;
}

bool PodAI::Run(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��ڲ԰�̌���
	auto playerIsTurnLeft = me_.GetNearestPlayer()->GetisTurnFlag();
	// ������ڲ԰���E�������Ă������ڲ԰�̍����ɍs���܂Œ��i
	if (!playerIsTurnLeft)
	{
		me_.GetSpeed().x = -2;
	}
	// ������ڲ԰�����������Ă������ڲ԰�̉E���ɍs���܂Œ��i
	else
	{
		me_.GetSpeed().x = 2;
	}

	// ���i������
	me_.GetPos().x += me_.GetSpeed().x;

	// ��ڲ԰���߼޼��
	auto playerPos = me_.GetNearestPlayer()->GetPos();

	if (!playerIsTurnLeft)
	{
		if (me_.GetPos().x <= playerPos.x - 100)
		{
			// AI��Ԃ�Attack�ɕύX
			me_.GetAIState() = AIState::Attack;
			// �U��������Ұ��݂ɕύX
			me_.ChangeAnimation("attack_prepare");
			// ���g�̍s����Ԃ�Attack�ɕύX
			updater_ = &PodAI::Attack;
			return true;
		}
	}
	else
	{
		if (me_.GetPos().x >= playerPos.x + 100)
		{
			// AI��Ԃ�Attack�ɕύX
			me_.GetAIState() = AIState::Attack;
			// �U��������Ұ��݂ɕύX
			me_.ChangeAnimation("attack_prepare");
			// ���g�̍s����Ԃ�Attack�ɕύX
			updater_ = &PodAI::Attack;
			return true;
		}
	}

	return false;
}

bool PodAI::Death(std::list<std::shared_ptr<Enemy>>& enemies)
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

void PodAI::Initialize(void)
{
	// �܂��ŏ��̏��Walk�ɂ���
	updater_ = &PodAI::Walk;
}