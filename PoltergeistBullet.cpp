#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Camera.h"
#include"Assets.h"
#include"Field.h"

//死亡するまでの時間
const float Delay_time{3.0f};

PoltergeistBullet::PoltergeistBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, float atk_power) :
	died_timer_{ 0.0f } {
	//ワールドの設定
	world_ = world;
	//名前の設定
	name_ = "PoltergeistBullet";
	//タグ名の設定
	tag_ = "EnemyAttack";
	//衝突判定球の設定
	collider_ = BoundingSphere{0.4f};
	//座標の初期化
	transform_.position(position);
	//移動量の設定
	velocity_ = velocity;
	//攻撃力の設定
	atk_power_ = atk_power;
}

//更新
void PoltergeistBullet::update(float delta_time) {
	//3秒たったら死亡
	/*if (died_timer_ / 60 >= Delay_time) {
		die();
		return;
	}
	else died_timer_ += delta_time;*/
	

	//画面外に出たら死亡
	if (is_out_camera()) {
		die();
		return;
	}
	Actor* camera = world_->find_actor("Camera");
	if (camera != nullptr)
		camera_pos = camera->transform().position();
	//回転する
	transform_.rotate(0.0f,1.0f,1.0f);
	//移動する
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//壁との衝突判定
	collide_field();
}

//描画
void PoltergeistBullet::draw() const {
	

	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	//gsDrawMesh(Mesh_Player);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	collider_.draw();
	glPopAttrib();


	//GSmatrix4 projection = GSmatrix4::perspective(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f);
	
	GSmatrix4 projection_mat, modelview_mat;
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projection_mat);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&modelview_mat);
	GSmatrix4 world_view_projection = modelview_mat * projection_mat;

	//
	GSvector3 light_position{ 100.0f,100.0f,100.0f };//ライトの位置
	GScolor light_ambient{ 0.2f,0.2f,0.2f,1.0f };//ライトの環境光
	GScolor light_diffuse{ 1.0f,1.0f,1.0f,1.0f };//ライトの拡散反射光
	GScolor light_specular{ 1.0f,1.0f,1.0f,1.0f };//ライトの鏡面反射光

	GScolor material_ambient{ 1.0f,1.0f,1.0f,1.0f };
	GScolor material_diffuse{ 1.0f,1.0f,1.0f,1.0f };
	GScolor material_spacular{ 1.0f,1.0f,1.0f,1.0f };
	GScolor material_emission{ 0.0f,0.0f,0.0f,1.0f };
	float material_shininess{ 10.0f };

	//gsBeginShader(0);
	//GSmatrix4 world = transform_.localToWorldMatrix();

	//gsSetShaderParamMatrix4("u_WorldViewProjectionMatrix", (GSmatrix4*)&world_view_projection);
	//gsSetShaderParamMatrix4("u_WorldMatrix", (GSmatrix4*)&world);
	//gsSetShaderParam3f("u_CameraPosition", (GSvector3*)&camera_pos);
	////ライトの座標をシェーダーに渡す
	//gsSetShaderParam3f("u_LightPosition", (GSvector3*)&light_position);
	//gsSetShaderParam4f("u_LightAmbient", &light_ambient);
	//gsSetShaderParam4f("u_LightDiffuse", &light_diffuse);
	//gsSetShaderParam4f("u_LightSpecular", &light_specular);

	////
	//gsSetShaderParam4f("u_MaterialAmbient", &material_ambient);
	//gsSetShaderParam4f("u_MaterialDiffuse", &material_diffuse);
	//gsSetShaderParam4f("u_MaterialSpecular", &material_spacular);
	//gsSetShaderParam4f("u_MaterialEmission", &material_emission);
	//gsSetShaderParam1f("u_MaterialShininess", material_shininess);

	////ベースカラーのテクスチャ
	//gsSetShaderParamTexture("u_BaseMap", 0);
	//gsSetShaderParamTexture("u_NormalMap", 1);
	
	gsDrawMeshEx(Mesh_Book);
	gsEndShader();
	glDisable(GL_POLYGON_STIPPLE);


	glPopMatrix();
	//collider().draw();
}

//衝突リアクション
void PoltergeistBullet::react(Actor& other) {
	//エネミー以外に当たったら死亡
	if (other.tag() != "EnemyTag") {
		DamageProp::do_attack(other, *this, atk_power_);
		die();
	}
}

bool PoltergeistBullet::is_out_camera() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//画面内にいたら移動する
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//カメラの前ベクトル
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target)); 
	return (angle >= 45.0f);
}

//壁との衝突判定
void PoltergeistBullet::collide_field() {
	//壁との衝突判定(球体との判定)
	GSvector3 center;//衝突後の球体の中心座標
	if (world_->field()->collide(collider(),&center)) {
		die();
	}
}