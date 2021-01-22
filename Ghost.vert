#version 330

//頂点座標
layout(location = 0)in vec4 in_Position;
//法線ベクトル
layout(location = 2)in vec3 in_Normal;
//テクスチャ座標
layout(location = 8)in vec2 in_TexCoord;


//ワールド座標系の位置
out vec4 v_WorldPosition;
//ワールド座標系の法線ベクトル
out vec3 v_WorldNormal;
//ワールド座標系のテクスチャ座標
out vec2 v_TexCoord;

//ワールド変換行列
uniform mat4 u_WorldMatrix;
//ワールド・ビュー・プロジェクション変換行列
uniform mat4 u_WorldViewProjectionMatrix;

void main(void){
//頂点座標をワールド座標に変換
v_WorldPosition = u_WorldMatrix * in_Position;
//法線ベクトルをワールド座標系に変換
v_WorldNormal = mat3(u_WorldMatrix) * in_Normal;
//テクスチャ座標をワールド座標系に変換
v_TexCoord = in_TexCoord;

//ワールド・ビュー・プロジェクション変換
gl_Position = u_WorldViewProjectionMatrix * in_Position;

}