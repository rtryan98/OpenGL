#include <Base.h>

int main(int argc, char* argv[])
{
	Window window{ 1600, 900, "Hello Model!" };
	window.preInit(4, 6);
	window.create();
	window.postInit();
	while (!window.isCloseRequested())
	{
		window.render();
		if (window.isKeyPressed(GLFW_KEY_ESCAPE))
		{
			window.close();
		}
	}
	return 0;
}
