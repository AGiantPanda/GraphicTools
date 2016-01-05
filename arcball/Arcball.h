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
- set your camera looking at (0.0, 0.0, 0.0), set it's
  location somewhere at zaxis.
- draw your object at the center of the world space (which
  is (0.0, 0.0, 0.0)), or translate it yourself.
--------------------------------------------------

Oct. 20, 2015 - ZhuohanChen (pandachen0611@gmail.com)

*************************************************/

#pragma once

//std includes

//gl includes
#include <glm.hpp>

namespace ARCBALL_Opr{
enum Opr_State {
	ROTATE_ON,
	ROTATE_OFF,
	SCALE_ON,
	SCALE_OFF,
	TRANSLATE_ON,
	TRANSLATE_OFF
};
}

class Arcball
{
public:
	//setup with vectors
	Arcball(float radius, glm::vec2 center);
	//setup with scalar values
	Arcball(float radius, float centerX, float centerY);
	
	//returns the arcball matrix
	glm::mat4 GetArcballMatrix();
	
	void Model_opr(ARCBALL_Opr::Opr_State state);
	void Model_opr_off(ARCBALL_Opr::Opr_State state);
	
	glm::mat4 motion(glm::vec2 preP, glm::vec2 nowP);

	//update the parameter when window size changed
	void setParam(float radius, glm::vec2 center);
	void setSpeed(float speed);
	
private:
	//model attributes
	glm::mat4 ArcbMatrix;
	glm::mat4 ScalMatrix;
	glm::mat4 TranMatrix;
	glm::vec2 Center;
	float Radius;
	float Scale;
	float Speed;
	
	//model options
	bool is_scale;
	bool is_rotate;
	bool is_translate;
	
	//calculate the vector of the point
	glm::vec3 computeVector(float x, float y);
};