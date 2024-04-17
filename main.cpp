
#include "Application.h"

int main()
{
    Enxus::Application *App = new Enxus::Application();

    App->Run();

    delete App;
}