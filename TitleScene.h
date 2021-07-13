#pragma once
#include <array>
#include "BaseScene.h"
#include "Geometory.h"

using ArrayInt = std::array<int, 2>;

// fade in out �̍ۂɎg�p
enum class Fade
{
	In,
	Out,
	Max
};

enum class Next
{
	Manual,
	Game,
	Max
};

class TitleScene :
	public BaseScene
{
public:
	TitleScene(SceneController& sCon);
	~TitleScene();

private:
	void UpDate(const std::vector<std::shared_ptr<Input>>& input);
	void Draw(void);
	void Initialize(void);

	// ���قł̕�������߼޼��
	Vector2I stringPos_;
	// ������̽�߰��(fadeIn,out�̍ۂɎg�p)
	Vector2I stringSp_[2];
	// �����߼޼��
	Vector2I arrowPos_;

	// ̪��޲݂�̪��ޱ�Ă̶���
	int fadeCnt_;
	int pushFadeCnt_;
	// ̪��޲݂Ʊ�Ă̐؂�ւ�
	Fade fade_;
	// ���̼�݂ɐ؂�ւ����׸�
	Next nextFlag_;
	// �������݂��������׸�
	bool isNext_;
	// start�̕������L���鶳��
	float extendCount_;
	// ��L�̶��Ă����Z���Ă����׸�
	bool extendFlag_;
	// start�̕��������񂾂񓧉߂����Ă����A���t�@�l
	float alphaCount_;
};

