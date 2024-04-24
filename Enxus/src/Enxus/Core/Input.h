#ifndef INPUT_H
#define INPUT_H

// #include

namespace Enxus
{
    // Singleton
    class Input
    {
    public:
        ~Input() {}
        static bool IsKeyPressed(int keycode);
        static bool IsMouseButtonPressed(int button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };

}
#endif
