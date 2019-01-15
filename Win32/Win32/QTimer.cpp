#include"MyGameEngine.h"
using namespace MathInterface;

QTimer::QTimer(TIMER_TIMEUINT timeUnit)
{
	//millsceond default
	m_TimeUnit = timeUnit;
	mMilliSecondsPerCount = 0.0;
	mDeltaTime = 0.0;
	mTotalTime = 0.0;
	mIsPaused = FALSE;

	//每秒可以数多少次
	INT64 countsPerSecond;
	//获取这个计数计时器的频率
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	mMilliSecondsPerCount = (1000.0) / (double)countsPerSecond;//每一count多少毫秒
	QueryPerformanceCounter((LARGE_INTEGER*)&mCurrentCount);

}

void QTimer::setDestruction()
{
}

double QTimer::GetTotalTime() const
{
	switch (m_TimeUnit)
	{
	case TIMER_TIMEUNIT_MILLISECOND:
		return mTotalTime;
	case TIMER_TIMEUNIT_SECOND:
		return (mTotalTime / 1000.0);
		break;
	}
	return 0.0;
}

double QTimer::GetInterval() const
{
	switch (m_TimeUnit)
	{
	case TIMER_TIMEUNIT_MILLISECOND:
		return mDeltaTime;
		break;
	case TIMER_TIMEUNIT_SECOND:
		return (mDeltaTime / 1000.0);
		break;
	};
	return 0.0;
}

void QTimer::SetTimeUnit(TIMER_TIMEUINT timeUnit)
{
	if (timeUnit == TIMER_TIMEUNIT_SECOND || timeUnit == TIMER_TIMEUNIT_MILLISECOND)
	{
		m_TimeUnit = timeUnit;
	};
}

void QTimer::NextTick()
{
	if (mIsPaused)
	{
		mDeltaTime = 0.0;
	}
	else

	{
		mPrevCount = mCurrentCount;
		QueryPerformanceCounter((LARGE_INTEGER*)&mCurrentCount);

		mDeltaCount = mCurrentCount - mPrevCount;
		bool isdeltatimepositive = ((mDeltaCount > 0));
		if(isdeltatimepositive)
		{
			mDeltaTime = (double)(mDeltaCount*mMilliSecondsPerCount);
		}
		else
		{
			mDeltaTime = 0.0;
		}

		mTotalTime += mDeltaTime;

		++mCurrentSecondTickCount;
		{
			if (mCurrentSecondInteger != UINT(mTotalTime / 1000.0))
			{
				mFPS = mCurrentSecondTickCount;
				mCurrentSecondTickCount = 0;//reset
				mCurrentSecondInteger = UINT(mTotalTime / 1000.0);
			}
		}
	}
}

UINT QTimer::GetFPS() const
{
	return mFPS;
}

void QTimer::Pause()
{
	mIsPaused = true;
}

void QTimer::Continue()
{
	mIsPaused = FALSE;
}

void QTimer::ResetAll()
{
	mTotalTime = 0.0;
	mDeltaTime = 0.0;
	mIsPaused = FALSE;
}

void QTimer::ResetTotalTime()
{
	mTotalTime = 0;
}
