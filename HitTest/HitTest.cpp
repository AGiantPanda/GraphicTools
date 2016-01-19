/**********************

HitTest.cpp

detect whether a point/line/quad is hitted.

ZhuohanChen

**********************/

#include "HitTest.h"

HitTest::HitTest(): m_fDeviation(1.0f)
{
	//m_matProjView = ProjViewMatrix;
}

HitTest::~HitTest()
{
	
}

//Push the point you want to detect into the collider vector. 
//And give it a name.
void HitTest::PushPoint(glm::vec3 &point, std::string name)
{
	collider tmp;
	tmp.name = name;
	tmp.type = HitTest::POINT;
	tmp.points.push_back(&point);
	m_vecColliders.push_back(tmp);
	m_vecColliders[m_vecColliders.size() - 1].id = m_vecColliders.size();
}

//Push the line you want to detect into the collider vector. 
//And give it a name.
void HitTest::PushLine(glm::vec3 &point1, glm::vec3 &point2, std::string name)
{
	collider tmp;
	tmp.name = name;
	tmp.type = HitTest::LINE;
	tmp.points.push_back(&point1);
	tmp.points.push_back(&point2);
	m_vecColliders.push_back(tmp);
	m_vecColliders[m_vecColliders.size() - 1].id = m_vecColliders.size();
}

//Push the quad you want to detect into the collider vector. 
//And give it a name.
void HitTest::PushQuad(
	glm::vec3 &point1,
	glm::vec3 &point2,
	glm::vec3 &point3,
	glm::vec3 &point4,
	std::string name
)
{
	collider tmp;
	tmp.name = name;
	tmp.type = HitTest::QUAD;
	tmp.points.push_back(&point1);
	tmp.points.push_back(&point2);
	tmp.points.push_back(&point3);
	tmp.points.push_back(&point4);
	m_vecColliders.push_back(tmp);
	m_vecColliders[m_vecColliders.size() - 1].id = m_vecColliders.size();
}

//Set the ProjViewMatrix, width and the height.
//This should be set before the detection.
void HitTest::SetAttrib(glm::mat4 ProjViewMatrix, float width, float height)
{
	m_matProjView = ProjViewMatrix;
	m_fWIDTH = width;
	m_fHEIGHT = height;
}

//Input the current mouse position and it will return the hitted id.
//Return 0 when nothing hitted.
int HitTest::GetHitId(glm::vec2 mouse)
{
	int id = 0;
	float depth = 0.0; float tmp_d = 0.0f;
	for(int i = 0; i < m_vecColliders.size(); ++i){
		switch(m_vecColliders[i].type){
		case HitTest::POINT:
			 tmp_d = __IsPointHit(mouse, m_vecColliders[i]);
			if(tmp_d > 0.0){
				if (depth == 0.0){
					id = i + 1;
					depth = tmp_d;
				}
				else{
					id  = (depth > tmp_d) ? i + 1 : id;
				}
			}
			break;
		case HitTest::LINE:
			tmp_d = __IsLineHit(mouse, m_vecColliders[i]);
			if(tmp_d != 0.0){
				if (depth == 0.0){
					id = i + 1;
					depth = tmp_d;
				}
				else{
					id = (depth > tmp_d) ? i + 1 : id;
				}
			}
			break;
		case HitTest::QUAD:
			tmp_d = __IsQuadHit(mouse, m_vecColliders[i]);
			if (tmp_d != 0.0){
				if (depth == 0.0){
					id = i + 1;
					depth = tmp_d;
				}
				else{
					id = (depth > tmp_d) ? i + 1 : id;
				}
			}
			break;
		default:
			return 0;
		}
	}
	return id;
}

//Return the name of the hitted primitive, "" if nothing hitted.
std::string HitTest::GetHitName(glm::vec2 mouse)
{
	int id = GetHitId(mouse);
	if (id > 0)
		return m_vecColliders[id - 1].name;
	else
		return "";
}

//Return true if the given name primitve is hitted.
bool HitTest::IsHit(std::string name)
{
	return false;
}

//TODO(chenzhuohan@ihefe.com): update the certain point
void HitTest::UpdatePoint(glm::vec3 point, std::string name)
{
	
}

//TODO(chenzhuohan@ihefe.com): update the certain line
void HitTest::UpdateLine(glm::vec3 point1, glm::vec3 point2, std::string name)
{
	
}

//TODO(chenzhuohan@ihefe.com): update the certain quad
void HitTest::UpdateQuad(
	glm::vec3 point1, 
	glm::vec3 point2, 
	glm::vec3 point3, 
	glm::vec3 point4, 
	std::string name
)
{
	
}

//Translate the point into the screen space, compare it with the mouse 
//location. If more than one points is hitted, return the forward one.
//Return 0 if no point is hitted.
float HitTest::__IsPointHit(glm::vec2 mouse, collider point)
{
	glm::vec4 viewPoint = m_matProjView * glm::vec4(*point.points[0], 1.0f);
	glm::vec2 screen = glm::vec2(viewPoint) / viewPoint.w;
	screen = glm::vec2(screen.x * m_fWIDTH / 2, screen.y*m_fHEIGHT / 2);
	if(glm::length(mouse -  screen) <= m_fDeviation)
		return viewPoint.z;

	return false;
}

float HitTest::__IsLineHit(glm::vec2 mouse, collider line)
{
	glm::vec4 viewPoint1 = m_matProjView * glm::vec4(*line.points[0], 1.0f);
	glm::vec4 viewPoint2 = m_matProjView * glm::vec4(*line.points[1], 1.0f);
	glm::vec2 screen1 = glm::vec2(viewPoint1) / viewPoint1.w;
	screen1 = glm::vec2(screen1.x * m_fWIDTH / 2, screen1.y*m_fHEIGHT / 2);
	glm::vec2 screen2 = glm::vec2(viewPoint2) / viewPoint2.w;
	screen2 = glm::vec2(screen2.x * m_fWIDTH / 2, screen2.y*m_fHEIGHT / 2);

	glm::vec2 len1 = screen2 - screen1;
	if (glm::length(len1) <= 2 * m_fDeviation ||
		(glm::abs(screen1.x - screen2.x) < 2 * m_fDeviation ? false : (screen1.x < screen2.x) ? 
		!(mouse.x >= screen1.x && mouse.x <= screen2.x) :
		!(mouse.x >= screen2.x && mouse.x <= screen1.x)) ||
		(glm::abs(screen1.y - screen2.y) < 2 * m_fDeviation ? false : (screen1.y < screen2.y) ?
		!(mouse.y >= screen1.y && mouse.y <= screen2.y) :
		!(mouse.y >= screen2.y && mouse.y <= screen1.y)))
		return false;

	float A, B, C;
	float dis;

	if (glm::abs(screen1.x - screen2.x) <= 0.0001f){
		dis = m_fDeviation - glm::abs(mouse.x - screen1.x);
	}else{
		A = (screen1.y - screen2.y) / (screen1.x - screen2.x);
		B = -1;
		C = -A*screen1.x + screen1.y;
		dis = A * mouse.x + B * mouse.y + C;
		dis = m_fDeviation * m_fDeviation * (A * A + B * B) - dis * dis;
	}

	if (dis >= 0.0f){
		//get the depth and return
		if (viewPoint1.z - viewPoint2.z <= 0.0001)
			return viewPoint1.z;
		else{
			mouse = glm::vec2(mouse.x / m_fWIDTH*2, mouse.y/m_fHEIGHT*2) * viewPoint1.w;
			if (glm::abs(viewPoint1.x - viewPoint2.x) <= 0.0001f){
				A = (viewPoint1.z - viewPoint2.z) / (viewPoint1.y - viewPoint2.y);
				C = -A*viewPoint1.y + viewPoint1.z;
				return A * mouse.y + C;
			}
			else{
				A = (viewPoint1.z - viewPoint2.z) / (viewPoint1.x - viewPoint2.x);
				C = -A*viewPoint1.x + viewPoint1.z;
				return A*mouse.x + C;
			}
		}
	}

	return false;
}

//TODO(chenzhuohan@ihefe.com): the algo to detect which quad should return need
//to optimized.
float HitTest::__IsQuadHit(glm::vec2 mouse, collider quad)
{
	glm::vec4 viewPoint1 = m_matProjView * glm::vec4(*quad.points[0], 1.0f);
	glm::vec4 viewPoint2 = m_matProjView * glm::vec4(*quad.points[1], 1.0f);
	glm::vec4 viewPoint3 = m_matProjView * glm::vec4(*quad.points[2], 1.0f);
	glm::vec4 viewPoint4 = m_matProjView * glm::vec4(*quad.points[3], 1.0f);
	glm::vec2 screen1 = glm::vec2(viewPoint1) / viewPoint1.w;
	screen1 = glm::vec2(screen1.x * m_fWIDTH / 2, screen1.y*m_fHEIGHT / 2);
	glm::vec2 screen2 = glm::vec2(viewPoint2) / viewPoint2.w;
	screen2 = glm::vec2(screen2.x * m_fWIDTH / 2, screen2.y*m_fHEIGHT / 2);
	glm::vec2 screen3 = glm::vec2(viewPoint3) / viewPoint3.w;
	screen3 = glm::vec2(screen3.x * m_fWIDTH / 2, screen3.y*m_fHEIGHT / 2);
	glm::vec2 screen4 = glm::vec2(viewPoint4) / viewPoint4.w;
	screen4 = glm::vec2(screen4.x * m_fWIDTH / 2, screen4.y*m_fHEIGHT / 2);

	glm::vec3 len1 = glm::vec3(screen2 - screen1, 0.0f);
	glm::vec3 len2 = glm::vec3(screen3 - screen2, 0.0f);
	glm::vec3 len3 = glm::vec3(screen4 - screen3, 0.0f);
	glm::vec3 len4 = glm::vec3(screen1 - screen4, 0.0f);
	if (glm::length(len1) <= 2 * m_fDeviation ||
		glm::length(len2) <= 2 * m_fDeviation)
		return false;
	
	int b = -1;
	if (glm::cross(len1, glm::vec3(mouse - screen1, 0.0f)).z >= 0.0)
		b = 1;
	bool line2 = b*glm::cross(len2, glm::vec3(mouse - screen2, 0.0f)).z >= 0.0;
	bool line3 = b*glm::cross(len3, glm::vec3(mouse - screen3, 0.0f)).z >= 0.0;
	bool line4 = b*glm::cross(len4, glm::vec3(mouse - screen4, 0.0f)).z >= 0.0;
	if (line2 &&
		 line3&&
		line4)
		return (viewPoint1.z + viewPoint2.z + viewPoint3.z + viewPoint4.z) / 4;
	return false;
}