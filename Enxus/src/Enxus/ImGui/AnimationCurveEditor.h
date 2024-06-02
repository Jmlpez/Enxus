#ifndef ANIMATION_CURVE_EDITOR_H
#define ANIMATION_CURVE_EDITOR_H

#include "Base.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"

namespace Enxus
{
    enum class AnimationCurveType
    {
        None = 0,
        Linear,
        InSine,
        InQuad,
        InCubic,
        InQuart,
        InQuint,
        InExpo,
        InCirc,
        InBack,
        OutSine,
        OutQuad,
        OutCubic,
        OutQuart,
        OutQuint,
        OutExpo,
        OutCirc,
        OutBack,
        InOutSine,
        InOutQuad,
        InOutCubic,
        InOutQuart,
        InOutQuint,
        InOutExpo,
        InOutCirc,
        InOutBack,
    };

    class AnimationCurveEditor
    {
    public:
        AnimationCurveEditor(AnimationCurveType initialType = AnimationCurveType::None);
        ~AnimationCurveEditor();

        float Evaluate(float point);
        void OnImGuiRender();

        bool operator==(const AnimationCurveEditor &curve) { return m_BezierPoints == curve.m_BezierPoints; }
        bool operator!=(const AnimationCurveEditor &curve) { return m_BezierPoints != curve.m_BezierPoints; }

        bool operator==(const AnimationCurveEditor &curve) const { return m_BezierPoints == curve.m_BezierPoints; }
        bool operator!=(const AnimationCurveEditor &curve) const { return m_BezierPoints != curve.m_BezierPoints; }

    private:
        void RecalculateTable();
        void BuildHelperTable();

    private:
        // initial values --> is in group of two ([0], [1]), ([2],[3])
        std::array<float, 4> m_BezierPoints;

        // The arrays size must be 1 + s_Steps.
        static ImVec2 s_ResultsTable[1025];
        static float s_HelperTable[1025 * 4];

        static const uint32_t s_Steps;
        AnimationCurveType m_CurveType;
    };
}

#endif