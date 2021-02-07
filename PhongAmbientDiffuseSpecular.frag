#version 330

layout(location = 0)out vec4 out_FragColor;

in vec4 v_WorldPosition;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;

uniform vec3 u_CameraPosition;
uniform vec3 u_LightPosition;

uniform vec4 u_LightAmbient;
uniform vec4 u_LightDiffuse;
uniform vec4 u_LightSpecular;

uniform vec4 u_MaterialAmbient;
uniform vec4 u_MaterialDiffuse;
uniform vec4 u_MaterialSpecular;
uniform vec4 u_MaterialEmission;
uniform float u_MaterialShininess;

uniform sampler2D g_BaseMap;

void main(void){
	vec3 N=normalize(v_WorldNormal);
	vec3 L=normalize(u_LightPosition-v_WorldPosition.xyz);
	vec3 V=normalize(u_CameraPosition-v_WorldPosition.xyz);
	vec3 H=normalize(L+V);

	vec4 ambientColor=u_LightAmbient*u_MaterialAmbient;

	float diffuse=max(dot(N,L),0.0f);
	vec4 diffuseColor=u_LightDiffuse*u_MaterialDiffuse*diffuse;

	float specular=pow(max(dot(N,H),0.0f),u_MaterialShininess);
	vec4 specularColor=u_LightSpecular*u_MaterialSpecular*specular;

	vec4 baseColor=texture(g_BaseMap,v_TexCoord);

	vec4 color=(ambientColor+diffuseColor+u_MaterialEmission)*baseColor+specularColor;

	out_FragColor=vec4(color.rgb,baseColor.a);
}