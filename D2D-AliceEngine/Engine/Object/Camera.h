#pragma once
#include "Object/Object.h"
#include <Math/Transform.h>

/*
* @brief 카메라 클래스. 
* @details 현재는 Object를 상속받아 기본적인 기능만 구현되어 있습니다.
*/

class Camera : public Object
{
public:
	Camera() {};
	~Camera() {};

	void Initialize();
	void Update();
	void Release();

	std::shared_ptr<Transform> m_transform;
};

