#include shaders/data/attributes.inc.glsl
#include shaders/data/transform.inc.glsl

out vec4 v_Color;

void main() 
{
    v_Color = a_Color;
    gl_Position = u_MVPMatrix * a_Position;
}