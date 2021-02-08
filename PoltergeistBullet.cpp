#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Camera.h"
#include"Assets.h"
#include"Field.h"

//���S����܂ł̎���
const float Delay_time{3.0f};

PoltergeistBullet::PoltergeistBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, float atk_power) :
	died_timer_{ 0.0f } {
	//���[���h�̐ݒ�
	world_ = world;
	//���O�̐ݒ�
	name_ = "PoltergeistBullet";
	//�^�O���̐ݒ�
	tag_ = "EnemyAttack";
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{0.4f};
	//���W�̏�����
	transform_.position(position);
	//�ړ��ʂ̐ݒ�
	velocity_ = velocity;
	//�U���͂̐ݒ�
	atk_power_ = atk_power;
}

//�X�V
void PoltergeistBullet::update(float delta_time) {
	//3�b�������玀�S
	/*if (died_timer_ / 60 >= Delay_time) {
		die();
		return;
	}
	else died_timer_ += delta_time;*/
	

	//��ʊO�ɏo���玀�S
	if (is_out_camera()) {
		die();
		return;
	}
	Actor* camera = world_->find_actor("Camera");
	if (camera != nullptr)
		camera_pos = camera->transform().position();
	//��]����
	transform_.rotate(0.0f,1.0f,1.0f);
	//�ړ�����
	transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	//�ǂƂ̏Փ˔���
	collide_field();
}

//�`��
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
	GSvector3 light_position{ 100.0f,100.0f,100.0f };//���C�g�̈ʒu
	GScolor light_ambient{ 0.2f,0.2f,0.2f,1.0f };//���C�g�̊���
	GScolor light_diffuse{ 1.0f,1.0f,1.0f,1.0f };//���C�g�̊g�U���ˌ�
	GScolor light_specular{ 1.0f,1.0f,1.0f,1.0f };//���C�g�̋��ʔ��ˌ�

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
	////���C�g�̍��W���V�F�[�_�[�ɓn��
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

	////�x�[�X�J���[�̃e�N�X�`��
	//gsSetShaderParamTexture("u_BaseMap", 0);
	//gsSetShaderParamTexture("u_NormalMap", 1);
	
	gsDrawMeshEx(Mesh_Book);
	gsEndShader();
	glDisable(GL_POLYGON_STIPPLE);


	glPopMatrix();
	//collider().draw();
}

//�Փ˃��A�N�V����
void PoltergeistBullet::react(Actor& other) {
	//�G�l�~�[�ȊO�ɓ��������玀�S
	if (other.tag() != "EnemyTag") {
		DamageProp::do_attack(other, *this, atk_power_);
		die();
	}
}

bool PoltergeistBullet::is_out_camera() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//��ʓ��ɂ�����ړ�����
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//�J�����̑O�x�N�g��
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target)); 
	return (angle >= 45.0f);
}

//�ǂƂ̏Փ˔���
void PoltergeistBullet::collide_field() {
	//�ǂƂ̏Փ˔���(���̂Ƃ̔���)
	GSvector3 center;//�Փˌ�̋��̂̒��S���W
	if (world_->field()->collide(collider(),&center)) {
		die();
	}
}