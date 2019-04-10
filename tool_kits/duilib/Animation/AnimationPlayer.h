#ifndef UI_ANIMATION_ANIMATIONPLAYER_H_
#define UI_ANIMATION_ANIMATIONPLAYER_H_

#pragma once

namespace ui 
{

typedef std::function<void (int)> PlayCallback;
typedef std::function<void (void)> CompleteCallback;

class UILIB_API AnimationPlayerBase : public nbase::SupportWeakCallback
{
public:
	AnimationPlayerBase();
	~AnimationPlayerBase() { }

	void SetAnimationType(int type) { m_animationType = type; };
	int GetAnimationType() const { return m_animationType; };
	void SetStartValue(int startValue) { m_startValue = startValue; }
	int GetStartValue() { return m_startValue; }

	void SetEndValue(int endValue) { m_endValue = endValue; }
	int GetEndValue() { return m_endValue; }

	void SetTotalMillSeconds(int totalMillSeconds) { m_totalMillSeconds = totalMillSeconds; }
	bool IsPlaying() { return m_bPlaying; }
	bool IsFirstRun() { return m_bFirstRun; }

	void SetCallback(const PlayCallback& callback) { m_playCallback = callback; }
	void SetCompleteCallback(const CompleteCallback& callback) { m_completeCallback = callback; }

	void Reset();
	virtual void Init();
	virtual void Start();
	virtual void Stop();
	virtual void Continue();
	virtual void ReverseContinue();
	virtual void StartTimer();
	virtual int GetCurrentValue() = 0;

private:
	void Play();
	void ReverseAllValue();
	void Complete();

protected:
	int m_animationType;
	int m_startValue;
	int m_endValue;
	int m_currentValue;

	int m_totalMillSeconds;
	double m_palyedMillSeconds;
	int m_elapseMillSeconds;

	bool m_bFirstRun;
	bool m_bPlaying;
	bool m_reverseStart;

	PlayCallback m_playCallback;
	CompleteCallback m_completeCallback;
		
	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_timeFrequency;
	
	nbase::WeakCallbackFlag m_weakFlagOwner;
};


class UILIB_API AnimationPlayer : public AnimationPlayerBase
{
public:
	AnimationPlayer();

	virtual void Init() override;
	virtual void StartTimer() override;
	virtual int GetCurrentValue() override;

	void InitFactor();
	virtual void SetSpeedUpfactorA(double factorA) { m_speedUpfactorA = factorA; }
	virtual void SetSpeedDownfactorA(double factorA) { m_speedDownfactorA = factorA; }

	void SetMaxTotalMillSeconds(int maxTotalMillSeconds) { m_maxTotalMillSeconds = maxTotalMillSeconds; }
	void SetLinearSpeed(double linearSpeed) { m_linearSpeed = linearSpeed; }
	void SetSpeedUpRatio(double speedUpRatio) { m_speedUpRatio = speedUpRatio; }
	void SetSpeedDownRatio(double speedDownRatio) { m_speedDownRatio = speedDownRatio; }

private:
	double m_speedUpRatio;
	double m_speedDownRatio;
	double m_speedUpMillSeconds;

	double m_linearSpeed;
	double m_linerMillSeconds;

	double m_speedDownMillSeconds;
	
	double m_speedUpfactorA;
	double m_speedDownfactorA;
	double m_speedDownfactorB;

	int m_maxTotalMillSeconds;
};

} // namespace ui

#endif // UI_ANIMATION_ANIMATIONPLAYER_H_
