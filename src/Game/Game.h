#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void Game_Setup();
void Game_Shutdown();

void Game_Load();
void Game_Unload();

void Game_Update(float time, float deltaTime);
void Game_Render();

#ifdef __cplusplus
}
#endif
