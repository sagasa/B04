#version 330
// 出力カラー
layout(location = 0) out vec4 out_FragColor;

// ワールド座標系の位置
in vec4 v_WorldPosition;
// ワールド座標系の法線ベクトル
in vec3 v_WorldNormal;
// ワールド座標系の接ベクトル
in vec3 v_WorldTangent;
// ワールド座標系の従法線ベクトル
in vec3 v_WorldBinormal;
// テクスチャ座標
in vec2 v_TexCoord;

// ベースカラーテクスチャ
uniform sampler2D g_BaseMap;
// 法線マップテクスチャ
uniform sampler2D u_NormalMap;

void main(void) {
    // 法線マップカラーの取得
    vec4 normal = texture(u_NormalMap, v_TexCoord);
    // テクスチャカラーの取得
    vec4 baseColor = texture(g_BaseMap, v_TexCoord);

    // 最終的なカラーを出力
    out_FragColor = vec4(baseColor.rgb, 1.0);
}