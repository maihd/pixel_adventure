#pragma once

#include <stdint.h>

// @todo: convert to C ABI
namespace Game
{
    bool Setup();
    void Shutdown();

    void Load();
    void Unload();

    void Update(float time, float deltaTime);
    void Render();

    void RenderDevTools();
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
