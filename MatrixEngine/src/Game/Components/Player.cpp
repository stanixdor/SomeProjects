#include "Player.h"

#include "../../MatrixEngine.h"
#include "../../Engine/Platform/Input/Input.h"
#include <iostream>
#include "../../Engine/Physics/RigidBody.h"
#include "../../Engine/Graphics/SpriteAnimationData.h"
#include "../../Engine/Graphics/Mesh.h"
#include "../../Engine/Graphics/Shader.h"
#include "../../Engine/Graphics/SpriteLayer.h"
#include "../../Engine/Physics/CollisionSystem.h"
#include "../../Engine/Graphics/GraphicsSystem.h"

#define AMOUNT 1.0f
	//RTTI_DECLARATION;
using namespace Graphics;

void Player::Initialize()
{

	/*vertex1 = new Graphics::Shader("./Shader/Animations.vs", Graphics::Shader::eVertex);
	fragment1 = new Graphics::Shader("./Shader/Animations.fs", Graphics::Shader::ePixel);
	SpriteAnimationData * data = new SpriteAnimationData("RunningMan", nullptr, 5, 2.0f);
	auto* rend = GetOwner()->AddComponent<Graphics::AnimComp>();
	rend->pMesh = new Mesh();

	GetOwner()->FindComponentByType<AnimComp>()->mAnimData = data;

	rend->pTransform = rend->GetOwner()->m_transform;
	rend->pTransform->SetPos({0,0});
	rend->pShader = (new Graphics::ShaderProgram(vertex1, fragment1));
	rend->pTexture = new Graphics::Texture("Images/AnimationTest.png");
	rend->pMesh->Init();

	GetOwner()->AddComponent<RigidBody>();
	auto * rigidbody = GetOwner()->FindComponentByType<RigidBody>();
	CollisionSystem::Instance()->AddRigidBody(*rigidbody, true);

	auto* cameraObj = objectManager.AddGameObject("camera");
	cam1 = cameraObj->AddComponent<Graphics::Camera>();
	Graphics::SpriteLayer * sprLay = new Graphics::SpriteLayer("STATICBG", Graphics::SpriteLayer::LayerIndex::STATICBG, cam1);
	/////////////////

	sprLay->AddRenderable(rend);

	GraphicsSystem::Instance()->mSpriteLayer.push_back(sprLay);*/
}
void Player::Update()
{
	//if (Input::Instance()->KeyPressed('A'))
	//{
	//	GetOwner()->m_transform->SetPos(glm::vec2(GetOwner()->m_transform->GetPos().x - AMOUNT, GetOwner()->m_transform->GetPos().y));
	//}
	//
	//if (Input::Instance()->KeyPressed('D'))
	//{
	//	GetOwner()->m_transform->SetPos(glm::vec2(GetOwner()->m_transform->GetPos().x + AMOUNT, GetOwner()->m_transform->GetPos().y));
	//}
	//
	//if (Input::Instance()->KeyTriggered('W'))
	//{
	//	GetOwner()->FindComponentByType<RigidBody>()->AddForce({ 0, 1000.f });
	//}
	//
	//if (Input::Instance()->KeyPressed('S'))
	//{
	//	GetOwner()->FindComponentByType<RigidBody>()->AddForce({ 0, -1000.f });
	//}
	//GetOwner()->m_transform->posX = GetOwner()->m_transform->GetPos().x;
	//GetOwner()->m_transform->posY = GetOwner()->m_transform->GetPos().y;

}
