#pragma once
//Include and link appropriate libraries and headers//
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include "GlobalDX.h"

class Scene
{
public:
	
	Scene() {};
	~Scene() = default;

	bool InitScene();
	void UpdateScene(double time);
	void DrawScene();

	void getRot(float X, float Z);
	void getScale(float Xscale, float Yscale);

	XMMATRIX WVP;
private:

	float rotx = 0;
	float rotz = 0;
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	XMMATRIX Rotationx;
	XMMATRIX Rotationz;
	///////////////**************new**************////////////////////\\

	
	XMMATRIX cube1World;
	XMMATRIX cube2World;
	XMMATRIX camView;
	XMMATRIX camProjection;

	XMMATRIX d2dWorld;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	XMMATRIX Rotation;
	XMMATRIX Scale;
	XMMATRIX Translation;
	float rot = 0.01f;
};

