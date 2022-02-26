#pragma once


class Complement_Base
{
protected:
	float start;//開始値
	float end;//終了値
	float playTime;//開始から終了までの時間
	float* current;//現在値（戻り値を取れないのでポインター。）

	double startTime;//開始時のミリ秒
	double currentTime;//現在時間
	class Time* cacheTime;//Timeキャッシュ

	bool isLoopRevers;//終了時に終了からスタートするかどうか。TRUEで終了からスタート。
	bool isLoop;//ループ

public:

	Complement_Base(float _start, float _end, float _playTime, float* _out, bool _isLoop = false, bool _isLoopReverse = false);

	virtual ~Complement_Base() {};

	inline void SetIsLoopReverse(bool _set) { isLoopRevers = _set; }
	inline void SetIsLoopOff() { isLoop = false; }//ループ解除
	bool GetIsFinished();//補完が終了しているかを取得

	void TimeUpdate();//経過時間をアップデート

	virtual void Update() = 0;
};