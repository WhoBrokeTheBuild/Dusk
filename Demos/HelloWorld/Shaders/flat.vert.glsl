
layout(location = POSITION_LOCATION)    in vec4 a_Position;
layout(location = NORMAL_LOCATION)      in vec4 a_Normal;
layout(location = COLOR_LOCATION)       in vec4 a_Color;
layout(location = UV_LOCATION)          in vec4 a_UV;
layout(location = TANGENT_LOCATION)     in vec4 a_Tangent;
layout(location = BITANGENT_LOCATION)   in vec4 a_Bitangent;

layout(location = 0) out vec4 v_Position;

void main()
{
    // SetPos(3, 3, 3); LookAt(0, 0, 0);
    mat4 mvp = mat4(
        1.280330, -0.985598, -0.577362, -0.577350, 
        0.000000, 1.971197, -0.577362, -0.577350, 
        -1.280330, -0.985599, -0.577362, -0.577350, 
        -0.000001, -0.000001, 4.996254, 5.196153
    );
    gl_Position = mvp * a_Position;
    v_Position = a_Position;
}