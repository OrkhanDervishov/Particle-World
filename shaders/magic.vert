#version 410 core
in vec3 pos;

uniform mat4 view;
uniform mat4 perspective;

uniform vec3 camera_pos;
uniform float point_size;


void main(){
	gl_Position = perspective * view * vec4(pos.x, pos.z, pos.y, 1.0f);
	float shrink_scale = 1.0f/distance(pos, camera_pos);
	gl_PointSize = point_size * shrink_scale;
}