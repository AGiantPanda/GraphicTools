/*************************************************

Arcball.h

A C++ class that implements the Arcball.
This class takes as input mouse events and create
the appropriate quaternions and 4x4 matrices to 
represent the rotation given by the mouse.

This class is used as follows:
- initialize the center position (x,y) of the 
  screen, and the radius.
- on mouse down, call mouse_down with the mouse
  position.
- as the mouse is dragged, repeatedly call
  mouse_motion with the current mouse location.
- when the mouse button is released, call mouse_up.

Note:
- set your camera looking at (0.0, 0.0, 0.0).
- draw your object at the center of the world space (which
  is (0.0, 0.0, 0.0)), or translate it yourself.
--------------------------------------------------

Oct. 20, 2015 - ZhuohanChen (pandachen0611@gmail.com)

*************************************************/

#pragma once

//std includes

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

const GLfloat SPEED = 0.01f;

class Arcball
{
public:
	//setup with vectors
	Arcball(GLfloat radius, glm::vec2 center, glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat angle = 0.0f) :Zoom(1.0f), Speed(SPEED)
	{
		this->Center = center;
		glm::quat quaternion = glm::angleAxis((angle), rotation);
		this->ArcballMatrix = glm::toMat4(quaternion);
		this->Radius = radius;
	}
	//setup with scalar values
	Arcball(GLfloat radius, GLfloat centerX, GLfloat centerY, GLfloat rotX, GLfloat rotY, GLfloat rotZ, GLfloat angle) :Zoom(0.0f), Speed(SPEED)
	{
		this->Center = glm::vec2(centerX, centerY);
		glm::quat quaternion = glm::angleAxis((angle), glm::vec3(rotX, rotY, rotZ));
		this->ArcballMatrix = glm::toMat4(quaternion);
		this->Radius = radius;
	}
	
	//returns the arcball matrix
	glm::mat4 GetArcballMatrix()
	{
		return this->ZoomMatrix * this->ArcballMatrix;
	}
	
	void mouse_down(Mouse_State state)
	{
		if(state == LEFTBUTTON_DOWN)
			is_rotate = true;
		if(state == RIGHTBUTTON_DOWN)
			is_zooming = true;
	}
	
	glm::mat4 mouse_motion(GLfloat lastX, GLfloat lastY, GLfloat x, GLfloat y)
	{
		glm::quat rotationQuat;
		glm::mat4 tempMatrix;
		
		if(is_rotate){
			glm::vec3 lastVec, Vec;
			lastVec = computeVector(lastX, lastY);
			Vec = computeVector(x, y);
			GLfloat cosAngle = glm::dot(lastVec, Vec);
			if (cosAngle > 0.999999){ return tempMatrix; }
			glm::vec3 rotationAxis = glm::normalize(glm::cross(lastVec, Vec));
			rotationQuat = glm::angleAxis(glm::degrees(glm::acos(cosAngle)), rotationAxis);
			tempMatrix = glm::mat4_cast(rotationQuat);
			ArcballMatrix = tempMatrix * ArcballMatrix;
		}
		if(is_zooming){
			this->Zoom += (y - lastY) * this->Speed;
			if (this->Zoom < 0.1)this->Zoom = 0.1;
			glm::mat4 zoom;
			ZoomMatrix = glm::scale(zoom, glm::vec3(this->Zoom));
			tempMatrix = ZoomMatrix * tempMatrix;
		}
		
		return tempMatrix;
	}
	
	void mouse_up(Mouse_State state)
	{
		if(state == LEFTBUTTON_UP)
			is_rotate = false;
		if(state == RIGHTBUTTON_UP)
			is_zooming = false;		
	}

	//update the parameter when window size changed
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
	glm::mat4 ZoomMatrix;
	GLfloat Radius;
	GLfloat Zoom;
	GLfloat Speed;
	
	//model options
	bool is_zooming;
	bool is_rotate;
	
	//calculate the vector of the point
	glm::vec3 computeVector(GLfloat x, GLfloat y)
	{
		x = x - this->Center.x;
		y = y - this->Center.y;
		GLfloat z = (this->Radius)*(this->Radius) - x*x - y*y;
		return (z > 0) ? glm::normalize((glm::vec3(x, -y, glm::sqrt(z)))) : glm::normalize((glm::vec3(x, -y, 0)));
	}
}