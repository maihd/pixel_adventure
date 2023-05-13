#pragma once

#include <stdint.h>

namespace Game
{
    bool Setup();
    void Shutdown();

    void Load();
    void Unload();

    void Update(float time, float deltaTime);
    void Render();
}
