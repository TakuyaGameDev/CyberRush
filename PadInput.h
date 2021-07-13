#pragma once
#include <array>
#include <map>
#include "Input.h"


class PadInput :
	public Input
{
public:
	Peripheral GetType(void);
	bool Setup(const PLAYER& playerNo);

	void UpDate(void);
private:
	// �߯�ނ̓��͏��
	int padInfo_;
	std::map<KeyConfiguration, int> inputTable_;

};

