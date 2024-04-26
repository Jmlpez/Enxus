#include "SandboxApp.h"
#include "TerrainGenApp.h"

extern Enxus::Application *Enxus::CreateApplication();

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    auto App = Enxus::CreateApplication();
    App->Run();
    delete App;
}
