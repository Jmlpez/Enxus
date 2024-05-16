#include "SceneCompositionPanel.h"
#include "imgui/imgui.h"

static SceneCompositionPanelProps s_Props;

void SceneCompositionPanel::Init()
{
    s_Props.IsWireframe = false;
    s_Props.LightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
}

void SceneCompositionPanel::ShutDown()
{
}

const SceneCompositionPanelProps &SceneCompositionPanel::GetPanelProps()
{
    return s_Props;
}

void SceneCompositionPanel::OnImGuiRender()
{
    if (ImGui::BeginTabItem("Scene Composition"))
    {

        ImGui::Checkbox("Wireframe Mode", &s_Props.IsWireframe);
        ImGui::DragFloat3("Light Direction", glm::value_ptr(s_Props.LightDirection), 0.01f);
        ImGui::EndTabItem();
    }
}
