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

// カメラの位置（ワールド座標系）
uniform vec3 u_CameraPosition;

// ライトの位置（ワールド座標系）
uniform vec3 u_LightPosition;
// ライトの環境光パラメータ 
uniform vec4 u_LightAmbient;
// ライトの拡散反射光のパラメータ
uniform vec4 u_LightDiffuse;
// ライトの鏡面反射光のパラメータ
uniform vec4 u_LightSpecular;

// マテリアルの環境光反射率
uniform vec4 u_MaterialAmbient;
// マテリアルの拡散反射光の反射率
uniform vec4 u_MaterialDiffuse;
// マテリアルの鏡面反射光の反射率
uniform vec4 u_MaterialSpecular;
// マテリアルの放射光カラー
uniform vec4 u_MaterialEmission;
// マテリアルの鏡面反射光指数
uniform float u_MaterialShininess;

// ベースカラーマップテクスチャ
uniform sampler2D u_BaseMap;
// 法線マップテクスチャ
uniform sampler2D u_NormalMap; 
// スペキュラマップテクスチャ
uniform sampler2D u_SpecularMap;
// アンビエントオクルージョンマップ
uniform sampler2D u_AmbientOcclusionMap;
// 放射光マップ
uniform sampler2D u_EmissionMap;

void main(void) {
    // 法線マップの法線ベクトルを取得
    vec3 normal = texture(u_NormalMap, v_TexCoord).xyz * 2.0 - 1.0;
    // 法線マップ用の座標変換行列を作成
    mat3 TBN = mat3(normalize(v_WorldTangent), 
                    normalize(v_WorldBinormal),
                    normalize(v_WorldNormal));
    // 法線マップの法線ベクトルをワールド座標系に変換
    vec3 N = normalize(TBN * normal);
    // ライト方向のベクトルを求める
    vec3 L = normalize(u_LightPosition - v_WorldPosition.xyz);
    // 視点方向のベクトルを求める（ワールド座標系)
    vec3 V = normalize(u_CameraPosition - v_WorldPosition.xyz);
    // 2等分ベクトルを求める
    vec3 H = normalize(L + V);

    // ベースカラーマップのカラーを取得
    vec4 baseMapColor = texture(u_BaseMap, v_TexCoord);

    // 最終的な環境光を求める
    vec4 ambientColor = u_LightAmbient * u_MaterialAmbient * baseMapColor;
    
    // 拡散反射光の計算
    float diffuse = max(dot(N, L), 0.0);
    // 最終的な拡散反射光を求める
    vec4 diffuseColor = u_LightDiffuse * u_MaterialDiffuse * baseMapColor * diffuse;
    
    // 鏡面反射光の計算
    float specular = pow(max(dot(N, H), 0.0), u_MaterialShininess);
    // スペキュラマップのカラーを取得
    vec4 specularMapColor = texture(u_SpecularMap, v_TexCoord);
    // 最終的な鏡面反射光を求める
    vec4 specularColor = u_LightSpecular * u_MaterialSpecular * specularMapColor * specular;

    // 放射光マップのカラーを取得
    vec4 emissionMapColor = texture(u_EmissionMap, v_TexCoord);
    // 最終的な放射光を求める
    vec4 emissionColor = u_MaterialEmission * emissionMapColor;

    // 環境光・拡散反射光・鏡面反射光・放射光を合成
    vec4 finalColor = ambientColor + diffuseColor + specularColor + emissionColor;


    // 最終的なカラーを出力
    out_FragColor = vec4(finalColor.rgb, baseMapColor.a);
}