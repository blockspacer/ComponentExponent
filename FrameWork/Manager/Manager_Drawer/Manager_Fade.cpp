//=============================================================================
//
//  �t�F�[�h�}�l�[�W���[ [Manager_Fade.cpp]
//  Date   : 2021/11/16
//
//=============================================================================

#include "../../System/MainWindow.h"
#include "Manager_Fade.h"
#include "../Manager_Game.h"
#include "../../Renderer/Renderer_Polygon.h"
#include "../Manager_Shader.h"
#include "../Manager_Texture.h"
#include <time.h>

void Manager_Fade::Init()
{
	m_ShaderType = Manager_Shader::SHADER_UNLIT;//�f�t�H���g�̓A�����b�g�V�F�[�_�[

	m_FadeState = FADE_NONE;//�t�F�[�h���Ă��Ȃ�

	m_FadeTime  = 0.0f;
	m_StartTime = 0.0f;

	m_Color = Color(0.0f, 0.0f, 0.0f, 0.0f);//�F��RGB���ׂ�0.0f�ō�

	if (Manager_Texture* manager_Texture = Manager_Texture::Instance())
	{//�e�N�X�`���[�}�l�[�W���[�L���b�V��
		manager_Texture->LoadTexture("Fade", "../ROM/2D/Test/FadeTexture.png");
		m_Texture = manager_Texture->GetTexture("Fade");
	}
}

void Manager_Fade::Update()
{
	if (m_FadeState == FADE_NONE)
	{//��t�F�[�h���̓��^�[��
		return;
	}

	float Time = clock() - m_StartTime;
	Time /= 1000.0f;

	switch (m_FadeState)
	{
	case FADE_IN:

		m_Color.a = 1.0f - Time / m_FadeTime;

		if (Time > m_FadeTime)
		{//�t�F�[�h�C���I��

			m_Color.a = 0.0f;
			m_FadeState = FADE_NONE;

		}
		break;

	case FADE_OUT:

		m_Color.a = Time / m_FadeTime;

		if (Time > m_FadeTime)
		{//�t�F�[�h�A�E�g�I��

			m_Color.a = 1.0f;
			m_FadeState = FADE_NONE;
			//���x���ύX
			Manager_Game::Instance()->OpenLevel();
		}
		break;
	}

}

void Manager_Fade::Draw()
{
	if (m_FadeState == FADE_NONE)
	{
		return;
	}

	if (m_Texture) 
	{
		//�V�F�[�_�[�ݒ�
		Manager_Shader::Instance()->SetShader(m_ShaderType);

		// �e�N�X�`���ݒ�
		Renderer_DX11::Instance()->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		Int2 windowSize = MainWindow::Instance()->GetWindowSize();

		//�v���[�������_���[����`��
		Renderer_Polygon::Instance()->Draw2D(Vector2_Zero,Vector2{ static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) }, m_Color, false);
	}
}

void Manager_Fade::SetFade(State _fadeState, float _time)
{
	m_FadeState = _fadeState;
	m_FadeTime  = _time;
	m_StartTime = static_cast<float>(clock());

	if (_fadeState == FADE_IN)
	{
		m_Color.a = 1.0f;
	}
	else if (_fadeState == FADE_OUT)
	{
		m_Color.a = 0.0f;
	}
}