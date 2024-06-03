#ifndef ANIMATION_CURVE_EDITOR_H
#define ANIMATION_CURVE_EDITOR_H

#include "Base.h"
#include "glm/glm.hpp"

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
        static void BuildHelperTable();

    private:
        // initial values --> is in group of two ([0], [1]), ([2],[3])
        std::array<float, 4> m_BezierPoints;

        static const uint32_t s_Steps = 4096;

        static glm::vec2 s_ResultsTable[s_Steps + 1];
        static float s_HelperTable[(s_Steps + 1) * 4];

        AnimationCurveType m_CurveType;
    };
}

#endif