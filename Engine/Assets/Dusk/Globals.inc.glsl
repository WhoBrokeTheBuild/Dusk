#ifndef DUSK_GLOBALS_INC_GLSL
#define DUSK_GLOBALS_INC_GLSL

layout(binding=0, std140) uniform DuskGlobals
{
    // Viewport resolution (in pixels)
    vec2 u_Resolution;

    // Mouse location (in pixels)
    vec2 u_Mouse;

    // Number of frames that have been rendered
    uint u_FrameCount;

    // Time passed since initialization (in seconds)
    float u_TotalTime;

    // Time passed since the previous frame (in seconds)
    float u_DeltaTime;

    // A ratio of DeltaTime / ExpectedDeltaTime
    float u_FrameSpeedRatio;

    vec3 u_CameraPosition;
    vec3 u_CameraDirection;
    vec3 u_LightPosition;
};

#endif // DUSK_GLOBALS_INC_GLSL