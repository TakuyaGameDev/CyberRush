#include <cmath>
#include "Collision.h"
#include "Enemy.h"
#include "Floor.h"
#include "ControlledPlayer.h"
#include "WeaponBase.h"
#include "ExoskeletonAI.h"

ExoskeletonAI::ExoskeletonAI(Enemy& enemy):
	AIBase(enemy)
{
	// ������
	Initialize();
}

ExoskeletonAI::~ExoskeletonAI()
{
}

void ExoskeletonAI::Update(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��ڲ԰��T���o�����Ă����Z
	// ���̒l�����ɂȂ�ƃv���C���[�Ɍ������Ē��i
	searchFrame++;
	// ̨���ނƂ̓����蔻��
	if (BoxOutCollision()(Vector2I(me_.GetPos().x,me_.GetZPos()),
		Vector2I(0,-320),Vector2I(floorX,floorZ)))
	{
		// �n�`�ɓ������Ă�����death��Ұ��݂ɕύX
		me_.ChangeAnimation("death");
		// ���g��death��ԂɕύX
		updater_ = &ExoskeletonAI::Death;
	}
	// �~�̓����蔻��
	if (CircleCollision()(me_.GetType(),
		me_.SearchNearestPlayer()->GetPos() - me_.GetPos(),
		me_.GetSize() + me_.GetNearestPlayer()->GetSize(),
		me_.GetZPos() - me_.GetNearestPlayer()->GetZPos()))
	{
		// death��Ұ��݂̕ύX
		me_.ChangeAnimation("death");
		// ��ڲ԰����Ұ�ނ�H������׸ނ�true�ɂ���
		// ��ڲ԰����Ұ�ނ�^����
		me_.GetNearestPlayer()->GetOnDamaged() = true;
		// ���g�̏�Ԃ�death�ɕύX
		updater_ = &ExoskeletonAI::Death;
	}
	// ̨���ނɑ��݂���G����
	for (auto enemy : enemies)
	{
		// ���݉񂵂Ă���G�������ł͂����Ȃ��̂ł��̂悤�ɋL�q
		if (me_.GetId() != enemy->GetId())
		{
			// �����Ƃ̓����蔻��
			if (CircleCollision()(enemy->GetType(),
				enemy->GetPos() - me_.GetPos(), enemy->GetSize() + me_.GetSize(),
				enemy->GetZPos() - me_.GetZPos()))
			{
				// �����ւ̍U�������������׸ނ�true��
				enemy->GetFriendlyFireFlag() = true;
				// death��Ұ��݂ɕύX
				me_.ChangeAnimation("death");
				// ���g�̏�Ԃ�death�ɕύX
				updater_ = &ExoskeletonAI::Death;
			}
		}
	}
	// �摜�̕����ύX
	// ������
	if (rad >= 0)
	{
		me_.GetisTurnFlag() = false;
	}
	// �E����
	else
	{
		me_.GetisTurnFlag() = true;
	}
	(this->*updater_)(enemies);
}

bool ExoskeletonAI::Run(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// �ڒn���Ă�����
	if (me_.OnFloor() == true)
	{
		// ��ڲ԰�����m����l��59�ɂȂ� = ��ڲ԰�ɒ��i����
		if ((searchFrame / 2) % 60 == 59)
		{
			player_pos = Vector2I(me_.GetNearestPlayer()->GetPos().x,
								  me_.GetNearestPlayer()->GetZPos());
			rad = atan2(player_pos.y - me_.GetZPos(),
						player_pos.x - me_.GetPos().x);
		}
		me_.GetPos().x += 5 * cos(rad);
		me_.GetZPos() += 5 * sin(rad);
	}
	// ��ڲ԰�����m����l��59�ȉ�
	// ���i�͂��Ȃ��ŏ����ڲ԰���߼޼�݂�����
	// ���i����܂���ڲ԰���߼޼�݂��X�V��������
	else
	{
		player_pos = Vector2I(me_.GetNearestPlayer()->GetPos().x,
							  me_.GetNearestPlayer()->GetZPos());
			rad = atan2(player_pos.y - me_.GetZPos(),
						player_pos.x - me_.GetPos().x);
	}

	return false;
}

bool ExoskeletonAI::Death(std::list<std::shared_ptr<Enemy>>& enemies)
{
	// ��Ұ��ݏI����
	if (me_.GetisAnimEnd())
	{
		// ���g���폜
		me_.Delete();
		return true;
	}
	return false;
}

void ExoskeletonAI::Initialize(void)
{
	// ��ڲ԰�����m���鶳��
	searchFrame = 0;
	// ������ԂƂ���Run�ɐݒ�
	updater_ = &ExoskeletonAI::Run;
}
