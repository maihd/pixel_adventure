#pragma once

namespace Timer
{
    void    NewFrame(void);
    void    EndFrame(void);

    float   GetDeltaTime(void);
    float   GetTotalTime(void);

    void    SetFrameRate(float frameRate);
    float   GetFrameRate(void);

    void    SetTimeScale(float timeScale);
    float   GetTimeScale(void);
}
