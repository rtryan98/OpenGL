#include <Base.h>

constexpr unsigned int width = 1600;
constexpr unsigned int height = 900;
constexpr char* title = "Framebuffers";

Window window{ width, height, title };

void initWindow()
{
	window.preInit(4, 6);
	window.create();
	window.postInit();
}

Shader triangleShader{};
Shader quadShader{};

void initShaders()
{
	triangleShader.create();
	triangleShader.addShader("res/shader/triangle.vert.glsl", GL_VERTEX_SHADER);
	triangleShader.addShader("res/shader/triangle.frag.glsl", GL_FRAGMENT_SHADER);
	triangleShader.compile();
	quadShader.create();
	quadShader.addShader("res/shader/quad.vert.glsl", GL_VERTEX_SHADER);
	quadShader.addShader("res/shader/quad.frag.glsl", GL_FRAGMENT_SHADER);
	quadShader.compile();
}

uint32_t triangleVao{};
uint32_t quadVao{};

void initVertexArrays()
{
	// triangle vao
	glCreateVertexArrays(1, &triangleVao);
	// position
	glEnableVertexArrayAttrib(triangleVao, 0);
	glVertexArrayAttribFormat(triangleVao, 0, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float));
	glVertexArrayAttribBinding(triangleVao, 0, 0);

	// quad vao
	glCreateVertexArrays(1, &quadVao);
	// position
	glEnableVertexArrayAttrib(quadVao, 0);
	glVertexArrayAttribFormat(quadVao, 0, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float));
	glVertexArrayAttribBinding(quadVao, 0, 0);
	// texture coordinate
	glEnableVertexArrayAttrib(quadVao, 1);
	glVertexArrayAttribFormat(quadVao, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
	glVertexArrayAttribBinding(quadVao, 1, 0);
}

uint32_t triangleVbo{};
uint32_t quadVbo{};

constexpr float screenQuad[16]
{
	-1.0f, -1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 1.0f, 1.0f
};

constexpr float triangle[]
{
	-0.5f, -0.5f,
	 0.5f, -0.5f,
	 0.0f,  0.5f
};

void initVertexBuffers()
{
	// screenquad
	glCreateBuffers(1, &quadVbo);
	glNamedBufferStorage(quadVbo, 16 * sizeof(float), screenQuad, 0x0);
	// This quad is immutable by all means, thus flags is 0.
	glVertexArrayVertexBuffer(quadVao, 0, quadVbo, 0, 4 * sizeof(float));

	// triangle
	glCreateBuffers(1, &triangleVbo);
	glNamedBufferStorage(triangleVbo, 6 * sizeof(float), triangle, 0x0);
	// This triangle is immutable by all means, thus flags is 0.
	glVertexArrayVertexBuffer(triangleVao, 0, triangleVbo, 0, 2 * sizeof(float));
}

uint32_t fbo{};
uint32_t fboColorTextureAttachment{};
uint32_t rbo{};

void initFramebuffers()
{
	// create the framebuffer
	glCreateFramebuffers(1, &fbo);

	// create the color texture attachment
	glCreateTextures(GL_TEXTURE_2D, 1, &fboColorTextureAttachment);
	glTextureStorage2D(fboColorTextureAttachment, 1, GL_RGB8, width, height);
	glTextureParameteri(fboColorTextureAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(fboColorTextureAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// attach the texture to the framebuffer
	glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, fboColorTextureAttachment, 0);

	// create a render buffer object and the depth attachment
	glCreateRenderbuffers(1, &rbo);
	glNamedRenderbufferStorage(rbo, GL_DEPTH24_STENCIL8, width, height);
	glNamedFramebufferRenderbuffer(fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// check if the framebuffer is complete
	if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		Log::info("Framebuffer is complete!");
	}
	else
	{
		Log::warn("Framebuffer is incomplete!");
	}
}

int main(int argc, char* argv[])
{
	// initialize the window
	initWindow();
	// initialize debug callbacks
	OpenGL::initDebugCallback();
	// create and initialize the shaders
	initShaders();
	// create and initialize the vertex array objects
	initVertexArrays();
	// create and initialize the vertex buffers
	initVertexBuffers();
	// create the framebuffers
	initFramebuffers();
	// loop
	while (!window.isCloseRequested())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		triangleShader.bind();
		glBindVertexArray(triangleVao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		triangleShader.unbind(); // redundant call, but used for clarity

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// draw the screenquad with the scene
		quadShader.bind();
		glBindTextureUnit(0, fboColorTextureAttachment);
		glBindVertexArray(quadVao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glBindTextureUnit(0, 0);
		quadShader.unbind();
		// swap buffers and poll events
		window.render();
		if (window.isKeyPressed(GLFW_KEY_ESCAPE))
		{
			window.close();
		}
	}
	// cleanup
	glDeleteBuffers(1, &quadVbo);
	glDeleteBuffers(1, &triangleVbo);
	glDeleteVertexArrays(1, &quadVao);
	glDeleteVertexArrays(1, &triangleVao);
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &fboColorTextureAttachment);
}
