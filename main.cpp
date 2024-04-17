#include "SandboxApp.h"

extern Enxus::Application *Enxus::CreateApplication();

int main(int argc, char *argv[])
{
    auto App = Enxus::CreateApplication();
    App->Run();
    delete App;
}