#pragma once

#include "Engine/Managers/InputHandler.h"
#include "Engine/ECS/Base/Coordinator.h"
#include "Platform/Windows/Window.h"
#include "PhysicsWorld.h"
#include "Engine/Managers/CameraManager.h"
#include "Managers/TimeManager.h"


class Engine
{
public:

	Engine();
	~Engine();

	static Engine* GetInstance();
	static Coordinator *GetCoordinator();
	static InputHandler *GetInput();
	static PhysicsWorld *GetPhysicsWorld();
	static CameraComponent *GetMainCamera();

	void Initialize();
	void Run();
	void Shutdown();

	void OnApplicationFocus();
	void OnApplicationLostFocus();

private:

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

	TimeManager m_TimeManager;
	Coordinator m_Coordinator;
	CameraManager m_CameraManager;
	InputHandler m_InputHandler;
	PhysicsWorld m_PhysicsWorld;

	Win32::Window m_Window;

	bool m_IsPaused = false;

};
