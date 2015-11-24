/*************************************************

Arcball.cpp

--------------------------------------------------

Oct. 20, 2015 - ZhuohanChen (pandachen0611@gmail.com)

*************************************************/

#include "Arcball.h"

//std includes


//gl includes
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

Arcball::Arcball(float radius, glm::vec2 center)
{
	this->Radius = radius;
	this->Center = center;
	Speed = 0.01f;
	Scale = 1.0f;
	
	this->ArcbMatrix = glm::mat4();
	this->ScalMatrix = glm::mat4();
	this->TranMatrix = glm::mat4();
}

Arcball::Arcball(float radius, float centerX, float centerY)
{
	this->Radius = radius;
	this->Center = glm::vec2(centerX, centerY);
	Speed = 0.01f;
	Scale = 1.0f;
	
	this->ArcbMatrix = glm::mat4();
	this->ScalMatrix = glm::mat4();
	this->TranMatrix = glm::mat4();
}

glm::mat4 Arcball::GetArcballMatrix()
{
	return this->TranMatrix * this->ScalMatrix * this->ArcbMatrix;
}

void Arcball::Model_opr(ARCBALL_Opr::Opr_State state)
{
	if(state == ARCBALL_Opr::ROTATE_ON)
		is_rotate = true;
	if(state == ARCBALL_Opr::SCALE_ON)
		is_scale = true;
	if(state == ARCBALL_Opr::TRANSLATE_ON)
		is_translate = true;
}

void Arcball::Model_opr_off(ARCBALL_Opr::Opr_State state)
{
	if(state == ARCBALL_Opr::ROTATE_OFF)
		is_rotate = false;
	if(state == ARCBALL_Opr::SCALE_OFF)
		is_scale = false;
	if(state == ARCBALL_Opr::TRANSLATE_OFF)
		is_translate = false;
}

glm::mat4 Arcball::motion(glm::vec2 preP, glm::vec2 nowP)
{
	glm::quat rotationQuat;
	glm::mat4 tempMatrix;
	
	if(this->is_rotate){
		glm::vec3 preVec, nowVec;
		preVec = computeVector(preP.x, preP.y);
		nowVec = computeVector(nowP.x, nowP.y);
		float cosAngle = glm::dot(preVec, nowVec);
		if (cosAngle > 0.999999){ return tempMatrix; }
		glm::vec3 rotationAxis = glm::normalize(glm::cross(preVec, nowVec));
		rotationQuat = glm::angleAxis(glm::degrees(glm::acos(cosAngle)), rotationAxis);
		tempMatrix = glm::mat4_cast(rotationQuat);
		ArcbMatrix = tempMatrix * ArcbMatrix;
	}
	if(this->is_scale){
		this->Scale += (nowP.y - preP.y) * this->Speed;
		if (this->Scale < 0.1)this->Scale = 0.1;
		ScalMatrix = glm::scale(glm::mat4(), glm::vec3(this->Scale));
		tempMatrix = ScalMatrix * tempMatrix;
	}
	if(is_translate){
		
	}
	
	return tempMatrix;
}

void Arcball::setParam(float radius, glm::vec2 center)
{
	this->Radius = radius;
	this->Center = center;
}

void Arcball::setSpeed(float speed)
{
	this->Speed = speed;
}

glm::vec3 Arcball::computeVector(float x, float y)
{
	x = x - this->Center.x;
	y = y - this->Center.y;
	float z = (this->Radius)*(this->Radius) - x*x - y*y;
	return (z > 0) ? 
			glm::normalize((glm::vec3(x, -y, glm::sqrt(z)))) : 
			glm::normalize((glm::vec3(x, -y, 0)));
}