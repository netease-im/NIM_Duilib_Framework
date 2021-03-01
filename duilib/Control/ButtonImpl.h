template<typename InheritType>
UINT ui::ButtonTemplate<InheritType>::GetControlFlags() const
{
	return this->IsKeyboardEnabled() && this->IsEnabled() ? UIFLAG_TABSTOP : UIFLAG_DEFAULT;
}

template<typename InheritType>
ButtonTemplate<InheritType>::ButtonTemplate()
{
	this->m_uTextStyle = DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS | DT_NOCLIP | DT_SINGLELINE;
}

template<typename InheritType>
void ButtonTemplate<InheritType>::HandleMessage(EventArgs& event)
{
	if (!this->IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd) {
		if (this->m_pParent != NULL) this->m_pParent->HandleMessageTemplate(event);
		else __super::HandleMessage(event);
		return;
	}
	if (event.Type == kEventKeyDown) {
		if (this->IsKeyboardEnabled()) {
			if (event.chKey == VK_SPACE || event.chKey == VK_RETURN) {
				Activate();
				return;
			}
		}
	}
	if (event.Type == kEventInternalMenu) {
		if (this->IsContextMenuUsed()) {
			this->m_pWindow->SendNotify(this, kEventMouseMenu, event.wParam, event.lParam);
		}
		return;
	}

	__super::HandleMessage(event);
}

template<typename InheritType>
void ButtonTemplate<InheritType>::Activate()
{
	if (!this->IsActivatable()) return;
	if (this->m_pWindow != NULL) this->m_pWindow->SendNotify(this, kEventClick);
}
