#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void    Timer_NewFrame(void);
void    Timer_EndFrame(void);

float   Timer_GetDeltaTime(void);
float   Timer_GetTotalTime(void);

void    Timer_SetFrameRate(float frameRate);
float   Timer_GetFrameRate(void);

void    Timer_SetTimeScale(float timeScale);
float   Timer_GetTimeScale(void);

#ifdef __cplusplus
}
#endif
