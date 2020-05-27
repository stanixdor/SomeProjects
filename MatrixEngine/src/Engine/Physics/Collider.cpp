#include "Collider.h"
#include "GameObject\GameObject.h"
#include "Physics\CollisionTable.h"
#include "Physics\CollisionSystem.h"
#include "Physics\RigidBody.h"
#include "Graphics\GraphicsComponents\LineRenderer.h"
#include "GameObject\GameObject.h"
#include "../GLM/gtx/perpendicular.hpp"
#include "../GLM/gtx/matrix_transform_2d.hpp"

REFLECTION(Collider, collisionGroupName, colliderTransform, isGhost);

//int i = 0;
Collider::Collider()
{
	isOwnerStatic = nullptr;
	isGhost = false;
	m_name = "Collider";
	collisionGroupName = "";
	colliderTransform.SetScale({ 1.f, 1.f });
}

Collider::~Collider()
{
	CollisionSystem::Instance()->RemoveCollider(*this);
}

void Collider::Initialize()
{
	updateInEditMode = true;
	updateInEditPauseMenu = true;
	if (!m_GameObject->FindComponentByType<Graphics::LineRenderer>())
	{
		m_GameObject->AddComponent<Graphics::LineRenderer>();
	}

	m_GameObject->FindComponentByType<Graphics::LineRenderer>()->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	if (collisionGroupName == "")
		collisionGroupName = "default";

	addCollisionGroup(CollisionGroup::GetCollisionGroup(collisionGroupName));
	if (colliderTransform.GetScale().x == 0)
		colliderTransform.mScale.x = 1.f;

	if (colliderTransform.GetScale().y == 0)
		colliderTransform.mScale.y = 1.f;

	if (m_GameObject->FindComponentByType<RigidBody>() == nullptr)
	{
		//std::cout << m_GameObject->m_transform->GetPos().x << " " << m_GameObject->m_transform->GetPos().y << std::endl;
		m_GameObject->AddComponent<RigidBody>();
	}
	if (isOwnerStatic == nullptr)
		isOwnerStatic = &(m_GameObject->FindComponentByType<RigidBody>()->isStatic);
	CollisionSystem::Instance()->AddCollider(*this);

}

void Collider::Update()
{
	if (m_GameObject->FindComponentByType<RigidBody>() == nullptr)
		m_GameObject->AddComponent<RigidBody>();

	

	if (GetOwner()->m_transform->GetRotation() == 0 && mCollisionShape == ECollisionShape::CSHAPE_OBB)
		mCollisionShape = ECollisionShape::CSHAPE_AABB;
	else if (GetOwner()->m_transform->GetRotation() != 0 && mCollisionShape == ECollisionShape::CSHAPE_AABB)
		mCollisionShape = ECollisionShape::CSHAPE_OBB;

	drawCollider();
}

void Collider::LateUpdate()
{
	m_GameObject->FindComponentByType<Graphics::LineRenderer>()->Flush();
}

//!----------------------------------------------------------------------------
// \fn		ComputeAABB
// \brief	Computes the AABB of the rigidbody. Stores the result in 
//			the provided parameters (outPos and outSize).
// \details
//		- Depending on the shape, you must compute different AABBs:
//		- Case CSHAPE_AABB: Already an AABB. Trivial. 
//		- Case CSHAPE_CIRCLE: Return an AABB with size(x = y = radius). 
//		- Case CSHAPE_OBB: Return AABB defined by the max/min corners of the OBB along the x and y axes.
// ----------------------------------------------------------------------------
void Collider::ComputeAABB(glm::vec2 & outPos, glm::vec2& outSize)
{
	GameObject* owner = GetOwner();

	glm::vec2 scale = owner->m_transform->GetScale();
	GLfloat rotation = owner->m_transform->GetRotation();

	//depending on the collision shape we apply the values needed to outPos and outSize
	switch (mCollisionShape)
	{

	case CSHAPE_AABB:
		outPos = owner->m_transform->GetPos();
		outSize = owner->m_transform->GetScale();
		break;
	case CSHAPE_CIRCLE:
		outPos = owner->m_transform->GetPos();
		outSize = { scale.x * 2, scale.x * 2 };
		break;
	case CSHAPE_OBB:
		outPos = owner->m_transform->GetPos();
		outSize = { abs(cos(rotation) * scale.x) + abs(sin(rotation) * scale.y), abs(sin(rotation) * scale.x) + abs(cos(rotation) * scale.y) };
		break;
	default:
		break;
	}
}

Transform2D Collider::getTransform() const
{
	Transform2D finalTransform;

	finalTransform.SetPos(m_GameObject->m_transform->GetPos() + colliderTransform.GetPos());
	finalTransform.SetScale(glm::vec2(m_GameObject->m_transform->GetScale().x * colliderTransform.GetScale().x, m_GameObject->m_transform->GetScale().y * colliderTransform.GetScale().y ));
	finalTransform.SetAngle(m_GameObject->m_transform->GetRotation() + colliderTransform.GetRotation());
	
	return finalTransform;
}

CollisionGroup& Collider::getCollisionGroup()
{
	return mCollisionGroup;
}


void Collider::drawCollider()
{
	Transform2D tr = getTransform();

	glm::vec2 pos = glm::vec2(tr.GetPos());
	glm::vec2 scale = tr.GetScale();
	glm::vec2 point0 = pos + glm::vec2(-scale.x / 2.f, scale.y / 2.f);

	glm::vec2 point1 = pos + glm::vec2(scale.x / 2.f, scale.y / 2.f);

	glm::vec2 point2 = pos + glm::vec2(-scale.x / 2.f, -scale.y / 2.f);

	glm::vec2 point3 = pos + glm::vec2(scale.x / 2.f, -scale.y / 2.f);

	float angle = tr.GetRotation();
	glm::vec2 pivot = tr.GetPos();

	float P0x = cos(angle) * (point0.x - pivot.x) - sin(angle) *(point0.y - pivot.y) + pivot.x;
	float P0y = sin(angle) * (point0.x - pivot.x) + cos(angle) *(point0.y - pivot.y) + pivot.y;
	float P1x = cos(angle) * (point1.x - pivot.x) - sin(angle) *(point1.y - pivot.y) + pivot.x;
	float P1y = sin(angle) * (point1.x - pivot.x) + cos(angle) *(point1.y - pivot.y) + pivot.y;

	float P2x = cos(angle) * (point2.x - pivot.x) - sin(angle) *(point2.y - pivot.y) + pivot.x;
	float P2y = sin(angle) * (point2.x - pivot.x) + cos(angle) *(point2.y - pivot.y) + pivot.y;
	float P3x = cos(angle) * (point3.x - pivot.x) - sin(angle) *(point3.y - pivot.y) + pivot.x;
	float P3y = sin(angle) * (point3.x - pivot.x) + cos(angle) *(point3.y - pivot.y) + pivot.y;

	m_GameObject->FindComponentByType<Graphics::LineRenderer>()->DrawRect(glm::vec2(P0x, P0y), glm::vec2(P1x, P1y), glm::vec2(P2x, P2y), glm::vec2(P3x, P3y));
}

void Collider::addCollisionGroup(CollisionGroup group)
{
	mCollisionGroup = group;
	collisionGroupName = group.mName;
}
