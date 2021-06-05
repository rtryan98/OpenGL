#include <Base.h>

constexpr unsigned int width = 1600;
constexpr unsigned int height = 900;
constexpr char* title = "Hello Model!";
Window window{ width, height, title };

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tc;

    bool operator==(const Vertex& other) const
    {
        return position == other.position
            && normal == other.normal
            && tc == other.tc;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec2>()(vertex.tc) << 1)));
        }
    };
}

std::vector<Vertex> vboData{};
std::vector<uint32_t> iboData{};

void loadModel()
{
    tinyobj::attrib_t attrib{};
    std::vector<tinyobj::shape_t> shapes{};
    std::vector<tinyobj::material_t> materials{};
    std::string warning{}, error{};
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, "../vendor/bunny/bunny.obj"))
    {
        Log::error("Loading Model failed!");
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            vertex.position = {
                attrib.vertices[(uint64_t) 3 * index.vertex_index + 0],
                attrib.vertices[(uint64_t) 3 * index.vertex_index + 1],
                attrib.vertices[(uint64_t) 3 * index.vertex_index + 2]
            };

            vertex.normal = {
                attrib.normals[(uint64_t) 3 * index.normal_index + 0],
                attrib.normals[(uint64_t) 3 * index.normal_index + 1],
                attrib.normals[(uint64_t) 3 * index.normal_index + 2]
            };

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vboData.size());
                vboData.push_back(vertex);
            }

            iboData.push_back(uniqueVertices[vertex]);
        }
    }
}

Shader shader{};
Camera camera{};
glm::mat4 view{};

void updateCameraUniforms()
{
    view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
    shader.setUniformMat4f("uView", view);
}

int main(int argc, char* argv[])
{
    // init window
    window.preInit(4, 6);
    window.create();
    window.postInit();
    // init callback
    OpenGL::initDebugCallback();
    // init shaders
    shader.create();
    shader.addShader("models/res/shader/model.vert", GL_VERTEX_SHADER);
    shader.addShader("models/res/shader/model.frag", GL_FRAGMENT_SHADER);
    shader.compile();
    // init camera
    camera.init(90.0f, {0.0f, 0.0f, 0.0f}, 0.75f, 0.5f);
    // init model
    loadModel();
    // init VBOs
    uint32_t ibo{}, vbo{};
    glCreateBuffers(1, &ibo);
    glNamedBufferStorage(ibo, iboData.size() * sizeof(uint32_t), iboData.data(), 0x0);
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, vboData.size() * sizeof(Vertex), vboData.data(), 0x0);
    // init VAOs
    uint32_t vao{};
    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribBinding(vao, 0, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexArrayAttribBinding(vao, 1, 0);
    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tc));
    glVertexArrayAttribBinding(vao, 2, 0);
    // bind VBO and IBO to VAO
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(vao, ibo);
    // framebuffer clearcolor
    float_t clearColor[]
    {
        0.0f, 0.0f, 0.0f, 1.0f
    };
    float_t depth{ 1.0f };

    // init projection
    glm::mat4 projection = glm::perspective<float>(glm::radians(camera.getFov()), static_cast<float>(width) / static_cast<float>(height), 0.01f, 1000.0f);
    shader.setUniformMat4f(std::string("uProjection"), projection);
    // loop
    float_t currentTime{};
    float_t lastTime{};
    float_t dt{};
    while (!window.isCloseRequested())
    {
        // update dt
        currentTime = (float)glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;
        // clear screen
        glClearNamedFramebufferfv(0, GL_COLOR, 0, clearColor);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
        // update camera
        camera.update(window, dt);
        updateCameraUniforms();
        // draw the model
        shader.bind();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, iboData.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        shader.unbind();
        // swap buffers and check for exit
        window.render();
        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }
    }
    // cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    return 0;
}
