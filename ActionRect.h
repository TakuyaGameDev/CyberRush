#pragma once
#include "Geometory.h"
#include <string>
// ̧�ق���ǂݍ��񂾋�`�ް����i�[���Ď��ۂ̓����蔻�蓙�Ŏg�p�����`������ꍞ�޸׽

// ��`������
// �U����`�Ȃ̂����炢��`�Ȃ̂�
enum class RectType
{
	Attack,
	Damage,
	Max
};

static RectType begin(RectType)
{
	return RectType::Attack;
}
static RectType end(RectType)
{
	return RectType::Max;
}
static RectType operator++(RectType& type)
{
	return (type = (RectType)(std::underlying_type<RectType>::type(type) + 1));
}
static RectType operator*(RectType& type)
{
	return type;
}

struct RectData
{
	int frame_;
	Vector2I begin_;
	Vector2I end_;
	Vector2I size_;
	RectType type_;
	std::string actionName_;

	RectData() :begin_({ 0,0 }), end_({ 0,0 }),
		size_({ 0,0 }), type_(RectType::Max), frame_(0), actionName_("") {};

	RectData(Vector2I begin, Vector2I end, Vector2I size, RectType type, int frame, std::string actionName) :
		begin_(begin), end_(end), size_(size), type_(type), frame_(frame), actionName_(actionName) {};
};

class ActionRect
{
public:
	ActionRect();
	~ActionRect();

	// ��`�ް��̎擾
	RectData& GetData(void)
	{
		return data_;
	}

	const Vector2I& GetCenter(void)
	{
		return center_;
	}
	// �`��
	void Draw(const Vector2I& player);

private:
	// ��`���߼޼�݂⻲�ނ̏��
	RectData data_;
	// ��`�̒��S�߼޼��
	Vector2I center_;
	// ��`�̐F
	unsigned int boxcol_;
};



