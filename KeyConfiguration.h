#pragma once
#include <type_traits>

// ���݂̎��
enum class KeyConfiguration
{
	Right,
	Left,
	Up,
	Down,
	// �U������
	Fire,
	// ����ύX����
	ChangeWeapon,
	// �۰�ރ{�^��
	Reload,
	// �ެ�������
	Jump,
	// ����
	Decision,
	// ؾ��
	Reset,
	Max
};

static KeyConfiguration begin(KeyConfiguration)
{
	return KeyConfiguration::Right;
}

static KeyConfiguration end(KeyConfiguration)
{
	return KeyConfiguration::Max;
}

static KeyConfiguration operator++(KeyConfiguration& config)
{
	return (config =
		KeyConfiguration(std::underlying_type<KeyConfiguration>::type(config) + 1));
}

static KeyConfiguration operator*(KeyConfiguration config)
{
	return config;
}

