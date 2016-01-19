/**********************

HitTest.h

Detect whether a point/line/quad is hitted.
Whenever you want a primitive to be detect in hittest, push it into 
the HitTest vector, it will change it and your mouse position into 
the same space and then return whether it is hitted.
Before using a instance, you may need to use the function SetAttrib().

ZhuohanChen

**********************/

#ifndef HITTEST_H_
#define HITTEST_H_

//std includes
#include <vector>

//other includes
#include "glm/glm.hpp"

namespace GRAPHICUTILSHITTEST{
	enum ColliderType{
		POINT,
		LINE,
		QUAD
	};
};

class HitTest
{
public:
	struct collider{
		int id;
		std::string name;
		std::vector<glm::vec3 *> points;
		GRAPHICUTILSHITTEST::ColliderType type;
	};

	HitTest();
	~HitTest();
	
	oid SetAttrib(glm::mat4 ProjViewMatrix, float width, float height);
	
	//create a collider
	oid PushPoint(glm::vec3 &point, std::string name);
	oid PushLine(glm::vec3 &point1, glm::vec3 &point2, std::string name);
	oid PushQuad(
		glm::vec3 &point1, 
		glm::vec3 &point2, 
		glm::vec3 &point3, 
		glm::vec3 &point4, 
		std::string name
	);
	
	//detect & return the collider
	int GetHitId(glm::vec2 mouse);
	std::string GetHitName(glm::vec2 mouse);
	bool IsHit(std::string name);
		
	//update the collider
	void UpdatePoint(glm::vec3 point, std::string name);
	void UpdateLine(glm::vec3 point1, glm::vec3 point2, std::string name);
	void UpdateQuad(
		glm::vec3 point1,
		glm::vec3 point2,
		glm::vec3 point3,
		glm::vec3 point4,
		std::string name
		);
	
private:
	float m_fDeviation;
	glm::mat4 m_matProjView;
	float m_fWIDTH;
	float m_fHEIGHT;

	std::vector<collider> m_vecColliders;
	
	float __IsPointHit(glm::vec2 mouse, collider point);
	float __IsLineHit(glm::vec2 mouse, collider line);
	float __IsQuadHit(glm::vec2 mouse, collider quad);
};

#endif