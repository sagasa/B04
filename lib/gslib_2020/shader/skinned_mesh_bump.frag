#version 330
in vec2 v_TexCoord;
in vec3 v_View;
in vec3 v_Light;

layout(location = 0) out vec4 out_FragColor;

uniform vec4 gs_LightAmbient;
uniform vec4 gs_LightDiffuse;
uniform vec4 gs_LightSpecular;

uniform vec4  gs_MaterialAmbient;
uniform vec4  gs_MaterialDiffuse;
uniform vec4  gs_MaterialSpecular;
uniform vec4  gs_MaterialEmission;
uniform float gs_MaterialShininess;

uniform sampler2D gs_DiffseTexture;
uniform sampler2D gs_SpecularTexture;
uniform sampler2D gs_NormalTexture;
uniform sampler2D gs_AmbientOcclusionTexture;
uniform sampler2D gs_EmissiveTexture;

void main(void) {
	vec3 N = normalize(texture(gs_NormalTexture, v_TexCoord).xyz * 2.0 - 1.0);
	vec3 V = normalize(v_View);
	vec3 L = normalize(v_Light);
	vec3 H = normalize(L + V);
	float diffuse  = max(dot(N, L), 0);
	float specular = pow(max(dot(N, H), 0), gs_MaterialShininess);
	vec4 diffseColor = texture(gs_DiffseTexture, v_TexCoord);
	vec4 specularColor = texture(gs_SpecularTexture, v_TexCoord);
	vec4 aoColor = texture(gs_AmbientOcclusionTexture, v_TexCoord);
	vec4 emissiveColor = texture(gs_EmissiveTexture, v_TexCoord);
	vec4 color = gs_MaterialAmbient  * gs_LightAmbient  * diffseColor
			       + gs_MaterialDiffuse  * gs_LightDiffuse  * diffuse * diffseColor
			       + gs_MaterialSpecular * gs_LightSpecular * specular * specularColor;
	color *= aoColor;
	color += gs_MaterialEmission * emissiveColor;
	color.a = diffseColor.a * gs_MaterialDiffuse.a;
	out_FragColor = color;
}
