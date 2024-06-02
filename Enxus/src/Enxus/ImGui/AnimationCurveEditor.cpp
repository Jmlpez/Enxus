#include "pch.h"
#include "AnimationCurveEditor.h"
#include "ImGuiBezierEdit.h"

namespace Enxus
{
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
    }
    AnimationCurveEditor::~AnimationCurveEditor()
    {
    }

    float AnimationCurveEditor::Evaluate(float point)
    {
        return ImGui::BezierValue(point, m_BezierPoints.data());
    }
    void AnimationCurveEditor::OnImGuiRender()
    {
        ImGui::Bezier("Bezier Values", m_BezierPoints.data());
    }
}
