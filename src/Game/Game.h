#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool Game_Setup(void);
void Game_Shutdown(void);

void Game_Load(void);
void Game_Unload(void);

void Game_Update(float time, float deltaTime);
void Game_Render(void);

void Game_RenderDevTools(void);

#ifdef __cplusplus
}
#endif

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
