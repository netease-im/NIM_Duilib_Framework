#include "StdAfx.h"

namespace ui 
{

AnimationManager::AnimationManager() : m_pControl(nullptr), m_animationMap()
{

}

AnimationManager::AnimationManager(const AnimationManager& r) : m_pControl(nullptr), m_animationMap()
{

}

AnimationPlayer* AnimationManager::GetAnimationPlayer(AnimationType animationType) const
{
	auto it = m_animationMap.find(animationType);
	if (it != m_animationMap.end()) {
		return it->second.get();
	}
	else {
		return nullptr;
	}
}

AnimationPlayer* AnimationManager::SetFadeHot(bool bFadeHot)
{
	AnimationPlayer* animationArgs = nullptr;
	if (bFadeHot) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetStartValue(0);
		animationArgs->SetEndValue(255);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedDownRatio(0.7);
		animationArgs->SetTotalMillSeconds(250);
		std::function<void(int)> playCallback = nbase::Bind(&Control::SetHotAlpha, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);
		m_animationMap[kAnimationHot].reset(animationArgs);
	}
	else {
		m_animationMap.erase(kAnimationHot);
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeAlpha(bool bFadeVisible)
{
	AnimationPlayer* animationArgs = nullptr;
	if (bFadeVisible) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetStartValue(0);
		animationArgs->SetEndValue(255);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedDownRatio(0.7);
		animationArgs->SetTotalMillSeconds(250);
		std::function<void(int)> playCallback = nbase::Bind(&Control::SetAlpha, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);
		m_animationMap[kAnimationAlpha].reset(animationArgs);
		m_pControl->SetAlpha(0);
	}
	else {
		m_animationMap.erase(kAnimationAlpha);
		m_pControl->SetAlpha(255);
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeWidth(bool bFadeWidth)
{
	AnimationPlayer* animationArgs = nullptr;
	if (bFadeWidth) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetStartValue(0);
		CSize size(999999, 999999);
		size = m_pControl->EstimateSize(size);
		ASSERT(size.cy >= -2);
		animationArgs->SetEndValue(size.cx);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedUpfactorA(0.00084);
		animationArgs->SetSpeedDownRatio(0.7);
		std::function<void(int)> playCallback = nbase::Bind(&Control::SetFixedWidth, m_pControl, std::placeholders::_1, true, false);
		animationArgs->SetCallback(playCallback);
		m_animationMap[kAnimationWidth].reset(animationArgs);
	}
	else {
		m_animationMap.erase(kAnimationWidth);
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeHeight(bool bFadeHeight)
{
	AnimationPlayer* animationArgs = nullptr;
	if (bFadeHeight) {
		animationArgs = new AnimationPlayer();
		animationArgs->SetStartValue(0);
		CSize size(999999, 999999);
		size = m_pControl->EstimateSize(size);
		ASSERT(size.cy > 0);
		animationArgs->SetEndValue(size.cy);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedUpfactorA(0.00084);
		animationArgs->SetSpeedDownRatio(0.7);
		std::function<void(int)> playCallback = nbase::Bind(&Control::SetFixedHeight, m_pControl, std::placeholders::_1, false);
		animationArgs->SetCallback(playCallback);
		m_animationMap[kAnimationHeight].reset(animationArgs);
	}
	else {
		m_animationMap.erase(kAnimationHeight);
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeInOutX(bool bFade, bool bIsFromRight)
{
	AnimationPlayer* animationArgs = nullptr;
	if (bFade) {
		animationArgs = new AnimationPlayer();
		CSize size(999999, 999999);
		size = m_pControl->EstimateSize(size);
		if (size.cx <= 0) {
			size.cx = 100;
		}
		animationArgs->SetEndValue(0);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedUpfactorA(0.006);
		animationArgs->SetSpeedDownRatio(0.7);
		std::function<void(int)> playCallback = nbase::Bind(&Control::SetRenderOffsetX, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);

		if (bIsFromRight) {
			animationArgs->SetStartValue(-size.cx);
			m_animationMap[kAnimationInoutXFromRight].reset(animationArgs);
		}
		else {
			animationArgs->SetStartValue(size.cx);
			m_animationMap[kAnimationInoutXFromLeft].reset(animationArgs);
		}
	}
	else if (!bFade){
		if (bIsFromRight) {
			m_animationMap.erase(kAnimationInoutXFromRight);
		}
		else {
			m_animationMap.erase(kAnimationInoutXFromLeft);
		}
	}

	return animationArgs;
}

AnimationPlayer* AnimationManager::SetFadeInOutY(bool bFade, bool bIsFromBottom)
{
	AnimationPlayer* animationArgs = nullptr;
	if (bFade) {
		animationArgs = new AnimationPlayer();
		CSize size(999999, 999999);
		size = m_pControl->EstimateSize(size);
		if (size.cy <= 0) {
			size.cy = 100;
		}
		animationArgs->SetEndValue(0);
		animationArgs->SetSpeedUpRatio(0.3);
		animationArgs->SetSpeedUpfactorA(0.006);
		animationArgs->SetSpeedDownRatio(0.7);
		std::function<void(int)> playCallback = nbase::Bind(&Control::SetRenderOffsetY, m_pControl, std::placeholders::_1);
		animationArgs->SetCallback(playCallback);

		if (bIsFromBottom) {
			animationArgs->SetStartValue(-size.cy);
			m_animationMap[kAnimationInoutYFromBottom].reset(animationArgs);
		}
		else {
			animationArgs->SetStartValue(size.cy);
			m_animationMap[kAnimationInoutYFromTop].reset(animationArgs);
		}
	}
	else if (!bFade){
		if (bIsFromBottom) {
			m_animationMap.erase(kAnimationInoutYFromBottom);
		}
		else {
			m_animationMap.erase(kAnimationInoutYFromTop);
		}
	}

	return animationArgs;
}

void AnimationManager::Appear()
{
	m_pControl->SetVisible_(true);
	if (GetAnimationPlayer(kAnimationAlpha)) {
		m_animationMap[kAnimationAlpha]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationAlpha]->Continue();
	}
	if (GetAnimationPlayer(kAnimationWidth)) {
		m_animationMap[kAnimationWidth]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationWidth]->Continue();
	}
	if (GetAnimationPlayer(kAnimationHeight)) {
		m_animationMap[kAnimationHeight]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationHeight]->Continue();
	}
	if (GetAnimationPlayer(kAnimationInoutXFromLeft)) {
		m_animationMap[kAnimationInoutXFromLeft]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationInoutXFromLeft]->Continue();
	}
	if (GetAnimationPlayer(kAnimationInoutXFromRight)) {
		m_animationMap[kAnimationInoutXFromRight]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationInoutXFromRight]->Continue();
	}
	if (GetAnimationPlayer(kAnimationInoutYFromTop)) {
		m_animationMap[kAnimationInoutYFromTop]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationInoutYFromTop]->Continue();
	}
	if (GetAnimationPlayer(kAnimationInoutYFromBottom)) {
		m_animationMap[kAnimationInoutYFromBottom]->SetCompleteCallback(CompleteCallback());
		m_animationMap[kAnimationInoutYFromBottom]->Continue();
	}
}

void AnimationManager::Disappear()
{
	bool handled = false;

	CompleteCallback completeCallback = nbase::Bind(&Control::SetVisible_, m_pControl, false);
	if (GetAnimationPlayer(kAnimationAlpha)) {
		m_animationMap[kAnimationAlpha]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationAlpha]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationWidth)) {
		m_animationMap[kAnimationWidth]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationWidth]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationHeight)) {
		m_animationMap[kAnimationHeight]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationHeight]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationInoutXFromLeft)) {
		m_animationMap[kAnimationInoutXFromLeft]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationInoutXFromLeft]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationInoutXFromRight)) {
		m_animationMap[kAnimationInoutXFromRight]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationInoutXFromRight]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationInoutYFromTop)) {
		m_animationMap[kAnimationInoutYFromTop]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationInoutYFromTop]->ReverseContinue();
		handled = true;
	}
	if (GetAnimationPlayer(kAnimationInoutYFromBottom)) {
		m_animationMap[kAnimationInoutYFromBottom]->SetCompleteCallback(completeCallback);
		m_animationMap[kAnimationInoutYFromBottom]->ReverseContinue();
		handled = true;
	}

	if (!handled) {
		m_pControl->SetVisible_(false);
	}
}

void AnimationManager::MouseEnter()
{
	if (GetAnimationPlayer(kAnimationHot)) {
		m_animationMap[kAnimationHot]->Continue();
	}
}

void AnimationManager::MouseLeave()
{
	if (GetAnimationPlayer(kAnimationHot)) {
		m_animationMap[kAnimationHot]->ReverseContinue();
	}
}

}