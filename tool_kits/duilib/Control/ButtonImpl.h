template<typename InheritType>
ButtonTemplate<InheritType>::ButtonTemplate()
{
	m_uTextStyle =  DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS | DT_NOCLIP | DT_SINGLELINE;
}

template<typename InheritType>
void ButtonTemplate<InheritType>::HandleMessage(EventArgs& event)
{
	if( !IsMouseEnabled() && event.Type > kEventMouseBegin && event.Type < kEventMouseEnd ) {
		if( m_pParent != NULL ) m_pParent->HandleMessageTemplate(event);
		else __super::HandleMessage(event);
		return;
	}
	if( event.Type == kEventKeyDown )
	{
		if (IsKeyboardEnabled()) {
			if( event.chKey == VK_SPACE || event.chKey == VK_RETURN ) {
				Activate();
				return;
			}
		}
	}
	if( event.Type == kEventInternalMenu )
	{
		if( IsContextMenuUsed() ) {
			m_pWindow->SendNotify(this, kEventMouseMenu, event.wParam, event.lParam);
		}
		return;
	}

	__super::HandleMessage(event);
}

template<typename InheritType>
void ButtonTemplate<InheritType>::Activate()
{
	if( !IsActivatable() ) return;
	if( m_pWindow != NULL ) m_pWindow->SendNotify(this, kEventClick);
}


