#include <DxLib.h>
#include "CollisionManager.h"

void CollisionManager::Load(AnimationRect& actorRect, const std::string& actorname)
{
	for (auto rects = actorRect.begin(); rects != actorRect.end(); rects++)
	{
		// 1��Ұ��݂̖��O
		auto animname = rects->first;
		// ������1��Ұ��ݖ���ٰ�߂ŉ�
		// 1�ڰтɉ���`��������̂��̻��ޏ��
		// ͯ�ް����ǂݎ�������
		int size = 0;
		// �����ݓǂݍ���ł���̧�������
		int fileHandle = -1;

		// ͯ�ް̧�ٓǂݍ��݂����(1�ڰтɋ�`��������̂��̐��ǂݍ���)
		std::string fileName = "Data/h/" + actorname + "_" + animname + ".map";
		fileHandle = FileRead_open(fileName.c_str(), false);

		for (int i = 0; i < rects->second.size(); i++)
		{
			FileRead_seek(fileHandle, size_t(sizeof(int) * i), SEEK_SET);
			FileRead_read(&size, sizeof(int), fileHandle);
			for (int s = 0; s < size; s++)
			{
				rects->second[i].emplace_back(std::make_shared<ActionRect>());
			}
		}
		// ̧�ق����(1���)(ͯ�ް̧��)
		FileRead_close(fileHandle);

		// ��`�ް��̎��ް���ǂݍ���
		fileName = "Data/" + actorname + "_" + animname + ".map";
		fileHandle = FileRead_open(fileName.c_str(), false);
		int size1 = rects->second.size();

		for (int d1 = 0; d1 < size1; d1++)
		{
			int size2 = rects->second[d1].size();
			for (int d2 = 0; d2 < size2; d2++)
			{
				FileRead_seek(fileHandle, size_t(sizeof(RectData) * d2), SEEK_SET);
				FileRead_read(&rects->second[d1][d2]->GetData(), sizeof(RectData), fileHandle);
			}
		}

		// ̧�ق����(2���)(���ް�)
		FileRead_close(fileHandle);
	}
}
