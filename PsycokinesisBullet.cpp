#include "PsycokinesisBullet.h"
#include "Assets.h"
#include"IWorld.h"
#include"Field.h"
#include "Camera.h"

const float MaxAcceleration = 3.0f;
const GSvector3 PlayerOffset{ 0.0f,1.0f,0.0f };

PsycokinesisBullet::PsycokinesisBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, float period, float power)
	:acceleration_{ GSvector3(0.0f, 0.0f, 0.0f) },
	period_{ period }
{
	world_ = world;
	name_ = "PsycokinesisBullet";
	tag_ = "EnemyAttack";	
	velocity_ = velocity;
	collider_ = BoundingSphere{ 0.3f };
	transform_.position(position);
	damage_ = power;
	set_hp(0.0f);
 }

void PsycokinesisBullet::update(float delta_time) {
	Actor* player = world_->find_actor("Player");
	if(player==nullptr)
	{
		player = world_->find_actor("PlayerPaladin");
		if (player == nullptr)return;
	}
	Actor* camera = world_->camera();
	if(camera!=nullptr)camera_pos_ = camera->transform().position();

	acceleration_ = GSvector3::zero();
	GSvector3 diff = player->transform().position() + PlayerOffset - transform_.position();
	acceleration_ += ((diff - velocity_ * period_) * 2.0f / (period_ * period_));
	if (acceleration_.magnitude() >= MaxAcceleration) {
		acceleration_ = acceleration_.normalize() * MaxAcceleration;
	}
	period_ -= (delta_time / 60.0f);
	velocity_ += acceleration_ * (delta_time / 60.0f);
	
	transform_.position(transform_.position() + velocity_ * (delta_time / 60.0f));

	if (world_->field()->collide(collider()))die();
	transform_.rotate(1.0f, 1.0f, 0.0f);
}

void PsycokinesisBullet::draw()const {
	collider().draw();
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	glTranslatef(0.0f, -0.25f, 0.0f);
	
	
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

	gsBeginShader(Shader_Book);
	GSmatrix4 world = transform_.localToWorldMatrix();

	gsSetShaderParamMatrix4("u_WorldViewProjectionMatrix", (GSmatrix4*)&world_view_projection);
	gsSetShaderParamMatrix4("u_WorldMatrix", (GSmatrix4*)&world);
	gsSetShaderParam3f("u_CameraPosition", (GSvector3*)&camera_pos_);
	//ライトの座標をシェーダーに渡す
	gsSetShaderParam3f("u_LightPosition", (GSvector3*)&light_position);
	gsSetShaderParam4f("u_LightAmbient", &light_ambient);
	gsSetShaderParam4f("u_LightDiffuse", &light_diffuse);
	gsSetShaderParam4f("u_LightSpecular", &light_specular);

	//
	gsSetShaderParam4f("u_MaterialAmbient", &material_ambient);
	gsSetShaderParam4f("u_MaterialDiffuse", &material_diffuse);
	gsSetShaderParam4f("u_MaterialSpecular", &material_spacular);
	gsSetShaderParam4f("u_MaterialEmission", &material_emission);
	gsSetShaderParam1f("u_MaterialShininess", material_shininess);

	//ベースカラーのテクスチャ
	gsSetShaderParamTexture("u_BaseMap", 0);
	gsSetShaderParamTexture("u_NormalMap", 1);
	gsDrawMeshEx(Mesh_Book);

	gsEndShader();

	glPopMatrix();
}
void PsycokinesisBullet::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		do_attack(other, *this, damage_);
		die();
	}
}

bool PsycokinesisBullet::on_hit(const Actor& attacker, float atk_power)
{
	return false;
}

