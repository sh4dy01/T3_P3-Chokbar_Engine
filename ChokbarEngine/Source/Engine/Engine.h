#pragma once

#include "Engine/Managers/InputHandler.h"
#include "Engine/ECS/Base/Coordinator.h"
#include "GameTimer.h"
#include "Platform/Windows/Window.h"
#include "PhysicsWorld.h"
#include "Engine/Managers/CameraManager.h"


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

protected:
	void PreInitialize();

	void Update(float dt);

	virtual void OnResize();

private:
	void Render();
	void CalculateFrameStats();
	bool NeedsToClose();

	float GetAspectRatio() const { return m_Window.GetWidth() / static_cast<float>(m_Window.GetHeight()); }

private:
	static Engine *m_Instance;

	GameTimer m_GameTimer;
	Coordinator m_Coordinator;
	CameraManager m_CameraManager;

	// physic
	Win32::Window m_Window;

	InputHandler m_InputHandler;

	PhysicsWorld m_PhysicsWorld;

	bool m_IsGamePaused = false;
};