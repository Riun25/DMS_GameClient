#define  _CRTDBG_MAP_ALLOC
#include "GameEngine/DearsGameEngineAPI.h"
#include "GameProcessor.h"
#include "resource.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::unique_ptr<GameProcessor> application = std::make_unique<GameProcessor>(hInstance, L"DMS_Merc", 10);

	application->Initialize();
	application->Run();
	//application->Finalize();

	return 0;
}