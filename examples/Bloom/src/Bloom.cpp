#include <Base.h>

constexpr unsigned int width = 1280;
constexpr unsigned int height = 720;
constexpr char* title = "Bloom";

int main(int argc, char* argv[])
{
	Window window{width, height, title};
	window.preInit(4, 6);
	window.create();
	window.postInit();

	OpenGL::initDebugCallback();

	Shader screenQuadShader{};
	screenQuadShader.addShader("res/shader/quad.vert.glsl", GL_VERTEX_SHADER);
	screenQuadShader.addShader("res/shader/quad.frag.glsl", GL_FRAGMENT_SHADER);
	screenQuadShader.compile();

	GLuint screenQuadVao{};
	glCreateVertexArrays(1, &screenQuadVao);
	glEnableVertexArrayAttrib(screenQuadVao, 0);
	glVertexArrayAttribFormat(screenQuadVao, 0, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float));
	glVertexArrayAttribBinding(screenQuadVao, 0, 0);
	glEnableVertexArrayAttrib(screenQuadVao, 1);
	glVertexArrayAttribFormat(screenQuadVao, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
	glVertexArrayAttribBinding(screenQuadVao, 1, 0);

	GLuint screenQuadVbo{};
	glCreateBuffers(1, &screenQuadVbo);
	GLfloat screenQuad[16]
	{
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	glNamedBufferStorage(screenQuadVbo, 16 * sizeof(GLfloat), screenQuad, 0);
	glVertexArrayVertexBuffer(screenQuadVao, 0, screenQuadVbo, 0, 4 * sizeof(GLfloat));

	while (!window.isCloseRequested())
	{


		// draw the screenquad
		screenQuadShader.bind();
		glBindVertexArray(screenQuadVao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		screenQuadShader.unbind();

		window.render();
		if (window.isKeyPressed(GLFW_KEY_ESCAPE))
		{
			window.close();
		}
	}

	glDeleteBuffers(1, &screenQuadVbo);
	glDeleteVertexArrays(1, &screenQuadVao);

	return 0;
}
