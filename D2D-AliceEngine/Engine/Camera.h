#pragma once
#include "Object.h"

/*
* @brief 카메라 클래스. 
* @details 현재는 Object를 상속받아 기본적인 기능만 구현되어 있습니다.
*/

class Camera : public Object
{
public:
	Camera() {};
	~Camera() {};

	void Initialize() override;
	void LoadBitmapData(const std::wstring& path) override;
	void Release() override;
	void Render() override;
};

