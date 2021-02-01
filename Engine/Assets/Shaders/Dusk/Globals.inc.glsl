#ifndef DUSK_GLOBALS_INC_GLSL
#define DUSK_GLOBALS_INC_GLSL

layout(binding = 0, std140) uniform DuskGlobals
{
    vec2 Resolution;
    vec2 Mouse;
    int FrameCount;
    float TotalTime;
    float FrameSpeedRatio;
    
};

#endif // DUSK_GLOBALS_INC_GLSL