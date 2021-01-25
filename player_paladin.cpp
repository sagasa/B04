#include "player_paladin.h"
#include "IWorld.h"
#include <iostream>


#include "AttackCollider.h"
#include "player_ghost.h"


void player_paladin::draw() const
{
   
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    //gsDrawMesh(Mesh_Player);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    collider_.draw();
    glPopAttrib();

    
	
    // ���[���h�E�r���[�E�v���W�F�N�V�����s��̍쐬
    GSmatrix4 projection_mat, modelview_mat;
    glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projection_mat);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&modelview_mat);
    GSmatrix4 world_view_projection = modelview_mat * projection_mat;

    // ���C�g�̃p�����[�^
    GSvector3 light_position{ 100.0f, 100.0f, 100.0f };    // ���C�g�̈ʒu�i���[���h���W�n�j
    GScolor light_ambient{ 0.2f, 0.2f, 0.2f, 1.0f };     // ���C�g�̊����J���[
    GScolor light_diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };     // ���C�g�̊g�U���ˌ��J���[
    GScolor light_specular{ 1.0f, 1.0f, 1.0f, 1.0f };    // ���C�g�̋��ʔ��ˌ��J���[

    // �}�e���A���̃p�����[�^
    GScolor material_ambient{ 1.0f, 1.0f, 1.0f, 1.0f };  // �����̔��˗�
    GScolor material_diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };  // �g�U���ˌ��̔��˗�
    GScolor material_spacular{ 1.0f, 1.0f, 1.0f, 1.0f }; // ���ʔ��ˌ��̔��˗�
    GScolor material_emission{ 0.0f, 0.0f, 0.0f, 1.0f }; // ���ˌ��J���[�iRGBA)
    float   material_shininess{ 10.0f };                 // ���ʔ��ˎw�� �i�n�C���C�g�j

    // �V�F�[�_�[��L���ɂ���
    gsBeginShader(0);
    // ���[���h�E�r���[�E�v���W�F�N�V�����s����V�F�[�_�[�ɓn��
    gsSetShaderParamMatrix4("u_WorldViewProjectionMatrix",
        (GSmatrix4*)&world_view_projection);
    // ���[���h�ϊ��s����V�F�[�_�[�ɓn��
    gsSetShaderParamMatrix4("u_WorldMatrix", &transform_.localToWorldMatrix());
    // �J�����̍��W���V�F�[�_�[�ɓn��
    gsSetShaderParam3f("u_CameraPosition", (GSvector3*)&camera_pos_);
    // ���C�g�̃p�����[�^���V�F�[�_�[�ɓn��
    gsSetShaderParam3f("u_LightPosition", (GSvector3*)&light_position);
    gsSetShaderParam4f("u_LightAmbient", &light_ambient);
    gsSetShaderParam4f("u_LightDiffuse", &light_diffuse);
    gsSetShaderParam4f("u_LightSpecular", &light_specular);
    // �}�e���A���̃p�����[�^���V�F�[�_�[�ɓn��
    gsSetShaderParam4f("u_MaterialAmbient", &material_ambient);
    gsSetShaderParam4f("u_MaterialDiffuse", &material_diffuse);
    gsSetShaderParam4f("u_MaterialSpecular", &material_spacular);
    gsSetShaderParam4f("u_MaterialEmission", &material_emission);
    gsSetShaderParam1f("u_MaterialShininess", material_shininess);
    // �x�[�X�J���[�̃e�N�X�`��
    gsSetShaderParamTexture("u_BaseMap", 0);
    // �@���}�b�v�̃e�N�X�`��
    gsSetShaderParamTexture("u_NormalMap", 1);
    // ���b�V���̕`��
    mesh_.draw();
    // �V�F�[�_�[�𖳌��ɂ���
    gsEndShader();

    glPopMatrix();
}

void player_paladin::wake_up()
{
    hp_ = 3;
    tag_ = "PlayerTag";
    name_ = "PlayerPaladin";
    change_state(Jump, 1,false);
}

void player_paladin::stop()
{
    tag_ = "PlayerEquip";
	name_ = "Equip";
    change_state(Stop, 0);
}

bool player_paladin::can_interact(const Actor& from)
{
    return true;
}

void player_paladin::change_state(State state, GSuint motion, bool loop)
{
    state_ = state;
    motion_loop_ = loop;
    state_timer_ = 0;
    mesh_.change_motion(motion, loop);
}

void player_paladin::attack()
{
    // �U��������o��������ꏊ�̋���
    const float AttackColliderDistance{ 1.2f };
    // �U������̔��a
    const float AttackColliderRadius{ 1.0f };
    // �U��������o���ꏊ�̍���
    const float AttackColliderHeight{ 1.0f };

    // �U�����肪�L���ɂȂ�܂ł̒x������
    const float AttackCollideDelay{ 15.0f };
    // �U������̎���
    const float AttackCollideLifeSpan{ 15.0f };

    // �Փ˔�����o����������W�����߂�i�O���̈ʒu�j
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // �����̕␳�i��������̍����j
    position.y += AttackColliderHeight;
    // �Փ˔���p�̋����쐬
    BoundingSphere collider{ AttackColliderRadius, position };
    // �Փ˔�����o��������
    world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerAttack", "PlayerAttack", AttackCollideLifeSpan, AttackCollideDelay ,3});

    change_state(Attack, 8, false);
}


player_paladin::player_paladin(IWorld* world, const GSvector3& position) :Player(world, position, AnimatedMesh{ Mesh_Paladin, Skeleton_Paladin, Animation_Paladin })
{
    is_soft_ = false;
    height_ext_ = 0.5;
    collider_ = BoundingSphere{ 0.8f,GSvector3{0.0f,1.4f,0.0f} };
    stop();
}

bool player_paladin::on_hit(const Actor& attacker, float atk_power)
{
    if (state_ == Stop) return false;
    if ((attacker.tag() == "EnemyAttack" || attacker.tag() == "EnemyTag") && state_ != Damage)
    {
        hp_ -= atk_power;
        if (hp_ <= 0)
        {
            stop();
            world_->add_actor(new player_ghost{ world_,transform_.position() });
            std::cout << "Stop " << hp_ << "\n";
        }else
        {
            change_state(Damage, 5, false);
            std::cout << "Hit paladin " << hp_<<" "<<transform_.position().z << "\n";
        }
        return true;
    }
    return false;
}

const float Velocity = 0.07f;
const GSvector3 gravity{ 0.0f, 0.01f, 0.0f };

void player_paladin::update(float delta_time)
{
    Actor* camera = world_->find_actor("Camera");
	if(camera!=nullptr)
		camera_pos_ = camera->transform().position();
	
    //�d��
    velocity_ -= gravity * delta_time;
    //�N��
    if (state_ != Stop) {

        GSvector2 inputVelocity = get_input();
        inputVelocity.y = 0;
    	
        //�U�����͓��͂��΂�
        if (state_ == Attack)
        {
            inputVelocity.x = 0;
        }

        if (inputVelocity.x < 0) {
            transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
        }
        else if (inputVelocity.x > 0) {
            transform_.localRotation(GSquaternion::euler(0.0f, 90.0f, 0.0f));
        }
        else
        {
            velocity_.x *= 0.9f;
        }

        if (state_ != Attack && state_ != Damage)
        {
            //�U���J�n
            if (gsGetKeyState(GKEY_F) || gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_X))
                attack();
        }

        if ((state_ != Attack || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_Y)) && gsGetKeyTrigger(GKEY_E))
        {
            stop();
            world_->add_actor(new player_ghost{ world_,transform_.position() });
        }
    	

        //�W�����v�L�����Z��
        if (state_ == Jump && on_ground_ && jump_count_ <= 0)
            change_state(Idle, 2);
        //�f�t�H�ȊO�Ȃ�
        if (state_ != Idle && state_ != Move)
        {
            state_timer_ += delta_time;
            if (mesh_.motion_end_time() <= state_timer_)
            {
                if (motion_loop_)
                    state_timer_ = 0;
                else
                {
                    change_state(Idle, 2);
                }
            }

        }
        else
        {
            //���x�ŃA�j���[�V�����ύX
            if (inputVelocity.x != 0)
            {
                change_state(Move, 9);
            }
            else
            {
                change_state(Move, 2);
            }
            //�U���J�n
            if (gsGetKeyState(GKEY_F) == GS_TRUE || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X))
                attack();
        }

        //�n��ŉ����͂������Ȃ�
        if (on_ground_)
        {
            if (gsGetKeyState(GKEY_SPACE) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A))
            {
                jump(3, 0.08f);
                change_state(Jump, 6, false);
            }
            inputVelocity.y = MAX(inputVelocity.y, 0);
        }
        // �ړ��ʂ��v�Z
        float speed = 0.04f;    // �ړ��X�s�[�h
        velocity_ += inputVelocity * delta_time * speed;
        velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);
        //velocity_.y = CLAMP(velocity_.y, -Velocity, Velocity);
    	
    }else
    {
        velocity_ *= 0.9;
    }
    update_physics(delta_time);
	
    //���b�V���̍X�V
    mesh_.update(delta_time);
    //�s���ݒ�
    GSmatrix4 dummy;
    dummy.identity();
    mesh_.transform(dummy);
}
