#pragma once
#include <string>
#include <map>
#include <array>
#include <vector>
#include <DxLib.h>
#include "PLAYER.h"
#include "KeyConfiguration.h"

// ���͋@��
enum class Peripheral
{
	Key,
	Pad,
	Max
};

// �������Ă���̂��A�O�ڰтɉ����Ă���̂�
enum class TrgFlag
{
	Now,
	Old,
	Max
};

// �����͂��ضް�׸�
using InputTrg
= std::array<bool, static_cast<int>(TrgFlag::Max)>;
// KeyConfiguration�𷰂ɂ���Trg�׸ނ����o��
using PeripheralInfo
= std::map<KeyConfiguration, InputTrg>;

struct Input
{
public:
	void operator()()
	{
		UpDate();
	}
	// �����̓��͏��̎擾
	const PeripheralInfo& GetPeriData(void)
	{
		return periData_;
	}
	// �����̓��͑��u�����߂̎擾
	virtual Peripheral GetType(void) = 0;

	virtual bool Setup(const PLAYER& playerNo) = 0;

	// �ڑ����Ă�����ڲ԰���擾
	int GetConnectedPlayerNum(void)
	{
		return GetJoypadNum();
	}

private:
	virtual void UpDate(void) = 0;

protected:
	PeripheralInfo periData_;
	PLAYER player_;
};
