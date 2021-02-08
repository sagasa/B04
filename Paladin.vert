#version 330
// 頂点座標（ローカル座標系）
layout(location = 0) in vec4 in_Position;
// ボーンウェイト
layout(location = 1)  in vec4 in_BoneWeight;
// 法線ベクトル（ローカル座標系）
layout(location = 2) in vec3 in_Normal;
// ボーン番号
layout(location = 7)  in vec4 in_BoneIndices;  
// テクスチャ座標
layout(location = 8) in vec2 in_TexCoord;
// 接ベクトル（ローカル座標系）
layout(location = 14) in vec3 in_Tangent;
// 従法線ベクトル（ローカル座標系）
layout(location = 15) in vec3 in_Binormal;

// ワールド座標系の位置
out vec4 v_WorldPosition;
// ワールド座標系の法線ベクトル
out vec3 v_WorldNormal;
// ワールド座標系の接ベクトル
out vec3 v_WorldTangent;
// ワールド座標系の従法線ベクトル
out vec3 v_WorldBinormal;
// テクスチャ座標
out vec2 v_TexCoord;

// ワールド変換行列
uniform mat4 u_WorldMatrix;
// ワールド・ビュー・プロジェクション変換行列
uniform mat4 u_WorldViewProjectionMatrix;

// ボーンの変換行列
uniform mat4 gs_BoneMatrices[128];              

void main(void) {
    // スキニング用の変換行列を求める
    mat4 matLocal = gs_BoneMatrices[int(in_BoneIndices.x)] * in_BoneWeight.x
                  + gs_BoneMatrices[int(in_BoneIndices.y)] * in_BoneWeight.y
                  + gs_BoneMatrices[int(in_BoneIndices.z)] * in_BoneWeight.z
                  + gs_BoneMatrices[int(in_BoneIndices.w)] * in_BoneWeight.w;
    // スキニング後のローカル座標系に変換
    vec4 localPosition = matLocal * in_Position;
    // 法線ベクトルのスキニング用の座標変換
    vec3 localNormal   = mat3(matLocal) * in_Normal;
    // 接ベクトルのスキニング用の座標変換
    vec3 localTangent  = mat3(matLocal) * in_Tangent;
    // 従法線ベクトルのスキニング用の座標変換
    vec3 localBinormal = mat3(matLocal) * in_Binormal;

    // 頂点座標をワールド座標系に変換
    v_WorldPosition = u_WorldMatrix * localPosition;
    // 法線ベクトルをワールド座標系に変換
    v_WorldNormal = mat3(u_WorldMatrix) * localNormal;
    // 接ベクトルをワールド座標系に変換
    v_WorldTangent = mat3(u_WorldMatrix) * localTangent;
    // 従法線ベクトルをワールド座標系に変換
    v_WorldBinormal = mat3(u_WorldMatrix) * localBinormal;
    // テクスチャ座標の出力
    v_TexCoord = in_TexCoord;
    // ワールド・ビュー・プロジェクション変換
    gl_Position = u_WorldViewProjectionMatrix * in_Position;
}