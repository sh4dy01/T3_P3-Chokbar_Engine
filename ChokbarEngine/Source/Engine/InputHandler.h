
#define ROTATION_GAIN 0.004f
#define MOVEMENT_GAIN 0.1f

#include <DirectXMath.h>
#include "Chokbar.h"
using namespace DirectX;

class MoveLookController
{
private: 
	XMFLOAT3 m_position;
	float m_pitch;
	float m_yaw;

	bool m_forward;
	bool m_back;
	bool m_left;
	bool m_right;
	bool m_up;
	bool m_down;

	bool m_moveInUse;
	UINT32 m_movePointerID;
	XMFLOAT2 m_moveFirstDown;
	XMFLOAT2 m_movePointerPosition;
	XMFLOAT2 m_moveCommand;

	bool m_lookInUse;
	UINT32 m_lookPointerID;
	XMFLOAT2 m_lookLastPoint;
	XMFLOAT2 m_lookLastDelta;
	XMFLOAT2 m_delta;
	XMFLOAT2 m_RotationDelta;

public: 
	void onPointerPressed(
		_In_ Windows::UI::Core::CoreWindow^ sender, 
		_In_ Windows::UI::Core::PointerEventArgs^ args
		);
	void onPointerMoved(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::PointerEventArgs^ args
		);
	void onPointerReleased(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::PointerEventArgs^ args
		);
	void onKeyDown(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::KeyEventArgs^ args
		);
	void onKeyUp(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::KeyEventArgs^ args
		);

	void Initialize(_In_ Windows::UI::Core::CoreWindow^ window);

	void Update(Windows::UI::Core::CoreWindow^ window);

	void setPosition(_In_ XMFLOAT3 pos);
	void setOrientation(_In_ float pitch, _In_ float yaw);

	XMFLOAT3 getPosition();
	XMFLOAT2 getLookPoint();

};