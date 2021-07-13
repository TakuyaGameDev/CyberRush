#pragma once
#include <EffekseerForDXLib.h>
#include <string>
#include <map>
#include <list>

#include "Geometory.h"

// �̪�Ă��Ǘ�����׽
// �ݸ���ݸ׽

#define lpEffect EffectManager::GetInstance()

class EffectManager
{
public:
	static EffectManager& GetInstance(void)
	{
		static EffectManager s_Instance;
		return s_Instance;
	}
	// �X�V
	void UpDate(void);
	// �̪�Ă̕`��
	void Draw(void);
	// �S�Ă̴̪�Ă̍Đ����~�߂�
	bool StopAll(void);
	// �̪�Ă̍Đ�
	bool Play(std::string effectName, const Vector2I& pos);
	// effekseer�̏������֘A
	bool Init(const Vector2I& size);

	// �̪�Ă��Đ����؂ǂ������׸ގ擾
	const int& IsPlayingEffect(std::string effectName);
	// �̪�Ă�۰��
	void Load(std::string effectName);
private:
	EffectManager() = default;
	// ��߰�֎~
	EffectManager(const EffectManager&) = delete;
	void operator=(const EffectManager&) = delete;
	// ��ڲ����ق̎擾
	const int& GetHandle(std::string effectName);

	// �̪�������
	std::map<std::string, int> handles_;
	// ��ڲ����ق��i�[����ؽ�
	std::list<int> playList_;
};


