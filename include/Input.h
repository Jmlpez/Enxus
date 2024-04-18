#ifndef INPUT_H
#define INPUT_H

// #include

namespace Enxus
{
    // Singleton
    class Input
    {
    public:
        ~Input() { delete s_Instance; }
        inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
        inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

    private:
        bool IsKeyPressedImpl(int keycode);
        bool IsMouseButtonPressedImpl(int button);
        std::pair<float, float> GetMousePositionImpl();
        float GetMouseXImpl();
        float GetMouseYImpl();

    private:
        static Input *s_Instance;
    };

}
#endif
