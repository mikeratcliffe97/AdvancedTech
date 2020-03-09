#include <windows.h>
#include <memory>
#include "Game.h"


LRESULT CALLBACK WndProc(HWND WindowHandle, UINT message, WPARAM wParam, LPARAM lParam);	//Tell Windows what the "callback procedure" is to process events.


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)	//<--Code Begins Here.
{
	int ReturnValue = -1;	//-1 is a failure.
	HWND WindowHandle;		//Windows uses handles to keep track of windows.
	MSG msg = { 0 };			//Windows Event.
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	Game Game1;	//Instantiates an instance of our Game class. Only one Game per window (single instance).

	WNDCLASSEX wndClass = { 0 };		//A window "class" has to be defined before a Windows window is created. (See WIN32 programming.)
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "BareBones2DGameCodeClass";


	if (RegisterClassEx(&wndClass))
	{
		RECT rc = { 0, 0, 1176, 674 };	//Set screen resolution.
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		WindowHandle = CreateWindowA("BareBones2DGameCodeClass", "My Game", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
			NULL, NULL, hInstance, NULL);

		if (WindowHandle)
		{
			ShowWindow(WindowHandle, cmdShow);	//Show the window on the screen.

			if (Game1.InitializeDX(hInstance, WindowHandle))	//Initialize DirectX11 and call our Intialize and LoadContent methods.
			{
				while (msg.message != WM_QUIT)	//Keep going forever until Windows sends a quit event.
				{
					if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))	//Handle Windows Event Messages.
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}

					// Update and Draw
					Game1.Update(0.0f);		//Call our Game's Update method.
					if (Game1.GraphicsCardDeviceContext != NULL)	//Make sure we actually have a device context to draw with.
					{
						Game1.Draw();			//Call our Game's Draw method.
					}
					else
					{
						//This should never happen unless no graphics device can be found or something pretty major went wrong.
						//If it does however, its time to crash the program gracefully since we can't draw.
						PostQuitMessage(0);	//Send a Windows event to shut down this program.
					}
				}

				Game1.Shutdown();	//Shutdown DirectX and cleanup before letting the window shutdown.

				ReturnValue = static_cast<int>(msg.wParam);
			}
		}
	}


	return ReturnValue;
}


LRESULT CALLBACK WndProc(HWND WindowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;

	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(WindowHandle, &paintStruct);
		EndPaint(WindowHandle, &paintStruct);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(WindowHandle, message, wParam, lParam);
	}

	return 0;
}
