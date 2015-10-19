/*************************************************

Arcball.h

A C++ class that implements the Arcball.
This class takes as input mouse events and create
the appropriate quaternions and 4x4 matrices to 
represent the rotation given by the mouse.

This class is used as follows:
- initialize the center position (x,y,z) of the 
  arcball on the world, and the radius.
- on mouse down, call mouse_down with the mouse
  position.
- as the mouse is dragged, repeatedly call 
  mouse_motion with the current mouse location.
- when the mouse button is released, call mouse_up

Note:
 - set your camera at (0.0, 0.0, whatever_ever_you_want) 
   as well as looking at (0.0, 0.0, 0.0), the UpAxis is 
   supposed to be (0.0, 1.0, 0.0).
 - draw your object at the center of the world space (which
   is (0.0, 0.0, 0.0)).
--------------------------------------------------



*************************************************/

#pragma once

//std includes
#include <iostream>

//gl includes
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

enum Mouse_State {
	LEFTBUTTON_DOWN,
	LEFTBUTTON_UP,
	RIGHTBUTTON_DOWN,
	RIGHTBUTTON_UP
};

class Arcball
{
public:
	//setup with vectors
	Arcball(GLfloat radius, glm::vec2 center, glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat angle = 0.0f)
	{
		this->Center = center;
		glm::quat quaternion = glm::angleAxis((angle), rotation);
		this->ArcballMatrix = glm::toMat4(quaternion);
		this->Radius = radius;
		//need to translate this matrix if the position isn't (0, 0, 0) in world space
		//...
	}
	//setup with scalar values
	Arcball(GLfloat radius, GLfloat centerX, GLfloat centerY, GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat angle)
	{
		this->Center = glm::vec2(centerX, centerY);
		glm::quat quaternion = glm::angleAxis((angle), glm::vec3(rotX, rotY, rotZ));
		this->ArcballMatrix = glm::toMat4(quaternion);
		this->Radius = radius;
		//need to translate this matrix if the position isn't (0, 0, 0) in world space
		//...
	}
	
	//returns the arcball matrix
	glm::mat4 GetArcballMatrix()
	{
		return this->ArcballMatrix;
	}
	
	void mouse_down(Mouse_State state)
	{
		if(state == LEFTBUTTON_DOWN)
			is_rotate = true;
		if(state == RIGHTBUTTON_DOWN)
			is_zooming = true;
	}
	
	void mouse_motion(GLfloat lastX, GLfloat lastY, GLfloat x, GLfloat y)
	{
		glm::quat rotationQuat;
		
		if(is_rotate){
			std::cout << "rotate" << std::endl;
			glm::vec3 lastVec, Vec;
			lastVec = computeVector(lastX, lastY);
			Vec = computeVector(x, y);
			glm::vec3 rotationAxis = glm::normalize(glm::cross(lastVec, Vec));
			GLfloat cosAngle = glm::dot(lastVec, Vec);
			GLfloat s = glm::sqrt((1 + cosAngle)*2);
			GLfloat invs = 1 / s;
			//rotationQuat = glm::quat(
			//	s * 0.5f,
			//	rotationAxis.x * invs,
			//	rotationAxis.y * invs,
			//	rotationAxis.z * invs);
			//rotationQuat = glm::normalize(glm::quat(glm::degrees(glm::acos(cosAngle)), rotationAxis));
			rotationQuat = glm::angleAxis(glm::degrees(glm::acos(cosAngle)), rotationAxis);
		}
		if(is_zooming){
			
		}
		this->updateArcballMatrix(rotationQuat);
	}
	
	void mouse_up(Mouse_State state)
	{
		if(state == LEFTBUTTON_UP)
			is_rotate = false;
		if(state == RIGHTBUTTON_UP)
			is_zooming = false;		
	}

	//update the parameter when the window size changed
	void setParam(GLfloat radius, glm::vec2 center)
	{
		this->Radius = radius;
		this->Center = center;
	}

	void setParam(GLfloat radius, GLfloat centerX, GLfloat centerY)
	{
		this->Radius = radius;
		this->Center = glm::vec2(centerX, centerY);
	}
	
private:
	//model attributes
	glm::vec2 Center;
	glm::mat4 ArcballMatrix;
	GLfloat Radius;
	
	//model options
	bool is_zooming;
	bool is_rotate;
	
	//calculate the arcball matrix using the rotation and angle
	void updateArcballMatrix(glm::quat quaternion)
	{
		ArcballMatrix = glm::mat4_cast(quaternion) * ArcballMatrix;
	}
	
	//calculate the vector of the point
	glm::vec3 computeVector(GLfloat x, GLfloat y)
	{
		GLfloat z = (this->Radius)*(this->Radius) - x*x - y*y;
		return (z > 0) ? glm::normalize((glm::vec3(x, y, glm::sqrt(z)))) : glm::normalize((glm::vec3(x, y, 0)));
	}
}