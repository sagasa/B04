#version 330

layout(location = 0)in vec4 in_Position;
layout(location = 2)in vec3 in_Normal;
layout(location = 8)in vec2 in_TexCoord;

out vec4 v_WorldPosition;
out vec3 v_WorldNormal;
out vec2 v_TexCoord;

uniform mat4 u_WorldMatrix;
uniform mat4 u_WorldViewProjectionMatrix;

void main(void){
	v_WorldPosition=u_WorldMatrix*in_Position;
	v_WorldNormal=mat3(u_WorldMatrix)*in_Normal;
	v_TexCoord=in_TexCoord;
	gl_Position=u_WorldViewProjectionMatrix*in_Position;
}