#ifndef DUSK_TRANSFORM_INC_GLSL
#define DUSK_TRANSFORM_INC_GLSL

layout(binding=1, std140) uniform DuskTransform
{
    // Model Matrix
    mat4 u_Model;
    
    // View Matrix
    mat4 u_View;

    // Projection Matrix
    mat4 u_Projection;

    // Model-View-Projection Matrix
    mat4 u_MVP;
};

#endif // DUSK_TRANSFORM_INC_GLSL