#pragma once
#include "AIBase.h"
class ExoskeletonAI :
    public AIBase
{
public:
	ExoskeletonAI(Enemy& enemy);
	~ExoskeletonAI();
	// 更新関数
	void Update(std::list<std::shared_ptr<Enemy>>& enemies);

	// ﾌﾟﾚｲﾔｰの背後に回り込む
	bool Run(std::list<std::shared_ptr<Enemy>>& enemies);
	// 死ぬ
	bool Death(std::list<std::shared_ptr<Enemy>>& enemies);
private:
	// 初期化
	void Initialize(void);
	// 状態遷移関数ﾎﾟｲﾝﾀ
	bool(ExoskeletonAI::* updater_)(std::list<std::shared_ptr<Enemy>>&);
	// 接地した後、ﾌﾟﾚｲﾔｰの位置をｻｰﾁするﾌﾚｰﾑｶｳﾝﾄ
	// 60ﾌﾚｰﾑの間でﾌﾟﾚｲﾔｰをｻｰﾁし続ける60ﾌﾚｰﾑを超えたらﾌﾟﾚｲﾔｰに直進
	int searchFrame;
	// ﾗｼﾞｱﾝ角度
	float rad;
	// ﾌﾟﾚｲﾔｰのﾎﾟｼﾞｼｮﾝ
	Vector2I player_pos;
};

