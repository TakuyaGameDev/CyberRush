#pragma once
#include <array>
#include <list>
#include <memory>
#include <unordered_map>

#include "BaseScene.h"
#include "ActorType.h"
#include "Fps.h"
#include "Wave.h"
#include "ScreenEffectMng.h"
#include "BulletType.h"

class ControlledPlayer;
class Enemy;
class BulletBase;
class Item;
class EnemyAIManager;
//class ShotBase;
//class Spawner;
class Object;
class Spawner;

using ObjectPtr = std::shared_ptr<Object>;
using ObjectList = std::list<ObjectPtr>;

class PlayScene :
	public BaseScene
{
public:
	PlayScene(SceneController& sCon);
	~PlayScene();

private:
	void UpDate(const std::vector<std::shared_ptr<Input>>& input);
	void Draw(void);
	void DrawPlayer(const std::shared_ptr<ControlledPlayer>& player);
	void Initialize(void);
	void AddObject(ObjectPtr object);
	// �G�����т��h���b�v���鏈��
	void DropItem(const Vector2I& pos,const int& z);

	void ChangeWeather(void);
	// 2��ڲ԰�ݽ�ݽ����p�׸�
	bool is2PCreate_;
	// ���s�׸�
	bool zFlag_;

	// �e��ڲ԰�A�G�̖��O
	std::array<std::string, static_cast<int>(ActorType::Max)> actorName_;

	std::vector<std::shared_ptr<ControlledPlayer>> playerList_;
	std::list<std::shared_ptr<Enemy>> enemyList_;
	std::unique_ptr<EnemyAIManager> aiManager_;
	// ����
	std::list<std::shared_ptr<Item>> itemList_;

	// �Ă��̒e(�G�����߂ɂ���ĕω�)
	std::vector<std::shared_ptr<BulletBase>> enemyBullets_;

	ObjectList objList_;
	// �ڰ�ڰ�
	Fps fps_;

	std::array<Vector2I, 2> skyPos_;

	int frame_;
	// ��߰Ű
	std::shared_ptr<Spawner> spawner_;

	// ������ڲ԰������ʒu�̓V��
	std::string currentWeather_;
	float weatherAnimationCount_;
	float thunderVol_;
	// �n�ʂ�h�炷�������ߕϐ�
	EFFECT_TYPE shakeEffect_;
	bool isShaking_;
	float shakeTime_;

	float bgmVolume_;

	// ������ڲ԰�����݂���wave��
	Wave wave_;
	Vector2I waveStringPos_;
	Vector2I waveNumPos_;
	float waitFrame_;
	float waveNumExRate_;

	bool drawNextWaveFlag_;
	bool changeWaveFlag_;
	bool isNextWave_;
	bool changeVolFlag_;
	bool goNextWave_;
	// ��ڲ԰���|�����G�̐�
	int defeatEnemyNum_;

	// 1Wave�ɑ΂���G�̐������i�[
	// �G�𐶐���������Z
	std::array<int, static_cast<int>(Wave::Max)> enemyCountinWave_;
	// 1Wave���̓G�̍ő吶����
	std::array<int, static_cast<int>(Wave::Max)> enemyMaxNuminWave_;

	bool isPlayBGM_;
	bool createEnemyFlag_;
	// �G���h���b�v�������т�����
	BulletType dropItemType_;
	// ���уh���b�v�̊m��
	int droppingRate_;
	// ��ʂɏo�����邱�Ƃ̂ł���G��
	std::array<int, static_cast<int>(Wave::Max)> enemyNum_display_;

	int existEnemyCount_;
};



