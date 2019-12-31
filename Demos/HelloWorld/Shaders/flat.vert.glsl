#version 330 core

layout(location = 0) in vec4 a_Position;

out vec4 v_Position;

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