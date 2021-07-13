#include <vector>
#include <map>
#include <string>
#include <array>

#include "ActorType.h"
#include "ObjectType.h"
#include "Geometory.h"
#include "WeaponType.h"
#include "BulletType.h"

#define lpImage ImageManager::GetInstance()

using VecInt = std::vector<int>;

using ActionSet = std::map<std::string, std::pair<std::pair<int, int>, bool>>;

struct Resource
{
	// ������actorType�̖��O
	std::string typeName;
	ActionSet actionNameSet;

	// �ڰѐ�
	int frame = 0;
	// ��Ұ��݂𕶎���Ŕ���
	std::map<std::string, VecInt> imageHandle;
	// �摜������
	Vector2I divCount_;
	// �摜��������
	Vector2I divSize_;
};

struct WeaponResource
{
	std::string weaponName;
	std::map<std::string,VecInt> imageHandle_;
	ActionSet actionNameSet_;

	// �摜������
	Vector2I divCount_;
	// �摜��������
	Vector2I divSize_;
};

struct BulletResource
{
	std::string bulletName;
	std::map<std::string, VecInt> imageHandle_;
	ActionSet actionNameSet_;

	// �摜������
	Vector2I divCount_;
	// �摜��������
	Vector2I divSize_;
};

struct StageResource
{
	// �����̵�޼ު�����߂̖��O
	std::string typeName;
	// �摜�����
	int imageHandle = -1;

	Vector2I divSize;
	Vector2I divCount;
};

class ImageManager
{
public:

	static ImageManager& GetInstance(void)
	{
		static ImageManager s_instance;
		return s_instance;
	}
	// ������۰��
	bool Load(const ActorType& actor,
		std::string filepath, Vector2I divSize, Vector2I divCount);
	// �摜�̕���۰��
	bool LoadDiv(std::string name, Vector2I divSize, Vector2I divCount);
	// �����ǂݍ��݂������摜����ق̎擾
	const VecInt& GetDivID(std::string name);

	// �摜��۰��
	void Load(std::string fileName);
	// �ð�޵�޼ު�Ă�۰��
	bool LoadStage(const ObjectType& objType, std::string objName);
	bool LoadWeapon(const WeaponType& weaponType, std::string weaponName);
	bool LoadBullet(const BulletType& bulletType,std::string bulletName);
	const VecInt& GetID(const ActorType& actor, std::string filepath);
	const int& GetID(std::string fileName);

	// �ð�޵�޼ު��ID�̎擾
	const StageResource& GetStageResource(const ObjectType& objType)
	{
		return stageResources_[static_cast<int>(objType)];
	}
	// ؿ���̎擾
	const Resource& GetResource(const ActorType& actor)
	{
		return resources_[static_cast<int>(actor)];
	}
	// ؿ���̎擾
	const WeaponResource& GetWeaponResource(const WeaponType& weapon)
	{
		return weaponResources_[static_cast<int>(weapon)];
	}

	const BulletResource& GetBulletResource(const BulletType& bullet)
	{
		return bulletResources_[static_cast<int>(bullet)];
	}

	const ActionSet& GetActionSet(ActorType actor)
	{
		return resources_[static_cast<int>(actor)].actionNameSet;
	}
	const std::string& GetActorName(const ActorType& actor)
	{
		return resources_[static_cast<int>(actor)].typeName;
	}
private:
	ImageManager() = default;
	ImageManager(const ImageManager&) = delete;
	void operator=(const ImageManager&) = delete;

	// �o��l������ؿ�����
	std::array<Resource, static_cast<int>(ActorType::Max)> resources_;
	std::array<WeaponResource, static_cast<int>(WeaponType::Max)> weaponResources_;

	std::array<BulletResource, static_cast<int>(BulletType::Max)> bulletResources_;

	// �ð�޵�޼ު�Ă�ؿ�����
	std::array<StageResource,static_cast<int>(ObjectType::Max)> stageResources_;

	// �摜�����(1���G)
	std::map<std::string, int> handles_;
	// �摜�����(�����摜)
	std::map<std::string, VecInt> divHandles_;
	void SetUp(ActorType actor);
	// �ǂݍ��񂾉摜�𕪊����ēǂݍ���
	void LoadDerivationGraph(ActorType actor, int handle, Vector2I divSize, Vector2I divCount);
};

