#include <DirectXMath.h>


using namespace DirectX;


class Camera : public Chokbar::Component
{
public:
	Camera();
	~Camera();

	
	// Get camera basis vectors.
	XMVECTOR GetRight() const;
	XMFLOAT3 GetRight3f() const;
	XMVECTOR GetUp() const;
	XMFLOAT3 GetUp3f() const;
	XMVECTOR GetLook() const;
	XMFLOAT3 GetLook3f() const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	// Get View/Proj matrices.
	XMMATRIX GetView()const;
	XMMATRIX GetProj()const;

	XMFLOAT4X4 GetView4x4f()const;
	XMFLOAT4X4 GetProj4x4f()const;

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();
	void UpdateProjMatrix(const float winWidth, const float winHeight);

private:

	// Camera coordinate system with coordinates relative to world space
	XMFLOAT3 m_Right = { 1.0f, 0.0f, 0.0f };
	XMFLOAT3 m_Up = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 m_Look = { 0.0f, 0.0f, 1.0f };

	// Cache frustum properties.
	float m_NearZ = 0.5f;
	float m_FarZ = 1000.0f;
	float m_Aspect = 0.0f;
	float m_FovY = 0.0f;
	float m_NearWindowHeight = 0.0f;
	float m_FarWindowHeight = 0.0f;

	bool m_ViewDirty = true;

	// Cache View/Proj matrices.
	DirectX::XMFLOAT4X4 m_View = Identity4x4();
	DirectX::XMFLOAT4X4 m_Proj = Identity4x4();
};