#include "BlankProject.h"
#include "ComponentTest.h"

ComponentTest::ComponentTest()
{
}

ComponentTest::~ComponentTest()
{
}

void ComponentTest::Awake()
{
	DEBUG_LOG("Awake");
}

void ComponentTest::Start()
{
	DEBUG_LOG("Start");
}

void ComponentTest::Update()
{
	DEBUG_LOG("Update");
}

void ComponentTest::FixedUpdate()
{
}

void ComponentTest::LateUpdate()
{
}
