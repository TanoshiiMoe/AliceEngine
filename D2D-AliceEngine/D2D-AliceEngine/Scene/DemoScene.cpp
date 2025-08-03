#include "DemoScene.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/VideoComponent.h>
#include <Component/Spine2DRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include "../Scripts/Sun.h"
#include "../Scripts/Moon.h"
#include "../Scripts/Earth.h"
#include "../Scripts/YuukaVideo.h"
#include "../Scripts/CameraController.h"

/*
*	NewObject<T>(std::wstring&) : �ش� �̸��� ���ӿ�����Ʈ�� �����ϰ� rawPointer�� ��ȯ�մϴ�.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : ��ǥ, ȸ��, ������, �Ǻ��� �����մϴ�.
*/

void DemoScene::Initialize()
{
	__super::Initialize();
}

void DemoScene::Release()
{
	__super::Release();
}

void DemoScene::Update()
{
	__super::Update();
}

void DemoScene::OnEnter()
{
	__super::OnEnter();

	m_camera = NewObject<gameObject>(L"Camera");
	m_camera->AddComponent<CameraController>();

	m_yuuka = NewObject<gameObject>(L"yuuka");
	m_yuuka->AddComponent<YuukaVideo>();

	m_sun = NewObject<gameObject>(L"Sun");
	m_sun->AddComponent<Sun>();

	m_earth = NewObject<gameObject>(L"Earth");
	m_earth->AddComponent<Earth>();

	m_moon = NewObject<gameObject>(L"Moon");
	m_moon->AddComponent<Moon>();

	m_widget = NewObject<gameObject>(L"widget");
	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L" <ī�޶�> \n"
		L" [w,s] : ī�޶� ���� �̵� \n"
		L" [a,d] : ī�޶� �¿� �̵� \n"
		L" [ [ / ] ] : ī�޶� ���, Ȯ��\n"
		L" [1/2] : D2D, Unity ��ǥ�� \n"
		L"\n"
		L" <�¾�, ����, ��> \n"
		L" [z,c] : �¾� ȸ�� \n"
		L" [ȭ��ǥ ��,��] : �¾� ���� �̵� \n"
		L" [ȭ��ǥ ��/��] : �¾� �¿� �̵� \n"
		L" [4] : �¾� �̸� �ѿ� ��ȯ \n"
		L" [b,m] : ���� ȸ�� \n"
		L" [y,h] : ���� ���� �̵� \n"
		L" [g,j] : ���� �¿� �̵� \n"
		L" [9,0] : �� ȸ�� \n"
		L" [o,l] : �� ���� �̵� \n"
		L" [k,;] : �� �¿� �̵� \n"
		L"\n"
		L" <��� ����> \n"
		L" [P] : ��� ����, ���� \n"
		L"\n"
		L" <�ִϸ��̼�> \n"
		L" [5,6] : �ƽ���, ����ī ��ȯ"
	);

	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget2->transform()->SetPosition(0, 0);

	m_widget3 = NewObject<gameObject>(L"widget3");
	m_widget3->transform()->SetPosition(0, 0);

	m_sun->AddChildObject(m_earth);
	m_earth->AddChildObject(m_moon);
	/*
	* UI�� gameObject�� ������ �и��ؼ� ��������. ���Ŀ� ���� 
	*/
	m_widget->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(20, 80));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(18.0f);

	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <��> \n [3] : �� ��ȯ");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(850, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <���� ��> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
}

void DemoScene::OnExit()
{
	__super::OnExit();
}
