#version 410 core

out vec4 color;

uniform vec3 part_color;

void main()
{
    vec2 p = gl_PointCoord * 2.0 - 1.0;

    if(dot(p, p) > 1.0)
        discard;

	color = vec4(part_color, 1.0);
    //color = vec4(1.0, 0.5, 0.0, 1.0);
}