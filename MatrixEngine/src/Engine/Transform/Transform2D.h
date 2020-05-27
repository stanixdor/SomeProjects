//Mikel Fernandez
#pragma once

#include "../GLM/glm.hpp"
#include "../GL/glew.h"
#include "../GameObject/Component.h"

class Transform2D: public Component
{
public:
	SERIALIZABLE;
	Transform2D();

	glm::vec2 translate(glm::vec2);
	glm::vec2 translate(float X, float Y);

	Transform2D(glm::vec2 position, GLfloat angle, glm::vec2 scale, GLfloat z = 2.0f);
	Transform2D(glm::vec2 position, glm::vec2 scale, GLfloat angle, GLfloat z = 2.0f);
	glm::mat4 getTransformToParent() const;
	glm::mat4 getTransformFromParent() const;

	glm::mat4 getMatrix() const;
	glm::mat4 getInvMtx() const;

	//Gettors
	glm::vec2 GetPos() const;
	glm::vec2 GetScale() const;
	GLfloat GetRotation() const;

	//settors
	void SetPos(glm::vec2 newPos);
	void SetScale(glm::vec2 newScale);
	void SetAngle(GLfloat newAngle);
	void IncreaseScaleBy(glm::vec2 newScale);
	void IncreasePosBy(glm::vec2 newPos);
	void RotateBy(GLfloat newAngle);

	virtual void Initialize();
	virtual void Update();

	float posX = 0.f;
	float posY = 0.f;

	GLfloat mZorder;
	Transform2D* parentTr = nullptr;

//private:
	glm::vec2 mPosition;
	bool mIsChild = false;
	GLfloat mAngle;
	glm::vec2 mScale;
};
