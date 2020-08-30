#include <Base.h>

constexpr unsigned int width = 1280;
constexpr unsigned int height = 720;
constexpr char* title = "Blur";

int main(int argc, char* argv[])
{
	Window window{width, height, title};
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
