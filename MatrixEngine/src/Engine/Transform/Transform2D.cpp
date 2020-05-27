//Mikel Fernandez


#include "Transform2D.h"
#include "GameObject\GameObject.h"

REFLECTION(Transform2D, posX, posY, mZorder, mPosition, mAngle, mScale, mIsChild);

Transform2D::Transform2D(): mPosition(glm::vec2(0,0)), mAngle(0.0f), mScale(glm::vec2(50.f,50.f)), mZorder(1.0f)
{
	m_name = "Transform";
	parentTr = nullptr;
}

Transform2D::Transform2D(glm::vec2 position, GLfloat angle, glm::vec2 scale, GLfloat z) : mZorder(z)
{
	mPosition = position;
	mAngle = angle;
	mScale = scale;
}

Transform2D::Transform2D(glm::vec2 position, glm::vec2 scale, GLfloat angle, GLfloat z) : mZorder(z)
{
	mPosition = position;
	mAngle = angle;
	mScale = scale;
}

glm::vec2 Transform2D::translate(glm::vec2 value)
{
	mPosition += value;
	return glm::vec2(mPosition);
}

glm::vec2 Transform2D::translate(float X, float Y)
{
	return translate(glm::vec2(X, Y));
}

glm::mat4 Transform2D::getTransformToParent() const
{
	Transform2D * aux = parentTr;
	glm::mat4 result = getMatrix();

	while (aux)
	{
		result = result * aux->getMatrix();
		aux = aux->parentTr;
	}


	return result;
}

glm::mat4 Transform2D::getTransformFromParent() const
{
	Transform2D * aux = parentTr;
	glm::mat4 result = getInvMtx();

	while (aux)
	{
		result = aux->getInvMtx() * result;
		aux = aux->parentTr;
	}

	return result;
}

glm::mat4 Transform2D::getMatrix() const
{
	glm::mat4 final_matrix;

	final_matrix[0][0] = mScale.x * cos(mAngle);
	final_matrix[0][1] = mScale.x * sin(mAngle);
	final_matrix[0][2] = 0;
	final_matrix[0][3] = 0.0f;
	final_matrix[1][0] = -mScale.y * sin(mAngle);
	final_matrix[1][1] = mScale.y * cos(mAngle);
	final_matrix[1][2] = 0;
	final_matrix[1][3] = 0.0f;
	final_matrix[2][0] = mPosition.x;
	final_matrix[2][1] = mPosition.y;
	final_matrix[2][2] = 1.0f;
	final_matrix[2][3] = 0.0f;
	final_matrix[3][0] = 0.0f;
	final_matrix[3][1] = 0.0f;
	final_matrix[3][2] = 0.0f;
	final_matrix[3][3] = 1.0f;

	return final_matrix;
}

glm::mat4 Transform2D::getInvMtx() const
{
	glm::mat4 final_matrix;

	final_matrix[0][0] = 1.0f / mScale.x * cos(mAngle);
	final_matrix[0][1] = -1.0f / mScale.y * sin(mAngle);
	final_matrix[0][2] = 0.0f;
	final_matrix[0][3] = 0.0f;
	final_matrix[1][0] = 1.0f / mScale.x * sin(mAngle);
	final_matrix[1][1] = 1.0f / mScale.y * cos(mAngle);
	final_matrix[1][2] = 0.0f;
	final_matrix[1][3] = 0.0f;
	final_matrix[2][0] = ((cos(mAngle) * -mPosition.x) + (sin(mAngle) * -mPosition.y)) / mScale.x;
	final_matrix[2][1] = ((sin(mAngle) * mPosition.x) + (-cos(mAngle) * mPosition.y)) / mScale.y;
	final_matrix[2][2] = 1.0f;
	final_matrix[2][3] = 0.0f;
	final_matrix[3][0] = 0.0f;
	final_matrix[3][1] = 0.0f;
	final_matrix[3][2] = 0.0f;
	final_matrix[3][3] = 1.0f;

	return  final_matrix;
}

glm::vec2 Transform2D::GetPos() const
{
	return mPosition;
}

glm::vec2 Transform2D::GetScale() const
{
	return mScale;
}

GLfloat Transform2D::GetRotation() const
{
	return mAngle;
}

void Transform2D::SetPos(glm::vec2 newPos)
{
	mPosition = newPos;
}

void Transform2D::IncreasePosBy(glm::vec2 newPos)
{
	mPosition += newPos;
}

void Transform2D::SetScale(glm::vec2 newScale)
{
	mScale = newScale;
}

void Transform2D::IncreaseScaleBy(glm::vec2 newScale)
{
	mScale += newScale;
}

void Transform2D::SetAngle(GLfloat newAngle)
{
	mAngle = newAngle;
}

void Transform2D::RotateBy(GLfloat newAngle)
{
	mAngle += newAngle;
}

void Transform2D::Initialize()
{
	if (mIsChild)
		GetOwner()->m_localTransform = this;
	else GetOwner()->m_transform = this;
	updateInEditMode = true;
}

//Do this properly
void Transform2D::Update()
{
	//auto* parent = GetOwner()->m_parent;

		GameObject* parent = m_GameObject->m_parent;
		while (parent != nullptr)
		{
			//GetOwner()->m_transform->mScale += parent->m_transform->mScale;
			//GetOwner()->m_transform->mAngle += parent->m_transform->mAngle;*/
			
			//GetOwner()->m_localtransform->RotateBy(parent->m_transform->mAngle);
			//GetOwner()->m_localtransform->mPosition += m_GameObject->m_parent->m_transform->mPosition;
			parent = parent->m_parent;
		}
		/*GetOwner()->m_transform->mScale += (mScale - glm::vec2(1,1));
		GetOwner()->m_transform->mAngle += mAngle;*/
		//GetOwner()->m_transform->mPosition += mPosition;
}
