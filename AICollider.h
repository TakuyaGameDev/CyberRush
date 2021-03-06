#pragma once
#include "Geometory.h"
#include "ActorType.h"

constexpr int size = 100;

// 敵のAI実現のためのｺﾗｲﾀﾞｰｸﾗｽ
// 敵同士がぶつかればAI起動
class Enemy;

class AICollider
{
public:
	AICollider();
	~AICollider();
	// ﾎﾟｼﾞｼｮﾝｱｯﾌﾟﾃﾞｰﾄ
	void SetPos(const Vector2I& pos,const int& z);
	// ｺﾗｲﾀﾞｰﾎﾞｯｸｽ描画
	void Draw(void);

	// ﾎﾟｼﾞｼｮﾝ取得関数
	const Vector2I& GetPos(void)
	{
		return pos_;
	}
	// Z軸のﾎﾟｼﾞｼｮﾝ取得
	const int& GetZPos(void)
	{
		return z_;
	}

private:
	// ﾎﾟｼﾞｼｮﾝ
	Vector2I pos_;
	// Z軸のﾎﾟｼﾞｼｮﾝ	
	int z_;

};

