#include "pch.h"

#if defined(_DEBUG)
#include "vld.h"
#endif

#undef main
#include "Renderer.h"

#include "HelperFuncts.h"

#include <Windows.h>

using namespace dae;

void enableColors()
{
	// Got this from Sander De Keukelaere
	DWORD consoleMode;
	const HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleMode(outputHandle, &consoleMode))
	{
		SetConsoleMode(outputHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	}
}

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	enableColors();

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;

	SDL_Window* pWindow = SDL_CreateWindow(
		"DualRasterizer - Re� Messely/2DAE15",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	const auto pTimer = new Timer();
	const auto pRenderer = new Renderer(pWindow);

	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool displayFPS = false;
	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_F1:
					pRenderer->ToggleBetweenHardwareSoftware();
					break;
				case SDL_SCANCODE_F2:
					pRenderer->ToggleRotation();
					break;
				case SDL_SCANCODE_F3:
					pRenderer->ToggleFireFXMesh();
					break;
				case SDL_SCANCODE_F4:
					pRenderer->ToggleTextureSamplingStates();
					break;
				case SDL_SCANCODE_F5:
					pRenderer->CycleShadingMode();
					break;
				case SDL_SCANCODE_F6:
					pRenderer->ToggleNormalMap();
					break;
				case SDL_SCANCODE_F7:
					pRenderer->ToggleDepthBufferVisualisation();
					break;
				case SDL_SCANCODE_F8:
					pRenderer->ToggleBoundingBoxVisualisation();
					break;
				case SDL_SCANCODE_F9:
					pRenderer->CycleCullModes();
					break;
				case SDL_SCANCODE_F10:
					pRenderer->ToggleUniformClearColor();
					break;
				case SDL_SCANCODE_F11:
					displayFPS = !displayFPS;
					std::wcout << YELLOW << L"[FPS DISPLAY] " << (displayFPS ? L"Enabled" : L"Disabled") << '\n' << RESET;
					break;
				}
				break;
			default: ;
			}
		}

		//--------- Update ---------
		pRenderer->Update(pTimer);

		//--------- Render ---------
		pRenderer->Render();

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			if (displayFPS)
				std::cout << WHITE << "dFPS: " << pTimer->GetdFPS() << '\n' << RESET;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}