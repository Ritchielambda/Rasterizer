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

	//重置所有数据
	void ResetAll();

	void ResetTotalTime();


private:
	//总时间
	double		mTotalTime;
	//INTERVAL 两帧的时间间隔
	double		mDeltaTime;
	//每个COUNT占了多少毫秒
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