#include "StdAfx.h"
#include "AnimationPlayer.h"

namespace ui 
{

AnimationPlayerBase::AnimationPlayerBase() : m_bFirstRun(true), m_animationType(0)
{
	Init();
}

void AnimationPlayerBase::Reset()
{
	m_weakFlagOwner.Cancel();
	Init();
}

void AnimationPlayerBase::Init()
{
	m_startValue = 0;
	m_endValue = 0;
	m_currentValue = 0;
	m_totalMillSeconds = DUI_NOSET_VALUE;
	m_palyedMillSeconds = 0;
	m_elapseMillSeconds = 0;
	m_currentTime.QuadPart = 0;
	m_timeFrequency.QuadPart = 0;
	m_reverseStart = false;
	m_bPlaying = false;
}

void AnimationPlayerBase::Start()
{
	m_weakFlagOwner.Cancel();
	m_palyedMillSeconds = 0;
	m_reverseStart = false;
	StartTimer();
}

void AnimationPlayerBase::Stop()
{
	m_weakFlagOwner.Cancel();
}

void AnimationPlayerBase::Continue()
{
	m_weakFlagOwner.Cancel();
	if (m_reverseStart == true)
		ReverseAllValue();

	m_reverseStart = false;
	StartTimer();
	m_bFirstRun = false;
}

void AnimationPlayerBase::ReverseContinue()
{
	m_weakFlagOwner.Cancel();
	if (m_reverseStart == false)
		ReverseAllValue();

	if (m_bFirstRun) 
		m_palyedMillSeconds = 0;

	m_reverseStart = true;
	StartTimer();
	m_bFirstRun = false;
}

void AnimationPlayerBase::StartTimer()
{
	QueryPerformanceFrequency(&m_timeFrequency);
	QueryPerformanceCounter(&m_currentTime);

	m_bPlaying = true;
	if (m_endValue - m_startValue == 0) {
		Complete();
		return;
	}

	m_elapseMillSeconds = m_totalMillSeconds / abs(m_endValue - m_startValue);
	if (m_elapseMillSeconds == 0) {
		m_elapseMillSeconds = 1;
	}

	Play();
	auto playCallback = nbase::Bind(&AnimationPlayerBase::Play, this);
	TimerManager::GetInstance()->AddCancelableTimer(m_weakFlagOwner.GetWeakFlag(), playCallback, m_elapseMillSeconds, TimerManager::REPEAT_FOREVER);
}

void AnimationPlayerBase::Play()
{
	LARGE_INTEGER newCurrentTime;
	QueryPerformanceCounter(&newCurrentTime);
	m_palyedMillSeconds += (double(newCurrentTime.QuadPart - m_currentTime.QuadPart) * 1000 )/m_timeFrequency.QuadPart;
	QueryPerformanceCounter(&m_currentTime);

	int newCurrentValue = GetCurrentValue();
	if (m_playCallback) {
		if (m_endValue > m_startValue && newCurrentValue >= m_endValue
			|| m_endValue < m_startValue && newCurrentValue <= m_endValue) {
			newCurrentValue = m_endValue;
			m_playCallback(newCurrentValue);
			Complete();
		}
		else {
			if (newCurrentValue != m_currentValue) {
				m_playCallback(newCurrentValue);
			}
		}
	}
	else {
		ASSERT(FALSE);
	}

	m_currentValue = newCurrentValue;
}

void AnimationPlayerBase::ReverseAllValue()
{
	std::swap(m_startValue, m_endValue);
	m_currentValue = m_startValue;
	m_palyedMillSeconds = m_totalMillSeconds - m_palyedMillSeconds;
	if (m_palyedMillSeconds < 0)
		m_palyedMillSeconds = 0;
}

void AnimationPlayerBase::Complete()
{
	if (m_completeCallback)
		m_completeCallback();

	m_weakFlagOwner.Cancel();
	m_bPlaying = false;
}

AnimationPlayer::AnimationPlayer()
{
	Init();
}

void AnimationPlayer::Init()
{
	__super::Init();
	m_speedUpRatio = 0;
	m_speedDownRatio = 0;
	m_speedDownMillSeconds = 0;
	m_linerMillSeconds = 0;
	m_linearSpeed = 0;
	m_speedUpfactorA = 0;
	m_speedDownfactorA = 0;
	m_speedDownfactorB = 0;
	m_maxTotalMillSeconds = 1000000;
}

void AnimationPlayer::StartTimer()
{
	InitFactor();
	__super::StartTimer();
}

int AnimationPlayer::GetCurrentValue()
{
	if (m_palyedMillSeconds >= m_totalMillSeconds) {
		return m_endValue;
	}

	int detaValue = 0;
	if (m_palyedMillSeconds <= m_speedUpMillSeconds) {
		detaValue = int(m_speedUpfactorA * m_palyedMillSeconds * m_palyedMillSeconds);
	}
	else if (m_palyedMillSeconds <= (m_speedUpMillSeconds + m_linerMillSeconds)) {
		double linerTime = m_palyedMillSeconds - m_speedUpMillSeconds;
		detaValue = int(m_speedUpfactorA * m_speedUpMillSeconds * m_speedUpMillSeconds + m_linearSpeed * linerTime);
	}
	else if (m_palyedMillSeconds <= m_totalMillSeconds) {
		double speedDownTime = m_palyedMillSeconds - m_speedUpMillSeconds - m_linerMillSeconds;
		detaValue = int(m_speedUpfactorA * m_speedUpMillSeconds * m_speedUpMillSeconds + m_linearSpeed * m_linerMillSeconds
			+ m_speedDownfactorA * speedDownTime * speedDownTime + m_speedDownfactorB * speedDownTime);
	}
	else {
		ASSERT(FALSE); 
	}

	int currentValue = 0;
	if (m_endValue > m_startValue) {
		currentValue = m_startValue + detaValue;
	}
	else {
		currentValue = m_startValue - detaValue;
	}
	
	return currentValue;
}

void AnimationPlayer::InitFactor()
{
	double s = abs(m_endValue - m_startValue);

	if (m_speedUpRatio == 0 && m_speedDownRatio == 0) {	//liner
		ASSERT(m_totalMillSeconds == DUI_NOSET_VALUE && m_linearSpeed != 0 || m_totalMillSeconds != DUI_NOSET_VALUE && m_linearSpeed == 0);
		if (m_totalMillSeconds == DUI_NOSET_VALUE) {
			m_totalMillSeconds = int(s / m_linearSpeed);
		}
		else {
			m_linearSpeed = s / m_totalMillSeconds;
		}
		m_linerMillSeconds = m_totalMillSeconds;
	}
	else {
		if (m_totalMillSeconds != DUI_NOSET_VALUE) {
			if (m_speedUpRatio != 0) {
				m_speedUpfactorA = s / ((m_speedUpRatio*m_speedUpRatio + (1 - m_speedUpRatio - m_speedDownRatio)*2*m_speedUpRatio + m_speedUpRatio*m_speedDownRatio)
					*m_totalMillSeconds*m_totalMillSeconds);
			}
			else if (m_speedDownRatio != 0) {
				m_speedDownfactorA = -s / ((m_speedDownRatio*m_speedDownRatio + (1 - m_speedUpRatio - m_speedDownRatio)*2*m_speedDownRatio + m_speedUpRatio*m_speedDownRatio)
					*m_totalMillSeconds*m_totalMillSeconds);
			}
			else {
				ASSERT(FALSE);
			}

		}
		double tmpValue = 0;
		if (m_speedUpfactorA != 0 && m_speedUpRatio != 0) {
			tmpValue = m_speedUpfactorA * m_speedUpRatio;
			if (m_speedDownRatio != 0) {
				m_speedDownfactorA = -m_speedUpfactorA * m_speedUpRatio / m_speedDownRatio;
			}
		}
		else if (m_speedDownfactorA != 0 && m_speedDownRatio != 0) {
			tmpValue = -m_speedDownfactorA * m_speedDownRatio;
			if (m_speedUpRatio != 0) {
				m_speedUpfactorA = -m_speedDownfactorA * m_speedDownRatio / m_speedUpRatio;
			}
		}
		else {
			ASSERT(FALSE);
		}
		if (m_totalMillSeconds == DUI_NOSET_VALUE) {
			m_totalMillSeconds = int(sqrt(s / (m_speedUpfactorA*m_speedUpRatio*m_speedUpRatio + (1 - m_speedUpRatio - m_speedDownRatio) * 2 * tmpValue
				- m_speedDownfactorA*m_speedDownRatio*m_speedDownRatio) ));
			if (m_totalMillSeconds > m_maxTotalMillSeconds) {
				m_totalMillSeconds = m_maxTotalMillSeconds;
				InitFactor();
				return;
			}
		}

		m_linearSpeed = 2 * tmpValue * m_totalMillSeconds;
		m_speedDownfactorB = m_linearSpeed;

		m_speedUpMillSeconds = m_totalMillSeconds * m_speedUpRatio;
		m_speedDownMillSeconds = m_totalMillSeconds * m_speedDownRatio;
		m_linerMillSeconds = m_totalMillSeconds - m_speedUpMillSeconds - m_speedDownMillSeconds;
	}
}

}