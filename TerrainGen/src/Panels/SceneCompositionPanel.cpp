#include "SceneCompositionPanel.h"
#include "imgui/imgui.h"

static SceneCompositionPanelProps s_Props;

void SceneCompositionPanel::Init()
{
    s_Props.IsWireframe = false;
    s_Props.UseShadows = true;
    s_Props.LightPosition = glm::vec3(6.0f, 4.0f, 9.0f);

    s_Props.OrthoProj.IsDebugging = false;
    s_Props.OrthoProj.NearPlane = 0.0f;
    s_Props.OrthoProj.FarPlane = 45.0f;
    s_Props.OrthoProj.WidthLimit = 15.0f;
    s_Props.OrthoProj.HeightLimit = 15.0f;
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
        {
            ImGui::SeparatorText("Lighting");
            ImGui::DragFloat3("Light Position", glm::value_ptr(s_Props.LightPosition), 0.01f);
            ImGui::Checkbox("Use Shadows", &s_Props.UseShadows);
            ImGui::Separator();
        }
        ImGui::Checkbox("Debug Ortho Projection", &s_Props.OrthoProj.IsDebugging);
        if (s_Props.OrthoProj.IsDebugging)
        {
            // For debugging purposes
            ImGui::DragFloat("Near Plane", &s_Props.OrthoProj.NearPlane, 0.01f);
            ImGui::DragFloat("Far Plane", &s_Props.OrthoProj.FarPlane, 0.01f);
            ImGui::DragFloat("Width", &s_Props.OrthoProj.WidthLimit, 0.01f);
            ImGui::DragFloat("Height", &s_Props.OrthoProj.HeightLimit, 0.01f);
        }
        ImGui::EndTabItem();
    }
}
