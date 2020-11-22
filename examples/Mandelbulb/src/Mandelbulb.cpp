#include <Base.h>

constexpr unsigned int width = 1600;
constexpr unsigned int height = 900;
constexpr char* title = "Mandelbulb";

Window window{ width, height, title };

void initWindow()
{
    window.preInit(4, 6);
    window.create();
    window.postInit();
}

Shader raycastComputeShader{};
Shader raycastShader{};

void initShaders()
{
    raycastShader.create();
    raycastShader.addShader("mandelbulb/res/shader/mandelbulb.vert", GL_VERTEX_SHADER);
    raycastShader.addShader("mandelbulb/res/shader/mandelbulb.frag", GL_FRAGMENT_SHADER);
    raycastShader.compile();
    raycastComputeShader.create();
    raycastComputeShader.addShader("mandelbulb/res/shader/mandelbulb.comp", GL_COMPUTE_SHADER);
    raycastComputeShader.compile();
}

uint32_t quadVao{};

void initVertexArrays()
{
    glCreateVertexArrays(1, &quadVao);
    glEnableVertexArrayAttrib(quadVao, 0);
    glVertexArrayAttribFormat(quadVao, 0, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float));
    glVertexArrayAttribBinding(quadVao, 0, 0);
    glEnableVertexArrayAttrib(quadVao, 1);
    glVertexArrayAttribFormat(quadVao, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
    glVertexArrayAttribBinding(quadVao, 1, 0);
}

uint32_t quadVbo{};
constexpr float screenQuad[16]
{
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 1.0f, 1.0f
};

void initVertexBuffers()
{
    glCreateBuffers(1, &quadVbo);
    glNamedBufferStorage(quadVbo, 16 * sizeof(float), screenQuad, 0x0);
        // This quad is immutable by all means, thus flags is 0.
    glVertexArrayVertexBuffer(quadVao, 0, quadVbo, 0, 4 * sizeof(float));
}

uint32_t quadTexture{};

void initTexture()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &quadTexture);
    glTextureParameteri(quadTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(quadTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(quadTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(quadTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(quadTexture, 1, GL_RGBA32F, width, height);
    raycastShader.setUniform1i("uRaycast", 0);
}

Camera camera{};

void updateCameraUniforms()
{
    raycastComputeShader.setUniform3f("camera.position", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    raycastComputeShader.setUniform3f("camera.front", camera.getFront().x, camera.getFront().y, camera.getFront().z);
    raycastComputeShader.setUniform3f("camera.up", camera.getUp().x, camera.getUp().y, camera.getUp().z);
    raycastComputeShader.setUniform3f("camera.right", camera.getRight().x, camera.getRight().y, camera.getRight().z);
}

int main(int argc, char* argv[])
{
    // initialize the program
    initWindow();
    OpenGL::initDebugCallback();
    initShaders();
    initVertexArrays();
    initVertexBuffers();
    initTexture();
    // initialize screen and camera
    camera.init(90.0f, {-1.0f, 0.0f, 0.0f}, 0.75f, 0.15f);
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    const float screenFovY = glm::tan(camera.getFov() / 180.0f * glm::atan(1.0f));
    const float screenFovX = (aspect * screenFovY);
    raycastComputeShader.setUniform1f("screen.fovX", screenFovX);
    raycastComputeShader.setUniform1f("screen.fovY", screenFovY);
    // framebuffer clearcolor
    float clearColor[]
    {
        0.0f, 0.0f, 0.0f, 1.0f
    };
    // loop
    float currentTime{};
    float lastTime{};
    float dt{};
    while (!window.isCloseRequested())
    {
        // update dt
        currentTime = (float) glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;
        // clear screen
        glClearNamedFramebufferfv(0, GL_COLOR, 0, clearColor);
        // update the camera
        camera.update(window, dt);
        updateCameraUniforms();
        // draw to the texture with compute
        // bind the texture as an image texture so it can be used as a writeonly uniform image2D
        glBindImageTexture(0, quadTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        // bind the shader and dispatch the compute
        raycastComputeShader.setUniform1f("power", 2.0f + glfwGetTime() * 0.15f);
        raycastComputeShader.bind();
        raycastComputeShader.dispatch((width / 32) + ((width % 32 != 0) ? 1 : 0), (height / 32) + ((height % 32 != 0) ? 1 : 0), 1);
        // unbind the image texture
        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        // wait until compute is finished
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        // bind everything else and draw the quad
        raycastShader.bind();
        glBindTextureUnit(0, quadTexture);
        glBindVertexArray(quadVao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // unbind everything
        glBindVertexArray(0);
        glBindTextureUnit(0, 0);
        raycastShader.unbind();
        window.render();
        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }
    }
    // cleanup
    glDeleteBuffers(1, &quadVbo);
    glDeleteVertexArrays(1, &quadVao);
}
