#include "GenericLevel.h"

std::unique_ptr<GameObject> cameraObj = std::make_unique<GameObject>();
Graphics::Camera *cam1 = nullptr;

void GenericLevel::Initialize()
{
	CollisionGroup floorCollision = CollisionGroup::CreateCollisionGroup("Floor");
	CollisionGroup playerCollision = CollisionGroup::CreateCollisionGroup("Player");
	CollisionTable::Instance()->AddPolicy((u32)CollisionGroup::GetCollisionGroup("floor"), (u32)CollisionGroup::GetCollisionGroup("Player"), CollisionPolicy::Resolve);
	CollisionSystem::Instance()->Init();

	cameraObj = std::make_unique<GameObject>("camera", true);
	cameraObj->EditableByGizmos = false;
	cam1 = cameraObj->AddComponent<Graphics::Camera>();

	GraphicsSystem::Instance()->mSpriteLayer[0]->setCamera(cam1);
}
void GenericLevel::LoadResources(const std::string& name, const std::string& folder)
{
	objectManager.LoadFromFile(name, folder);
}
void GenericLevel::Update()
{
	
	CollisionSystem::Instance()->Update();
	for (auto& comp : cameraObj->m_components)
		comp->Update();
	objectManager.Update();
}
void GenericLevel::UnloadResources()
{
	cameraObj.reset();
	objectManager.Destroy();
	cam1 = nullptr;
	CollisionGroup::collisionGroups.clear();
}
void GenericLevel::ShutDown()
{

}