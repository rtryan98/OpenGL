#include <cstdio>
#include <Base.h>
#include <vector>
#include <cstdlib>
#include <random>
#include <glm/gtx/common.hpp>

constexpr uint32_t width = 1920;
constexpr uint32_t height = 1080;
constexpr char* title = "FFT-Ocean";
Window window{ width, height, title };

Camera camera{};

uint32_t oceanVAO{};
uint32_t oceanVBO{};
uint32_t oceanIBO{};

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 uv;
};

std::vector<Vertex> oceanGeometry{};
std::vector<uint32_t> oceanIndices{};

void createGeometry()
{
    constexpr float_t size{ 0.25f };

    for (uint32_t i{ 0 }; i < 64; i++)
    {
        for (uint32_t j{ 0 }; j < 64; j++)
        {
            Vertex bl{ glm::vec3{static_cast<float_t>(i) * size + 0.0f, 0.0f, static_cast<float_t>(j) * size + 0.0f},
                glm::vec2{ static_cast<float_t>(i * 2.0f) / 128.0f, static_cast<float_t>(j * 2.0f) / 128.0f } };

            Vertex br{ glm::vec3{static_cast<float_t>(i) * size + size, 0.0f, static_cast<float_t>(j) * size + 0.0f},
                glm::vec2{ static_cast<float_t>(i * 2.0f + 2.0f) / 128.0f, static_cast<float_t>(j * 2.0f) / 128.0f } };

            Vertex tl{ glm::vec3{static_cast<float_t>(i) * size + size, 0.0f, static_cast<float_t>(j) * size + size},
                glm::vec2{ static_cast<float_t>(i * 2.0f + 2.0f) / 128.0f, static_cast<float_t>(j * 2.0f + 2.0f) / 128.0f } };

            Vertex tr{ glm::vec3{static_cast<float_t>(i) * size + 0.0f, 0.0f, static_cast<float_t>(j) * size + size},
                glm::vec2{ static_cast<float_t>(i * 2.0f) / 128.0f, static_cast<float_t>(j * 2.0f + 2.0f) / 128.0f } };

            oceanGeometry.push_back(bl);
            oceanGeometry.push_back(br);
            oceanGeometry.push_back(tl);
            oceanGeometry.push_back(tr);

            oceanIndices.push_back(0 + 64 * 4 * i + j * 4);
            oceanIndices.push_back(1 + 64 * 4 * i + j * 4);
            oceanIndices.push_back(2 + 64 * 4 * i + j * 4);
            oceanIndices.push_back(2 + 64 * 4 * i + j * 4);
            oceanIndices.push_back(3 + 64 * 4 * i + j * 4);
            oceanIndices.push_back(0 + 64 * 4 * i + j * 4);
        }
    }

    glCreateBuffers(1, &oceanVBO);
    glNamedBufferStorage(oceanVBO, sizeof(Vertex) * oceanGeometry.size(), oceanGeometry.data(), 0x0);

    glCreateBuffers(1, &oceanIBO);
    glNamedBufferStorage(oceanIBO, sizeof(uint32_t) * oceanIndices.size(), oceanIndices.data(), 0x0);

    glCreateVertexArrays(1, &oceanVAO);

    glEnableVertexArrayAttrib(oceanVAO, 0);
    glVertexArrayAttribFormat(oceanVAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribBinding(oceanVAO, 0, 0);

    glEnableVertexArrayAttrib(oceanVAO, 1);
    glVertexArrayAttribFormat(oceanVAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
    glVertexArrayAttribBinding(oceanVAO, 1, 0);

    glVertexArrayVertexBuffer(oceanVAO, 0, oceanVBO, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(oceanVAO, oceanIBO);
}

uint32_t randomTexture{};

void createGaussianRandomTexture()
{
    std::vector<uint8_t> textureData{};

    std::mt19937 generator{};
    std::normal_distribution<float_t> gaussRand{ 0.0f, 1.0f };

    for (uint32_t i{ 0 }; i < 256; i++)
    {
        for (uint32_t j{ 0 }; j < 256; j++)
        {
            float_t a{};
            float_t b{};
            do
            {
                a = gaussRand(generator);
            } while(a < 0.0f ||a > 1.0f);
            do
            {
                b = gaussRand(generator);
            } while (b < 0.0f || b > 1.0f);

            a = glm::floor(255.0f * a);
            b = glm::floor(255.0f * b);

            uint8_t aInt{ static_cast<uint8_t>(a) };
            uint8_t bInt{ static_cast<uint8_t>(b) };

            textureData.push_back(aInt);
            textureData.push_back(bInt);
        }
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glCreateTextures(GL_TEXTURE_2D, 1, &randomTexture);

    glTextureParameteri(randomTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(randomTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(randomTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(randomTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(randomTexture, 1, GL_RG8, 256, 256);
    glTextureSubImage2D(randomTexture, 0, 0, 0, 256, 256, GL_RG, GL_UNSIGNED_BYTE, textureData.data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

uint32_t debugVAO{};
uint32_t debugVBO{};

void createDebugGeometry()
{
    struct DebugVertex
    {
        glm::vec2 pos;
        glm::vec2 uv;
    };

    std::vector<DebugVertex> debugVertices{};
    debugVertices.push_back({ glm::vec2{0.0f, 0.0f}, glm::vec2{0.0f, 0.0f} });
    debugVertices.push_back({ glm::vec2{128.0f, 0.0f}, glm::vec2{1.0f, 0.0f} });
    debugVertices.push_back({ glm::vec2{0.0f, 128.0f}, glm::vec2{0.0f, 1.0f} });
    debugVertices.push_back({ glm::vec2{128.0f, 128.0f}, glm::vec2{1.0f, 1.0f} });

    glCreateBuffers(1, &debugVBO);
    glNamedBufferStorage(debugVBO, sizeof(DebugVertex) * debugVertices.size(), debugVertices.data(), 0x0);

    glCreateVertexArrays(1, &debugVAO);

    glEnableVertexArrayAttrib(debugVAO, 0);
    glVertexArrayAttribFormat(debugVAO, 0, 2, GL_FLOAT, GL_FALSE, offsetof(DebugVertex, pos));
    glVertexArrayAttribBinding(debugVAO, 0, 0);

    glEnableVertexArrayAttrib(debugVAO, 1);
    glVertexArrayAttribFormat(debugVAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(DebugVertex, uv));
    glVertexArrayAttribBinding(debugVAO, 1, 0);

    glVertexArrayVertexBuffer(debugVAO, 0, debugVBO, 0, sizeof(DebugVertex));
}

Shader debugShader{};

void drawDebugQuad(const glm::vec2& offset, float_t scale, uint32_t texture)
{
    glBindVertexArray(debugVAO);
    glBindTextureUnit(0, texture);
    debugShader.setUniform2f("uOffset", offset.x, offset.y);
    debugShader.setUniform1i("uTexture", 0);
    debugShader.setUniform1f("uScale", scale);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

bool polyMode = false;
bool pPressed = false;

void handleInput()
{
    if (window.isKeyPressed(GLFW_KEY_ESCAPE))
    {
        window.close();
    }
    if (window.isKeyPressed(GLFW_KEY_P) && !pPressed)
    {
        pPressed = true;
        polyMode = !polyMode;
        polyMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (!window.isKeyPressed(GLFW_KEY_P))
    {
        pPressed = false;
    }
}

glm::mat4 view{};
Shader oceanShader{};

void updateCameraUniforms()
{
    view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
    oceanShader.setUniformMat4f("uView", view);
}

int32_t main(int argc, char* argv[])
{
    window.preInit(4, 6);
    window.create();
    window.postInit();

    OpenGL::initDebugCallback();
    camera.init(90.0f, {0.0f, 1.0f, 0.0f}, 5.0f, 0.25f);
    createGeometry();
    createGaussianRandomTexture();
    createDebugGeometry();

    oceanShader.create();
    oceanShader.addShader("FFT-Ocean/res/shader/ocean.vert", GL_VERTEX_SHADER);
    oceanShader.addShader("FFT-Ocean/res/shader/ocean.frag", GL_FRAGMENT_SHADER);
    oceanShader.compile();

    glm::mat4 projection = glm::perspective<float_t>(glm::radians(camera.getFov()), static_cast<float_t>(width) / static_cast<float_t>(height), 0.001f, 512.0f);
    oceanShader.setUniformMat4f("uProjection", projection);

    debugShader.create();
    debugShader.addShader("FFT-Ocean/res/shader/debug.vert", GL_VERTEX_SHADER);
    debugShader.addShader("FFT-Ocean/res/shader/debug.frag", GL_FRAGMENT_SHADER);
    debugShader.compile();

    glm::mat4 debugProjection = glm::ortho<float_t>(0.0f, static_cast<float_t>(width), static_cast<float_t>(height), 0.0f, -1.0f, 1.0f);
    debugShader.setUniformMat4f("uProjection", debugProjection);

    float clearColor[]
    {
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float_t depth{ 1.0f };

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    float_t currentTime{};
    float_t lastTime{};
    float_t dt{};
    float_t time{};
    while (!window.isCloseRequested())
    {
        // update dt
        currentTime = (float_t)glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;
        time += dt;

        oceanShader.setUniform1f("uTime", time);

        // clear screen
        glClearNamedFramebufferfv(0, GL_COLOR, 0, clearColor);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
        // update the camera
        camera.update(window, dt);
        updateCameraUniforms();

        oceanShader.bind();
        glBindVertexArray(oceanVAO);
        glDrawElements(GL_TRIANGLES, oceanIndices.size(), GL_UNSIGNED_INT, nullptr);

        debugShader.bind();

        drawDebugQuad({64.0f, 64.0f}, 2.0f, randomTexture);

        window.render();
        handleInput();
    }

    glDeleteTextures(1, &randomTexture);
    glDeleteVertexArrays(1, &oceanVAO);
    glDeleteBuffers(1, &oceanVBO);
    glDeleteBuffers(1, &oceanIBO);

    return 0;
}
