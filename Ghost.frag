#version 330

//出力カラー
layout(location = 0)out vec4 out_FragColor;


//ワールド座標系の位置
in vec4 v_WorldPosition;
//ワールド座標系の法線ベクトル
in vec3 v_WorldNormal;
//テクスチャ座標
in vec2 v_TexCoord;


//ベースカラーテクスチャ
uniform sampler2D g_BaseMap;


void main(void){

//ベースカラー
vec4 color = texture(g_BaseMap,v_TexCoord);

//最終的なカラーを出力
out_FragColor = color;

}