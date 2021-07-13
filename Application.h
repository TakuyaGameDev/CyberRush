#pragma once
#include <memory>
#include <vector>
#include "Player.h"
#include "Geometory.h"


class Viewport
{
public:
	Vector2I GetSize(void)const;
};

class SceneController;
struct Input;

class Application
{
public:
	static Application& Instance(void)
	{
		static Application sInstance;
		return sInstance;
	}
	Viewport viewport_;
	// �������֌W
	bool Initialize(void);
	// �I��
	void Exit(void);

	void Run(void);

	void Terminate(void);
	// ��ʻ��ގ擾
	const Viewport& GetViewport(void) const;
	// �ްѺ��۰װ�擾
	std::shared_ptr<Input>& GetInput(const PLAYER& player);
private:
	Application() = default;
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	// �ްѺ��۰װ
	std::vector<std::shared_ptr<Input>> input_;
	// ��ݺ��۰װ
	std::shared_ptr<SceneController> sceneCtl_;

	bool isExit_ = false;
};

