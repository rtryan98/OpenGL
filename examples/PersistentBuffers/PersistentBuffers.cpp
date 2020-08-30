#include <Base.h>

int main()
{
	Window window{1280, 720, "Persistent Buffers"};
	window.preInit();
	window.create();
	window.postInit();
	while (!window.isCloseRequested())
	{
		window.render();
	}
	return 0;
}
