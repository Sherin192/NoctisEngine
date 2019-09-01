#include "ExampleDx11App.h"

#if defined WIN32

int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	noctis::App* app = new noctis::ExampleDx11App(hInstance);
	if (!app->Init())
		return 1;

	app->Run();

	delete(app);
	return 0;
}
#elif __linux__

int main()
{
	return 0;
}

#endif //WIN32