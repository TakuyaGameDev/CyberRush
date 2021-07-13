#include <DxLib.h>
#include "Application.h"
#include "PlayScene.h"
#include "ImageManager.h"
#include "Input.h"
#include "PLAYER.h"
#include "ControlledPlayer.h"
#include "Floor.h"
#include "WeaponBase.h"
#include "Spawner.h"
#include "Enemy.h"
#include "BackGround.h"
#include "AICollider.h"
#include "EnemyAIManager.h"
#include "PodAI.h"
#include "EnemyBullet.h"
#include "SoundManager.h"
#include "Item.h"
#include "EffectManager.h"


PlayScene::PlayScene(SceneController& sCon):
	BaseScene(sCon)
{
	Initialize();
}

PlayScene::~PlayScene()
{
}

void PlayScene::UpDate(const std::vector<std::shared_ptr<Input>>& input)
{
	fps_.Update();
	auto inputData1_ = input[static_cast<int>(PLAYER::ONE)]->GetPeriData();

	// �G�S���|��
	if (!drawNextWaveFlag_)
	{
		if (defeatEnemyNum_ >= enemyMaxNuminWave_[static_cast<int>(wave_)])
		{
			drawNextWaveFlag_ = true;
			// wave�ύX�׸ނ�true
			changeVolFlag_ = true;
		}
	}
	lpEffect.UpDate();
	if (!drawNextWaveFlag_)
	{
		bgmVolume_+= 3;
		if (bgmVolume_ >= 255)
		{
			bgmVolume_ = 255;
		}
		existEnemyCount_ = enemyList_.size();
		if (frame_ % 100 == 0)
		{
			if (enemyCountinWave_[static_cast<int>(wave_)] <
				enemyMaxNuminWave_[static_cast<int>(wave_)])
			{
				if (existEnemyCount_ < enemyNum_display_[static_cast<int>(wave_)])
				{
					// �G�̕���
					spawner_->MakeClone(enemyList_, playerList_, wave_);
					if (enemyList_.back()->GetType() != ActorType::Exoskeleton)
					{
						enemyCountinWave_[static_cast<int>(wave_)]++;
					}
				}
				for (auto enemy : enemyList_)
				{
					lpS_Effect.GetEnemy(enemy);
				}
			}
		}
		for (auto enemy : enemyList_)
		{
			aiManager_->UpDate(enemy);

			// �G�̱�Ұ��݊֌W
			enemy->Action();
			// �G�̒e����ڲ԰�̓����蔻��
			enemy->CheckHitMyBulletToPlayer(enemyBullets_);

			auto type = enemy->GetType();

			switch (type)
			{
			case ActorType::Pod:
				if (enemy->GetCurrentAnimation() == "attack_release")
				{
					enemy->AddBullet(enemyBullets_);
				}
				else
				{
					enemy->ReadyToShot();
				}
				break;
			case ActorType::Exoskeleton:
				break;
			case ActorType::Spacenaut:
				if (enemy->GetCurrentAnimation() == "attack")
				{
					if (frame_ % 10)
					{
						enemy->AddBullet(enemyBullets_);
					}
					else
					{
						enemy->ReadyToShot();
					}
				}
				break;

			case ActorType::Bigboy:
				if (enemy->GetCurrentAnimation() == "attack")
				{
					if (enemy->GetisAnimEnd())
					{
						enemy->AddBullet(enemyBullets_);
					}
				}
				else
				{
					enemy->ReadyToShot();
				}

				if (!isShaking_)
				{
					if (existEnemyCount_ < enemyNum_display_[static_cast<int>(wave_)] + 1)
					{
						createEnemyFlag_ = true;
					}
					else
					{
						createEnemyFlag_ = false;
					}

					if (enemy->OnFloor())
					{
						// �G�̕���
						spawner_->MakeClone(enemyList_, playerList_, wave_,createEnemyFlag_);
						lpSound.Play("onFloor",DX_PLAYTYPE_BACK);
						isShaking_ = true;
						shakeEffect_ = EFFECT_TYPE::shake;
					}
				}
				else
				{
					if (enemy->IsJumping())
					{
						isShaking_ = false;
					}
				}
				break;
			default:
				break;
			}
			// �G�����񂾂���ڲ԰���G��|�����������Z
			if (enemy->GetType() != ActorType::Exoskeleton)
			{
				if (wave_ < Wave::ThirdWave)
				{
					if (enemy->GetDeleteFlag())
					{
						defeatEnemyNum_++;
						existEnemyCount_--;
						if (GetRand(droppingRate_) == 1 ||
							GetRand(droppingRate_) == 2)
						{
							DropItem(enemy->GetPos(), enemy->GetZPos());
						}
					}
				}
				else
				{
					if (enemy->GetDeleteFlag())
					{
						if (enemy->GetType() == ActorType::Bigboy)
						{
							defeatEnemyNum_++;
							existEnemyCount_--;
						}
						if (GetRand(droppingRate_) == 1 ||
							GetRand(droppingRate_) == 2)
						{
							DropItem(enemy->GetPos(), enemy->GetZPos());
						}
					}
				}
			}
		}

		for (auto b : enemyBullets_)
		{
			b->UpDate();
		}

		if (isShaking_)
		{
			shakeTime_ += 0.3f;
			if (shakeTime_ >= 6.0f)
			{
				shakeEffect_ = EFFECT_TYPE::non;
				shakeTime_ = 0.0f;
			}
		}
		else
		{
			shakeEffect_ = EFFECT_TYPE::non;
			shakeTime_ = 0.0f;
		}

		// effectType��ύX���邱�Ƃŗh�炷�����\
		lpS_Effect.UpDate(shakeEffect_,5);

		fps_.Wait();
		frame_++;
	}
	else
	{
		if (changeVolFlag_)
		{
			bgmVolume_ -= 0.4f;
			if (bgmVolume_ <= 0)
			{
				bgmVolume_ = 0;
				lpSound.Stop("bgm_wave" + std::to_string(static_cast<int>(wave_)));
				changeVolFlag_ = false;
				changeWaveFlag_ = true;
			}
		}
		waveStringPos_.x += 3;
		if (waveStringPos_.x >= 350)
		{
			waveStringPos_.x = 350;
			waveNumExRate_ += 0.05f;
			if (waveNumExRate_ >= 1.0f)
			{
				waveNumExRate_ = 1.0f;
				waitFrame_ += 0.01f;
				if (waitFrame_ >= 10.0f)
				{
					waitFrame_ = 0.0f;
					waveNumExRate_ = 0.0f;
					waveStringPos_.x = -120;
					// �`���׸ނ�false
					drawNextWaveFlag_ = false;
					// BGM��ڲ�׸ނ�true
					isPlayBGM_ = true;
				}
			}
		}
	}
	if (changeWaveFlag_)
	{
		wave_ = wave_ + 1;
		ChangeWeather();
		changeWaveFlag_ = false;
	}

	lpSound.ChangeVolume("bgm_wave" + std::to_string(static_cast<int>(wave_+1)),
		bgmVolume_);

	if (isPlayBGM_)
	{
		defeatEnemyNum_ = 0;
		changeWaveFlag_ = false;
		isNextWave_ = false;
		isPlayBGM_ = false;
		goNextWave_ = false;
		lpSound.Play("bgm_wave" + std::to_string(static_cast<int>(wave_+1)), DX_PLAYTYPE_LOOP);
		isPlayBGM_ = false;
	}

	for (auto item : itemList_)
	{
		item->UpDate();
	}

	if (shakeEffect_ != EFFECT_TYPE::shake)
	{
		for (auto player : playerList_)
		{
			player->UpDate();
			player->GetCurrentWeapon()->UpDate();
		}
	}

	enemyList_.remove_if
	([](std::shared_ptr<Enemy>& enemy) {return enemy->GetDeleteFlag(); });

	itemList_.remove_if
	([](std::shared_ptr<Item>& item) {return item->GetDeleteFlag(); });

	// �G�̒e�̏���
	enemyBullets_.erase(std::remove_if(enemyBullets_.begin(),
		enemyBullets_.end(),
		[&](std::shared_ptr<BulletBase>& bullet) {
			return bullet->GetDeleteFlag();
		}), enemyBullets_.end());

	if (currentWeather_ == "Thundersky")
	{
		weatherAnimationCount_ += 0.05f;
		if (weatherAnimationCount_ >= 5)
		{
			weatherAnimationCount_ = 0.0f;
		}
	}
	else
	{
		for (auto& sky : skyPos_)
		{
			sky.x++;
			if (sky.x >= 1200)
			{
				sky.x = -400;
			}
		}
	}
}

void PlayScene::Draw(void)
{
	auto& app = Application::Instance();
	auto scr = app.GetViewport().GetSize();

	ClearDrawScreen();

	DrawRotaGraph(skyPos_[0].x,skyPos_[0].y,
		1.0f, 0.0f,
		lpImage.GetDivID(currentWeather_)[weatherAnimationCount_], true);
	if (currentWeather_ == "Normalsky")
	{
		DrawRotaGraph(skyPos_[1].x, skyPos_[1].y,
			1.0f, 0.0f,
			lpImage.GetDivID(currentWeather_)[weatherAnimationCount_], true);
	}


	for (auto obj : objList_)
	{
		obj->Draw();
	}

	for (auto b : enemyBullets_)
	{
		b->Draw();
	}

	for (auto player : playerList_)
	{
		DrawPlayer(player);
	}


	for (auto enemy : enemyList_)
	{
		enemy->Draw_();
		//enemy->GetAICollider()->Draw();
	}

	for (auto item : itemList_)
	{
		item->Draw();
	}

	if (drawNextWaveFlag_)
	{
		DrawRotaGraph(waveStringPos_.x, waveStringPos_.y, 1.0f, 0.0f,
			lpImage.GetID("UI/wave"), true, false);

		DrawRotaGraph(waveNumPos_.x,waveNumPos_.y, waveNumExRate_, 0.0f,
			lpImage.GetDivID("UI/wave_num")[static_cast<int>(wave_)], true, false);
		if (waveNumExRate_ >= 1.0f)
		{
			DrawRotaGraph(waveStringPos_.x, waveStringPos_.y + 60, 1.0f, 0.0f,
				lpImage.GetID("UI/untilnext"), true, false);
			DrawRotaGraph(waveNumPos_.x-80, waveNumPos_.y + 75, waveNumExRate_, 0.0f,
				lpImage.GetDivID("UI/number")[static_cast<int>(10 - waitFrame_)], true, false);
		}
	}
	//fps_.Draw();
	
	ScreenFlip();
}

void PlayScene::DrawPlayer(const std::shared_ptr<ControlledPlayer>& player)
{
	if (player->GetisTurnFlag())
	{
		zFlag_ = false;
	}
	else
	{
		zFlag_ = true;
	}

	if (zFlag_)
	{
		player->Draw_();
		player->GetCurrentWeapon()->Draw(player->GetisTurnFlag());
	}
	else
	{
		player->GetCurrentWeapon()->Draw(player->GetisTurnFlag());
		player->Draw_();
	}
}

void PlayScene::Initialize(void)
{
	auto& app = Application::Instance();

	auto scr = app.GetViewport().GetSize();
	auto& imageMng = ImageManager::GetInstance();

	wave_ = Wave::FirstWave;

	shakeEffect_ = EFFECT_TYPE::non;
	shakeTime_ = 0.0f;
	isShaking_ = false;

	enemyCountinWave_ = { 0,0,0 };
	enemyMaxNuminWave_ = { 30,30,1 };
	enemyNum_display_ = {5,6,2};
	existEnemyCount_ = 0;

	waveStringPos_ = { -120,50 };
	waveNumPos_ = { 530,50 };
	skyPos_[0] = { scr.x / 2, scr.y / 2 - 106 };
	skyPos_[1] = { -scr.x / 2, scr.y / 2 - 106 };
	waveNumExRate_ = 0.0f;
	waitFrame_ = 0.0f;
	bgmVolume_ = 0.0f;
	defeatEnemyNum_ = 0;
	droppingRate_ = 5;

	dropItemType_ = BulletType::Max;

	isPlayBGM_ = false;
	isNextWave_ = false;
	changeVolFlag_ = false;
	goNextWave_ = false;
	drawNextWaveFlag_ = true;
	createEnemyFlag_ = false;
	currentWeather_ = "Normalsky";
	weatherAnimationCount_ = 0.0f;

	changeWaveFlag_ = false;

	imageMng.LoadDiv("Normalsky", Vector2I(800, 387), Vector2I(2, 3));
	imageMng.LoadDiv("Thundersky", Vector2I(800, 387), Vector2I(2, 3));
	imageMng.LoadDiv("UI/wave_num", Vector2I(60, 60), Vector2I(3, 1));
	imageMng.LoadDiv("UI/number", Vector2I(22, 40), Vector2I(10, 1));
	imageMng.LoadDiv("Item/items", Vector2I(32, 32), Vector2I(3, 1));

	imageMng.Load("UI/enemy1_UI2");
	imageMng.Load("UI/wave");
	imageMng.Load("UI/untilnext");
	imageMng.Load("UI/bullets");

	lpSound.Load("pistol/fire", true);
	lpSound.Load("sub_machinegun/fire", true);
	lpSound.Load("shotgun/fire", true);

	lpSound.Load("Pod/fire", true);
	lpSound.Load("Spacenaut/fire", true);
	lpSound.Load("Bigboy/fire", true);


	lpSound.Load("damage", true);

	lpSound.Load("pistol/get", true);
	lpSound.Load("sub_machinegun/get", true);
	lpSound.Load("shotgun/get", true);

	lpSound.Load("explosion", true);
	lpSound.Load("bgm", true);
	lpSound.Load("onFloor", true);

	for (int i = 0; i < static_cast<int>(Wave::Max); i++)
	{
		lpSound.Load("bgm_wave" + std::to_string(i + 1), true);
	}

	// �G�S�̂�AI�̊Ǘ��׽
	aiManager_ = std::make_unique<EnemyAIManager>(enemyList_);

	spawner_ = std::make_shared<Spawner>();
	frame_ = 0;
	// ��ڲ԰�ƓG�̎�ނ̖��O
	actorName_ =
	{
		"Player",
		"Pod",
		"Exoskeleton",
		"Spacenaut",
		"Bigboy"
	};
	zFlag_ = false;

	playerList_.emplace_back(std::make_shared<ControlledPlayer>(Vector2I(200, 0), 0, ActorType::Player,itemList_));


	AddObject(std::make_shared<BackGround>());
	AddObject(std::make_shared<Floor>
		(Vector2I(app.GetViewport().GetSize().x / 2, app.GetViewport().GetSize().y / 2 + 150),
			ObjectType::Floor));

	// �w�i���߼޼�ݾ��
	// �ݽ�׸����߼޼�ݾ�Ă�����������ł����ق�����ׂ�ǉ�����Ƃ��ɂ���Ȃ肢���̂�
	for (auto obj : objList_)
	{
		if (obj->GetType() == ObjectType::BackGround)
		{
			obj->SetPos(Vector2I(app.GetViewport().GetSize().x / 2, app.GetViewport().GetSize().y / 2));
		}
	}

	for (auto player : playerList_)
	{
		lpS_Effect.GetPlayer(player);
	}
}

void PlayScene::AddObject(ObjectPtr object)
{
	objList_.push_back(object);
}

void PlayScene::DropItem(const Vector2I& pos, const int& z)
{
	dropItemType_ = (BulletType)GetRand(static_cast<int>(BulletType::MachineBullet));
	itemList_.emplace_back(std::make_shared<Item>(pos, z, dropItemType_));
}

void PlayScene::ChangeWeather(void)
{
	auto& app = Application::Instance();
	switch (wave_)
	{
	case Wave::FirstWave:
	case Wave::SecondWave:
		currentWeather_ = "Normalsky";

		break;
	case Wave::ThirdWave:
		// �摜���߼޼�݂̏�����
		skyPos_[0] = { app.GetViewport().GetSize().x/2,
			app.GetViewport().GetSize().y / 2 - 106 };
		skyPos_[1] = { -app.GetViewport().GetSize().x / 2,
			app.GetViewport().GetSize().y / 2 - 106 };
		currentWeather_ = "Thundersky";
		lpSound.Play("thunder", DX_PLAYTYPE_BACK);
		break;
	case Wave::Max:
		break;
	default:
		break;
	}
}
