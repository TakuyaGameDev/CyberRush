#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>

#include "ActionRect.h"


// ��`����۰�ނ���ׂ̼ݸ���ݸ׽
using ActRect = std::vector<std::shared_ptr<ActionRect>>;
// 1�ڰт̒��g�̋�`���(��������̂�vector�ɂ��Ă���)
using FRect = std::vector<ActRect>;
// 1��Ұ��݂̒��g�̋�`���(pair(��Ұ��ݕ�����Ƃ��̌����ڰ�)�Ŕ��ʉ\�ɂ���)
using AnimationRect = std::map<std::string, FRect>;
// ��ڲ԰�̋�`�Ȃ̂��G�̋�`�Ȃ̂��̎���
using ActorRect = std::map<std::string, AnimationRect>;

class CollisionManager
{
public:
	static CollisionManager& GetInstance(void)
	{
		static CollisionManager s_instance;
		return s_instance;
	}

	void Load(AnimationRect& actorRect, const std::string& actorname);


private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	void operator=(const CollisionManager&) = delete;

	// ͯ�ް̧���߲��
	FILE* headerFile_;
	// ��`�ް��������Ă���̧���߲��
	FILE* file_;

};

