#include "RPG.h"

// �֐����FGameOver		�Q�[���I�[�o�[�V�[��
// ���@���F
// �߂�l�F
void CRPG::GameOver()
{
	// �J�[�\���̔�\��
	m_pCCManager->ExchangeCursorVisible(FALSE);

	// ��ʂ̃N���A
	m_pCCManager->FillAll(L_BLUE, H_YELLOW, m_pCCManager->GetCSBInfo());

	// ���@�̏�����
	m_pPlayer->Initialize();

	// ==== �I�[�v�j���O�̕\�� ====
	COORD pos = {0, 0};
	COORD opPos = {20, 15};
	const int gameOverHeight = 2;

	// ��������̑��x
	int charaPopSpeed = 150;

	// �s����𑁂߂�s���̎w��
	int slowViewRowNumber = 1;
	std::string gameOverText[gameOverHeight] =
	{
		"���̌�V���E�J�c(�_���W�����U��)���̔ނ̎p���������̂͂��Ȃ�",
		"                                            Press Enter Key "
	};

	// ==== �Q�[���I�[�o�[�e�L�X�g�̕\�� ====
	// �N���W�b�g�̃��[���A�b�v���Ԍ���p
	float oneCycleDelay = 2.0f;
	DWORD startTime = timeGetTime();
	DWORD nowTime;
	float elapsedTime;

	// �J�E���^
	int count = 0;
	int countEnd = gameOverHeight;
	pos = opPos;
	
	Sleep(200);
	for(;;)
	{
		Sleep(1);
		// �L�[���͌��o
		m_pCCManager->Trigger();
		// �^�C�}�[�J�E���g
		nowTime = timeGetTime();
		elapsedTime = static_cast<float>(nowTime - startTime) / 1000.0f;
		if (elapsedTime >= oneCycleDelay || m_pCCManager->Press(KEY::RETURNKEY))
		{
			// �^�C�}�[���Z�b�g
			startTime = timeGetTime();
			// �\��
			if (count < slowViewRowNumber)
			{
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, gameOverText[count], NOWAIT, charaPopSpeed);
			}
			else
			{
				m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, gameOverText[count], NOWAIT, DONOTDELAY);
				oneCycleDelay = 0.5f;
			}
			pos.Y++;
			// �I������
			if (++count >= countEnd)
			{
				break;
			}
		}
	}
	m_pCCManager->WriteTextCharacter(L_BLUE, H_YELLOW, pos, "", WAIT, DONOTDELAY);

	// �^�C�g���o�b�N
	m_GameScene = TITLE;
}
