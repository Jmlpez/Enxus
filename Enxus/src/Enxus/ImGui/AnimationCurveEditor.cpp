#include "pch.h"
#include "AnimationCurveEditor.h"
#include "ImGuiBezierEdit.h"

namespace Enxus
{
    glm::vec2 AnimationCurveEditor::s_ResultsTable[AnimationCurveEditor::s_Steps + 1]{glm::vec2(0, 0)};
    float AnimationCurveEditor::s_HelperTable[(AnimationCurveEditor::s_Steps + 1) * 4];

    AnimationCurveEditor::AnimationCurveEditor(AnimationCurveType initialType)
        : m_CurveType(initialType)
    {
        static std::vector<std::array<float, 4>> s_BezierPointsFromCurveType{
            /*None*/ {0.000f, 0.000f, 1.000f, 1.000f},
            /*Linear*/ {0.000f, 0.000f, 1.000f, 1.000f},
            /*InSine*/ {0.470f, 0.000f, 0.745f, 0.715f},
            /*InQuad*/ {0.550f, 0.085f, 0.680f, 0.530f},
            /*InCubic*/ {0.550f, 0.055f, 0.675f, 0.190f},
            /*InQuart*/ {0.895f, 0.030f, 0.685f, 0.220f},
            /*InQuint*/ {0.755f, 0.050f, 0.855f, 0.060f},
            /*InExpo*/ {0.950f, 0.050f, 0.795f, 0.035f},
            /*InCirc*/ {0.600f, 0.040f, 0.980f, 0.335f},
            /*InBack*/ {0.600f, -0.28f, 0.735f, 0.045f},
            /*OutSine*/ {0.390f, 0.575f, 0.565f, 1.000f},
            /*OutQuad*/ {0.250f, 0.460f, 0.450f, 0.940f},
            /*OutCubic*/ {0.215f, 0.610f, 0.355f, 1.000f},
            /*OutQuart*/ {0.165f, 0.840f, 0.440f, 1.000f},
            /*OutQuint*/ {0.230f, 1.000f, 0.320f, 1.000f},
            /*OutExpo*/ {0.190f, 1.000f, 0.220f, 1.000f},
            /*OutCirc*/ {0.075f, 0.820f, 0.165f, 1.000f},
            /*OutBack*/ {0.175f, 0.885f, 0.320f, 1.275f},
            /*InOutSine*/ {0.445f, 0.050f, 0.550f, 0.950f},
            /*InOutQuad*/ {0.455f, 0.030f, 0.515f, 0.955f},
            /*InOutCubic*/ {0.645f, 0.045f, 0.355f, 1.000f},
            /*InOutQuart*/ {0.770f, 0.000f, 0.175f, 1.000f},
            /*InOutQuint*/ {0.860f, 0.000f, 0.070f, 1.000f},
            /*InOutExpo*/ {1.000f, 0.000f, 0.000f, 1.000f},
            /*InOutCirc*/ {0.785f, 0.135f, 0.150f, 0.860f},
            /*InOutBack*/ {0.680f, -0.55f, 0.265f, 1.550f},
        };
        m_BezierPoints = s_BezierPointsFromCurveType[(int)m_CurveType];

        BuildHelperTable();
        RecalculateTable();
    }
    AnimationCurveEditor::~AnimationCurveEditor()
    {
    }

    void AnimationCurveEditor::BuildHelperTable()
    {
        for (unsigned step = 0; step <= s_Steps; ++step)
        {
            float t = (float)step / (float)s_Steps;
            const uint32_t stepIndex = step << 2;                       // step *4
            s_HelperTable[stepIndex + 0] = (1 - t) * (1 - t) * (1 - t); // * P0
            s_HelperTable[stepIndex + 1] = 3 * (1 - t) * (1 - t) * t;   // * P1
            s_HelperTable[stepIndex + 2] = 3 * (1 - t) * t * t;         // * P2
            s_HelperTable[stepIndex + 3] = t * t * t;                   // * P3
        }
    }

    void AnimationCurveEditor::RecalculateTable()
    {
        glm::vec2 Points[4] = {{0, 0}, {m_BezierPoints[0], m_BezierPoints[1]}, {m_BezierPoints[2], m_BezierPoints[3]}, {1, 1}};
        // ImGui::bezier_table<1024>(Points, s_ResultsTable);
        for (unsigned step = 0; step <= s_Steps; ++step)
        {
            // step * 4
            const int stepIndex = step << 2;
            glm::vec2 point = {
                s_HelperTable[stepIndex] * Points[0].x +
                    s_HelperTable[stepIndex + 1] * Points[1].x +
                    s_HelperTable[stepIndex + 2] * Points[2].x +
                    s_HelperTable[stepIndex + 3] * Points[3].x,
                s_HelperTable[stepIndex] * Points[0].y +
                    s_HelperTable[stepIndex + 1] * Points[1].y +
                    s_HelperTable[stepIndex + 2] * Points[2].y +
                    s_HelperTable[stepIndex + 3] * Points[3].y,
            };
            s_ResultsTable[step] = point;
        }
    }

    float AnimationCurveEditor::Evaluate(float point)
    {
        if (
            m_BezierPoints[0] == m_BezierPoints[1] &&
            m_BezierPoints[1] == m_BezierPoints[2] &&
            m_BezierPoints[2] == m_BezierPoints[3] &&
            m_BezierPoints[3] == 0.0f)
            return point;

        if (point < 0.0f)
            point = 0.0f;
        if (point > 1.0f)
            point = 1.0f;

        return s_ResultsTable[(int)(point * s_Steps)].y;
    }
    void AnimationCurveEditor::OnImGuiRender()
    {
        if (ImGui::Bezier("Bezier Values", m_BezierPoints.data()))
        {
            RecalculateTable();
        }
    }
}
