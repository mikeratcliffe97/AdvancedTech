#pragma once
//Include and link appropriate libraries and headers//
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")
///////////////**************new**************////////////////////
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
///////////////**************new**************////////////////////

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include <D3D10_1.h>
#include <DXGI.h>
#include <D2D1.h>
#include <sstream>
#include <dwrite.h>
///////////////**************new**************////////////////////
#include <dinput.h>
///////////////**************new**************////////////////////

//Global Declarations - Interfaces//
IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;
ID3D11Buffer* squareIndexBuffer;
ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;
ID3D11Buffer* squareVertBuffer;
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D11PixelShader* D2D_PS;
ID3D10Blob* VS_Buffer;
ID3D10Blob* PS_Buffer;
ID3D10Blob* D2D_PS_Buffer;
ID3D11InputLayout* vertLayout;
ID3D11Buffer* cbPerObjectBuffer;
ID3D11BlendState* Transparency;
ID3D11RasterizerState* CCWcullMode;
ID3D11RasterizerState* CWcullMode;
ID3D11ShaderResourceView* CubesTexture;
ID3D11SamplerState* CubesTexSamplerState;
ID3D11Buffer* cbPerFrameBuffer;

ID3D10Device1* d3d101Device;
IDXGIKeyedMutex* keyedMutex11;
IDXGIKeyedMutex* keyedMutex10;
ID2D1RenderTarget* D2DRenderTarget;
ID2D1SolidColorBrush* Brush;
ID3D11Texture2D* BackBuffer11;
ID3D11Texture2D* sharedTex11;
ID3D11Buffer* d2dVertBuffer;
ID3D11Buffer* d2dIndexBuffer;
ID3D11ShaderResourceView* d2dTexture;
IDWriteFactory* DWriteFactory;
IDWriteTextFormat* TextFormat;

///////////////**************new**************////////////////////
IDirectInputDevice8* DIKeyboard;
IDirectInputDevice8* DIMouse;
///////////////**************new**************////////////////////

std::wstring printText;

//Global Declarations - Others//
LPCTSTR WndClassName = "AdvancedTech";
HWND hwnd = NULL;
HRESULT hr;

const int Width = 900;
const int Height = 900;

///////////////**************new**************////////////////////
DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;

float rotx = 0;
float rotz = 0;
float scaleX = 1.0f;
float scaleY = 1.0f;

float tranX = 0;
float tranY = 0;
float tranZ = -4.0f;
int placementmode = 0;
XMMATRIX Rotationx;
XMMATRIX Rotationz;
///////////////**************new**************////////////////////\\

XMMATRIX WVP;
XMMATRIX cube1World;
XMMATRIX cube2World;
XMMATRIX camView;
XMMATRIX camProjection;

XMMATRIX d2dWorld;

XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;

XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

XMMATRIX camRotationMatrix;
XMMATRIX groundWorld;

float moveLeftRight = 0.0f;
float moveBackForward = 0.0f;

float camYaw = 0.0f;
float camPitch = 0.0f;

XMMATRIX Rotation;
XMMATRIX Scale;
XMMATRIX Translation;
float rot = 0.01f;

double countsPerSecond = 0.0;
__int64 CounterStart = 0;

int frameCount = 0;
int fps = 0;

__int64 frameTimeOld = 0;
double frameTime;

//Function Prototypes//
bool InitializeDirect3d11App(HINSTANCE hInstance);
void CleanUp();
bool InitScene();
void DrawScene();
bool InitD2D_D3D101_DWrite(IDXGIAdapter1* Adapter);
void InitD2DScreenTexture();
void UpdateScene(double time);
void UpdateCamera();
void RenderText(std::wstring text, int inInt);

void StartTimer();
double GetTime();
double GetFrameTime();

bool InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool windowed);
int messageloop();

///////////////**************new**************////////////////////
bool InitDirectInput(HINSTANCE hInstance);
void DetectInput(double time);
///////////////**************new**************////////////////////

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

//Create effects constant buffer's structure//
struct cbPerObject
{
	XMMATRIX  WVP;
	XMMATRIX  World;
};

cbPerObject cbPerObj;

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

Light light;

struct cbPerFrame
{
	Light  light;
};

cbPerFrame constbuffPerFrame;

//Vertex Structure and Vertex Layout (Input Layout)//
struct Vertex    //Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
UINT numElements = ARRAYSIZE(layout);