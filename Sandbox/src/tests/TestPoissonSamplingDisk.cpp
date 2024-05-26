#include "pch.h"
#include "TestPoissonSamplingDisk.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{

    void TestPoissonSamplingDisk::CalculateGrid()
    {
        const int WIDTH = 300;
        const int HEIGHT = 300;
        const float RADIUS = 8;
        const int MAX_SAMPLES = 1000;
        const int K = 30; // Number of attempts

        // Seed generator
        std::random_device rd;
        std::mt19937 engine(rd()); // Mersenne Twister engine seeded with rd()

        // Define distributions
        std::uniform_int_distribution<int> randInt(0, WIDTH);
        std::uniform_real_distribution<float> randFloat(0.0, 1.0);

        float cellSize = RADIUS / glm::sqrt(2);

        int rows = static_cast<int>(HEIGHT / cellSize) + 1;
        int cols = static_cast<int>(WIDTH / cellSize) + 1;

        const glm::vec2 EMPTY_CELL = glm::vec2(-1, -1);

        std::vector<std::vector<glm::vec2>> grid(rows, std::vector<glm::vec2>(cols, EMPTY_CELL));
        std::vector<glm::vec2> pointsQueue, pointList;

        auto inBound = [](glm::vec2 point) -> bool
        {
            return point.x >= 0 && point.x <= WIDTH && point.y >= 0 && point.y <= HEIGHT;
        };

        auto generateRandomPoint = [&](glm::vec2 point) -> glm::vec2
        {
            float random01 = randFloat(engine);
            float random12 = randFloat(engine) + 1.0f;

            float r = RADIUS * random12;
            float angle = glm::two_pi<float>() * random01;
            return point + glm::vec2(r * cos(angle), r * sin(angle));
        };

        auto canPlace = [&](glm::vec2 newPoint) -> bool
        {
            glm::ivec2 cellCoords((int)(newPoint.x / cellSize), (int)(newPoint.y / cellSize));
            for (int i = std::max(0, cellCoords.y - 2); i < std::min(cellCoords.y + 3, rows); ++i)
            {
                for (int j = std::max(0, cellCoords.x - 2); j < std::min(cellCoords.x + 3, cols); ++j)
                {
                    const glm::vec2 &point = grid[i][j];
                    if (point != EMPTY_CELL && glm::distance(point, newPoint) < RADIUS)
                        return false;
                }
            }
            return true;
        };
        glm::ivec2 initial(randInt(engine), randInt(engine));

        pointsQueue.push_back(initial);
        pointList.push_back(initial);

        int gridRow = int(initial.x / cellSize);
        int gridCol = int(initial.y / cellSize);
        grid[gridRow][gridCol] = initial;

        int currentAdded = 1;
        while (!pointsQueue.empty() && currentAdded < MAX_SAMPLES)
        {
            int index = std::uniform_int_distribution<int>(0, pointsQueue.size() - 1)(engine);

            bool found = false;
            for (int k = 0; k < K; ++k)
            {
                glm::vec2 randPoint = generateRandomPoint(pointsQueue[index]);
                if (inBound(randPoint) && canPlace(randPoint))
                {
                    pointsQueue.push_back(randPoint);
                    pointList.push_back(randPoint);

                    int newGridRow = (int)(randPoint.x / cellSize);
                    int newGridCol = (int)(randPoint.y / cellSize);
                    grid[newGridRow][newGridCol] = randPoint;

                    currentAdded++;
                    found = true;
                    break;
                }
            }
            if (!found)
                pointsQueue.erase(pointsQueue.begin() + index);
        }

        // Output points for verification
        for (const auto &point : pointList)
        {
            std::cout << "Point: (" << point.x << ", " << point.y << ")\n";
        }

        float topLeftX = (float)WIDTH;
        float topLeftZ = (float)HEIGHT;
        for (int i = 0; i < pointList.size(); i++)
        {
            glm::vec2 point = pointList[i];
            point.x = point.x / 10.0f;
            point.y = point.y / 10.0f;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(point.x, 0.0f, point.y));
            model = glm::scale(model, glm::vec3(0.05f));
            m_BoxInstanceMatrix[i] = model;
        }
    }

    TestPoissonSamplingDisk::TestPoissonSamplingDisk()
    {
        // Calculate the points using Poisson Disk
        CalculateGrid();

        m_Box = Enxus::CreateRef<Enxus::Model>("Sandbox/res/models/box/box.obj");

        // There is only one mesh -> The BOX

        Enxus::Ref<Enxus::VertexBuffer> instanceBuffer =
            Enxus::CreateRef<Enxus::VertexBuffer>(
                &m_BoxInstanceMatrix[0],
                m_BoxInstanceMatrix.size() * sizeof(glm::mat4));

        Enxus::BufferLayout layout = {
            {Enxus::ShaderDataType::Mat4, "aInstanceMatrix"},
        };
        instanceBuffer->SetLayout(layout);
        m_Box->GetMeshes()[0]->GetVertexArray()->AddVertexBuffer(instanceBuffer);

        m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/advanced-opengl/instancing/instancing-obj.vert", "Sandbox/res/shaders/advanced-opengl/instancing/instancing-obj.frag");
        // m_Shader->Bind();
        // glm::mat4 model = glm::mat4(1.0f);
        // m_Shader->SetMat4("uModel", model);

        //----------------- FACE CULLING -------------------//
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        GLCall(glFrontFace(GL_CW));
    }

    TestPoissonSamplingDisk::~TestPoissonSamplingDisk()
    {
    }

    void TestPoissonSamplingDisk::OnUpdate(Enxus::Camera &camera)
    {

        m_Shader->SetMat4("uView", camera.GetViewMatrix());
        m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());

        for (const auto &mesh : m_Box->GetMeshes())
        {
            // int textureIndex
            // for (const auto textures : mesh->GetTextures())
            // {
            //     m_Shader->SetInt(("material." + name).c_str(), i);
            //     m_Texture->Bind(i); // active the current texture slot
            // }
            mesh->GetTextures()[0]->Bind(0);
            m_Shader->SetInt("material.texture_diffuse1", 0);

            mesh->GetVertexArray()->Bind();
            GLCall(glDrawElementsInstanced(GL_TRIANGLES, mesh->GetVertexArray()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0, 1000));
        }

        // Enxus::Renderer::DrawModel(m_Box, m_Shader);
    };

    void TestPoissonSamplingDisk::OnImGuiRender()
    {
    }

}
