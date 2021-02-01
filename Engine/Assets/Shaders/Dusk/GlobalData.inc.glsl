#ifndef DUSK_GLOBAL_DATA_INC_GLSL
#define DUSK_GLOBAL_DATA_INC_GLSL

layout(binding = 0, std140) uniform DuskGlobalData
{
    float Time;
    float DeltaTime;
    float FPS;
    int Frame;
    ivec2 Resolution;
    ivec2 Mouse;
    
};

#endif // DUSK_GLOBAL_DATA_INC_GLSL