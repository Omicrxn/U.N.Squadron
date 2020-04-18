#include "Module.h"

Module::Module(bool startEnabled) : enabled(startEnabled)
{

}

bool Module::Init()
{
	return true;
}
bool Module::Start() {
	return true;
}

update_status Module::PreUpdate()
{
	return update_status::UPDATE_CONTINUE;
}

update_status Module::Update()
{
	return update_status::UPDATE_CONTINUE;
}

update_status Module::PostUpdate()
{
	return update_status::UPDATE_CONTINUE;
}

bool Module::CleanUp()
{
	return true;
}

void Module::OnCollision(Collider* c1, Collider* c2)
{

}

void Module::Enable()
{
	if (!enabled)
	{
		enabled = true;
		Start();
	}
}

void Module::Disable()
{
	if (enabled) {
		enabled = false;
		CleanUp();
	}
}