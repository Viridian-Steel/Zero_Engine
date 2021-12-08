#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;


layout(push_constant) uniform Push {
	mat2 transform;
	vec2 offset; 
	vec3 color;
	} push;

void main(){
	gl_Position = vec4(push.transform * position + push.offset, 0., 1.0);
}
//#version 450

//layout (location = 0) in vec2 in_position;
//layout (location = 1) in vec2 in_uv;

//layout (location = 0) out vec2 out_uv;

//void main() {
//    gl_Position = vec4(in_position, 0.0, 1.0);
//    out_uv = in_uv;
//}