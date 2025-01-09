#include <Windows.h>
#include <memory>
#include "myGame/MyGame.h"
#include "debugTools/logger/Logger.h"

// Lib
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#pragma comment(lib,"winmm.lib")

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma comment(lib,"xaudio2.lib")


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Logger::Log("Hello,SUGER!\n");

	static std::unique_ptr<SUGER> game = std::make_unique<MyGame>();
	game->Run();


	Logger::Log("Bye,SUGER...\n");

	return 0;
}