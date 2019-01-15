#pragma once

class QTimer
{
public:

	QTimer(TIMER_TIMEUINT timeUnit);

	void		setDestruction();

	double GetTotalTime() const;
	
	double GetInterval() const;

	void SetTimeUnit(TIMER_TIMEUINT timeUnit = TIMER_TIMEUNIT_MILLISECOND);

	void NextTick();


	UINT GetFPS()const;

	void Pause();

	void Continue();

	//������������
	void ResetAll();

	void ResetTotalTime();


private:
	//��ʱ��
	double		mTotalTime;
	//INTERVAL ��֡��ʱ����
	double		mDeltaTime;
	//ÿ��COUNTռ�˶��ٺ���
	double		mMilliSecondsPerCount;
	//how many ticks are in current second
	UINT		mCurrentSecondTickCount;
	//a rounded total time
	UINT		mCurrentSecondInteger;

	UINT		mFPS;

	TIMER_TIMEUINT	m_TimeUnit;

	bool		mIsPaused;

	INT64		mPrevCount;
	
	INT64		mCurrentCount;

	INT64		mDeltaCount;


};