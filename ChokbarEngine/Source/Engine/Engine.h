#pragma once

#include "Core/Core.h"
#include "Engine/Managers/PhysicsWorld.h"
#include "Platform/Windows/Window.h"

class Engine
{
public:

	~Engine();

	static Engine* GetInstance();
	static InputHandler *GetInput();
	static PhysicsWorld *GetPhysicsWorld();
	static CameraComponent *GetMainCamera();

	void Initialize();
	void Run();
	void Shutdown();

	void OnApplicationFocus();
	void OnApplicationLostFocus();

private:

	Engine();

	void PreInitialize();
	void InitComponents();
	
	void TogglePause();

	void Update(float dt);

	virtual void OnResize();

private:

	void Render();
	void CalculateFrameStats();
	bool NeedsToClose();

	float GetAspectRatio() const { return m_Window.GetWidth() / static_cast<float>(m_Window.GetHeight()); }

private:
	static Engine *m_Instance;
	Coordinator* m_Coordinator;

	TimeManager m_TimeManager;
	CameraManager m_CameraManager;
	InputHandler m_InputHandler;
	PhysicsWorld m_PhysicsWorld;

	Win32::Window m_Window;

	bool m_IsPaused = false;

};
