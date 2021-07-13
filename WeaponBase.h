#pragma once
#include <string>
#include <map>
#include <list>
#include <memory>
#include <vector>
#include "Geometory.h"
#include "WeaponType.h"
#include "BulletType.h"

// �s�X�g���̒e�̍ő僊���[�h��
#define PISTOLBULLETNUM 8
// �V���b�g�K���̒e�̍ő僊���[�h��
#define SHOTBULLETNUM 30
// �}�V���K���̒e�̍ő僊���[�h��
#define MACHINEBULLETNUM 300

class BulletBase;

class WeaponBase
{
public:
	WeaponBase();
	~WeaponBase();

	virtual void UpDate(void) = 0;
	virtual bool Initialize(void);
	virtual void UpDateAnimation(std::string animName) = 0;

	virtual void Draw(const bool& turnFlag);

	void SetPos(const Vector2I& pos,int z);
	void SetAnimation(std::string animName);

	const WeaponType& GetType(void)
	{
		return type_;
	}

	const std::string& GetWeaponName(void)
	{
		return name_;
	}

	// �e�e�̒ǉ�
	void AddBullet(void);

	std::vector<std::shared_ptr<BulletBase>>& GetBullets(void)
	{
		return bullets_;
	}

	const std::string& GetAnimation(void)
	{
		return currentAnimation_;
	}

	int& GetHavingBulletNum(void)
	{
		return havingBulletNum_;
	}

	const BulletType& GetcanSetBulletType(void)
	{
		return canSetBulletType_;
	}

private:

	Vector2I drawPos_;
	Vector2I muzzleFlashPos_;
protected:
	Vector2I pos_;
	int z_;
	WeaponType type_;
	// ���했���۰�މ\������
	BulletType canSetBulletType_;

	std::string name_;
	std::string muzzleFlashName_;
	Vector2I muzzleFlashSize_;
	Vector2I muzzleFlashDivCount_;
	float muzzleFlashAnimationCount_;

	std::string currentAnimation_;
	std::map<std::string,int> animMaxNum_;
	// �������o���Ă���e�̎��
	BulletType bulletType_;

	// ���������Ă���e�e�̒eؽ�
	std::vector<std::shared_ptr<BulletBase>> bullets_;

	// ��Ұ��݂��I�������׸�
	bool isAnimEnd_;
	float animationCount_;
	// �g�嗦
	double exRate_;
	// ��]��
	double rotRate_;
	// ���Ɍ����Ă�����true
	bool isTurnLeft_;

	bool isFire_;

	// �����Ă���e�e�̐�
	int havingBulletNum_;
};

