#include "InputHandler.h"

void MoveLookController::onPointerPressed(
	_In_ Windows::UI::Core::CoreWindow^ sender,
	_In_ Windows::UI::Core::PointerEventArgs^ args
)
{
	UINT32 pointerID = args->CurrentPoint->PointerId;
	XMFLOAT2 position = XMFLOAT2(args->CurrentPoint->Position.x, args->CurrentPoint->Position.y);

	auto device = args->CurrentPoint->PointerDevice;
	auto deviceType = device->PointerDEviceType;
	if (deviceType == PointerDeviceType::Mouse)
	{
	}

	if ((position.x < 300 && position.y > 380) && (deviceType != PointerDeviceType::Mouse))
	{
		if (!m_moveInUse)
		{
			m_moveFirstDown = position;
			m_movePointerPosition = position;
			m_movePointerID = pointerID;
			m_moveInUse = true;
		}
	}
	else
	{
		if (!m_lookInUse)
		{
			m_lookLastPoint = position;
			m_lookPointerID = args->CurrentPoint->PointerId;
			m_lookLastDelta.x = 0;
			m_lookLastDelta.y = 0;
			m_lookInUse = true;
		}
	}
}

void MoveLookController::onPointerMoved(
_In_ Windows::UI::Core::CoreWindow^ sender,
_In_ Windows::UI::Core::PointerEventArgs^ args
)
{
	UINT32 pointerID = args->CurrentPoint->PointerId;
	XMFLOAT2 position = XMFLOAT2(args->CurrentPoint->Position.x, args->CurrentPoint->Position.y);

	if (pointerID == m_movePointerID)
	{
		m_movePointerPosition = position;
	}
	else if (pointerID == m_lookPointerID)
	{
		
		m_delta.x = position.x - m_lookLastPoint.x;
		m_delta.y = position.y - m_lookLastPoint.y;

		m_RotationDelta.x = m_delta.x * ROTATION_GAIN;
		m_RotationDelta.y = m_delta.y * ROTATION_GAIN;
		m_lookLastPoint = position;

		m_pitch -= m_RotationDelta.y;
		m_yaw -= m_RotationDelta.x;

		m_pitch = (float)__max(-XM_PI / 2.0f, m_pitch);
		m_pitch = (float)__min(+XM_PI / 2.0f, m_pitch);

	}
}

void MoveLookController::onPointerReleased(
	_In_ Windows::UI::Core::CoreWindow^ sender,
	_In_ Windows::UI::Core::PointerEventArgs^ args
)
{
	UINT32 pointerID = args->CurrentPoint->PointerId;
	XMFLOAT2 position = XMFLOAT2(args->CurrentPoint->Position.x, args->CurrentPoint->Position.y);

	if (pointerID == m_movePointerID)
	{
		m_moveInUse = false;
		m_movePointerID = 0;
	}
	else if (pointerID == m_lookPointerID)
	{
		m_lookInUse = false;
		m_lookPointerID = 0;
	}
}

void MoveLookController::onKeyDown(
	_In_ Windows::UI::Core::CoreWindow^ sender,
	_In_ Windows::UI::Core::PointerEventArgs^ args
)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	switch (Key)
	{
		if (Key == VirtualKey::Z)
		{
			m_forward = true;
		}
		if (Key == VirtualKey::S)
		{
			m_back = true;
		}
		if (Key == VirtualKey::Q)
		{
			m_left = true;
		}
		if (Key == VirtualKey::D)
		{
			m_right = true;
		}
	}
}

void MoveLookController::onKeyUp(
	_In_ Windows::UI::Core::CoreWindow^ sender,
	_In_ Windows::UI::Core::PointerEventArgs^ args
)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	switch (Key)
	{
		if (Key == VirtualKey::Z)
		{
			m_forward = false;
		}
		if (Key == VirtualKey::S)
		{
			m_back = false;
		}
		if (Key == VirtualKey::Q)
		{
			m_left = false;
		}
		if (Key == VirtualKey::D)
		{
			m_right = false;
		}
	}
}