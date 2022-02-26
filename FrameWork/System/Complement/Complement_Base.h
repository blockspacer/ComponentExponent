#pragma once


class Complement_Base
{
protected:
	float start;//�J�n�l
	float end;//�I���l
	float playTime;//�J�n����I���܂ł̎���
	float* current;//���ݒl�i�߂�l�����Ȃ��̂Ń|�C���^�[�B�j

	double startTime;//�J�n���̃~���b
	double currentTime;//���ݎ���
	class Time* cacheTime;//Time�L���b�V��

	bool isLoopRevers;//�I�����ɏI������X�^�[�g���邩�ǂ����BTRUE�ŏI������X�^�[�g�B
	bool isLoop;//���[�v

public:

	Complement_Base(float _start, float _end, float _playTime, float* _out, bool _isLoop = false, bool _isLoopReverse = false);

	virtual ~Complement_Base() {};

	inline void SetIsLoopReverse(bool _set) { isLoopRevers = _set; }
	inline void SetIsLoopOff() { isLoop = false; }//���[�v����
	bool GetIsFinished();//�⊮���I�����Ă��邩���擾

	void TimeUpdate();//�o�ߎ��Ԃ��A�b�v�f�[�g

	virtual void Update() = 0;
};