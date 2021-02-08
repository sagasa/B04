#include "player_paladin.h"
#include "IWorld.h"
#include <iostream>

#include "ParticleManager.h"
#include "AttackCollider.h"
#include "HPBar.h"
#include "player_ghost.h"

GLubyte mask[128];

enum {
    MotionSleep = 0,	// 歩き
    MotionIdle = 2,	// アイドル
    MotionWalk = 10,	// 歩き
    MotionAttack = 9,	// ガード
    MotionDamage = 5,	// ダウン
    MotionWakeUp = 1,	// 起動
    MotionExit = 3,	// 消滅

};
std::ostream& operator<<(std::ostream& os, const GSvector3& dt)
{
    os << dt.x << ',' << dt.y << ',' << dt.z;
    return os;
}

const GSvector2 baseHpOffset{ 5,5 };
const GSvector2 hpOffset{ 5,35 };

void player_paladin::draw() const
{
   
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    //沈む
    if (state_ == Dead && mesh_.motion_end_time() <= state_timer_)
    {
        float down = state_timer_ - mesh_.motion_end_time();
        down /= -200;
        std::cout << down << "\n";
        glTranslatef(0, down, 0);
    }
    //gsDrawMesh(Mesh_Player);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    collider_.draw();
    glPopAttrib();

	
	
    glPolygonStipple(mask);
    //glEnable(GL_POLYGON_STIPPLE);
	
    // ワールド・ビュー・プロジェクション行列の作成
    GSmatrix4 projection_mat, modelview_mat;
    glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&projection_mat);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&modelview_mat);
    GSmatrix4 world_view_projection = modelview_mat * projection_mat;

    // ライトのパラメータ
    GSvector3 light_position{ 100.0f, 100.0f, 100.0f };    // ライトの位置（ワールド座標系）
    GScolor light_ambient{ 2.0f, 2.0f, 2.0f, 1.0f };     // ライトの環境光カラー
    GScolor light_diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };     // ライトの拡散反射光カラー
    GScolor light_specular{ 1.0f, 1.0f, 1.0f, 1.0f };    // ライトの鏡面反射光カラー

    // マテリアルのパラメータ
    GScolor material_ambient{ 1.0f, 1.0f, 1.0f, 1.0f };  // 環境光の反射率
    GScolor material_diffuse{ 1.0f, 1.0f, 1.0f, 1.0f };  // 拡散反射光の反射率
    GScolor material_spacular{ 1.0f, 1.0f, 1.0f, 1.0f }; // 鏡面反射光の反射率
    GScolor material_emission{ 0.0f, 0.0f, 0.0f, 1.0f }; // 放射光カラー（RGBA)
    float   material_shininess{ 10.0f };                 // 鏡面反射指数 （ハイライト）

    // シェーダーを有効にする
    gsBeginShader(Shader_Paladin);
    // ワールド・ビュー・プロジェクション行列をシェーダーに渡す
    gsSetShaderParamMatrix4("u_WorldViewProjectionMatrix",
        (GSmatrix4*)&world_view_projection);
    // ワールド変換行列をシェーダーに渡す
    GSmatrix4 world = transform_.localToWorldMatrix();
    gsSetShaderParamMatrix4("u_WorldMatrix", &world);
    // カメラの座標をシェーダーに渡す
    gsSetShaderParam3f("u_CameraPosition", (GSvector3*)&camera_pos_);
    // ライトのパラメータをシェーダーに渡す
    gsSetShaderParam3f("u_LightPosition", (GSvector3*)&light_position);
    gsSetShaderParam4f("u_LightAmbient", &light_ambient);
    gsSetShaderParam4f("u_LightDiffuse", &light_diffuse);
    gsSetShaderParam4f("u_LightSpecular", &light_specular);
    // マテリアルのパラメータをシェーダーに渡す
    gsSetShaderParam4f("u_MaterialAmbient", &material_ambient);
    gsSetShaderParam4f("u_MaterialDiffuse", &material_diffuse);
    gsSetShaderParam4f("u_MaterialSpecular", &material_spacular);
    gsSetShaderParam4f("u_MaterialEmission", &material_emission);
    gsSetShaderParam1f("u_MaterialShininess", material_shininess);
    gsSetShaderParamArrayMatrix4("gs_BoneMatrices",128, &mesh_.bone_matrices(0));
    // ベースカラーのテクスチャ
    gsSetShaderParamTexture("u_BaseMap", 0);
    // 法線マップのテクスチャ
    gsSetShaderParamTexture("u_NormalMap", 1);
    // メッシュの描画
    //mesh_.draw();

    gsDisable(GS_CALC_SKELETON);
    gsBindSkeleton(Skeleton_Paladin);
    gsSetMatrixSkeleton(&mesh_.bone_matrices(0));
	gsDrawMesh(Mesh_Paladin);
    gsEnable(GS_CALC_SKELETON);
	
   
    // シェーダーを無効にする
    gsEndShader();

    glDisable(GL_POLYGON_STIPPLE);
	
    glPopMatrix();

	if(is_active())
	{
     
        glPushMatrix();
        GSvector2 pos{ 20,20};
		
		base_bar_.draw(baseHpOffset);
        bar_.draw(hpOffset);
        
        /*
		for (int i = 0; i < hp_; ++i)
        {
            

        	
            GSvector2 scale{ 0.1f, 0.1f };
            gsDrawSprite2D(Texture_Shield, &pos, nullptr, NULL, NULL, &scale, NULL);
            pos.x += 40;
        }*/
        glPopMatrix();
	}
}

void player_paladin::wake_up(const int base_hp)
{
    base_hp_ = base_hp;
    base_bar_.set_HP_NoAnim(base_hp);
    hp_ = 3;
    bar_.set_HP(3);
    tag_ = "PlayerTag";
    name_ = "PlayerPaladin";
    change_state(Wake, MotionWakeUp,false);
}

void player_paladin::stop()
{
    tag_ = "PlayerEquip";
	name_ = "Equip";
    change_state(Stop, MotionSleep);
}

bool player_paladin::can_interact(const Actor& from)
{
    return state_!=Dead;
}

void player_paladin::change_state(State state, GSuint motion, bool loop)
{
    state_ = state;
    if (!mesh_.change_motion(motion, loop))return;
    motion_loop_ = loop;
    state_timer_ = 0;
}

void player_paladin::attack()
{
    // 攻撃判定を出現させる場所の距離
    const float AttackColliderDistance{ 1.2f };
    // 攻撃判定の半径
    const float AttackColliderRadius{ 1.0f };
    // 攻撃判定を出す場所の高さ
    const float AttackColliderHeight{ 1.0f };

    // 攻撃判定が有効になるまでの遅延時間
    const float AttackCollideDelay{ 15.0f };
    // 攻撃判定の寿命
    const float AttackCollideLifeSpan{ 15.0f };

    // 衝突判定を出現させる座標を求める（前方の位置）
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // 高さの補正（足元からの高さ）
    position.y += AttackColliderHeight;
    // 衝突判定用の球を作成
    BoundingSphere collider{ AttackColliderRadius, position };
    // 衝突判定を出現させる
    world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerAttack", "PlayerAttack", AttackCollideLifeSpan, AttackCollideDelay ,3});
	//SE
    attack_se_ = true;
    change_state(Attack, MotionAttack, false);
}

bool player_paladin::is_active()const
{
    return state_ != Stop && state_ != Dead;
}


player_paladin::player_paladin(IWorld* world, const GSvector3& position) :Player(world, position, AnimatedMesh{ Mesh_Paladin, Skeleton_Paladin, Animation_Paladin })
{
    std::cout << " init " << position << "\n";
    is_soft_ = false;
    height_ext_ = 0.5;
    collider_ = BoundingSphere{ 0.8f,GSvector3{0.0f,1.2f,0.0f} };
    stop();

    for (int i = 0; i < 128; i+=2)
    {
        mask[i] = 0xFF;
    }
}

bool player_paladin::on_hit(const Actor& attacker, float atk_power)
{
    if (state_ == Stop) return false;
	//地形ダメージ
    if (attacker.tag() == "FieldDamage")
    {
        hp_ -= atk_power;
        bar_.set_HP(hp_);
        if (hp_ <= 0)
        {
            stop();
            change_state(Dead, MotionExit, false);
            world_->game_over();
        }
        gsPlaySE(SE_ParadinDamage);
        return true;
    }
    if ((attacker.tag() == "EnemyAttack" || attacker.tag() == "EnemyTag") && state_ != Damage)
    {
        hp_ -= atk_power;
        bar_.set_HP(hp_);
        if (hp_ <= 0)
        {
            stop();
            change_state(Dead, MotionExit, false);
            world_->add_actor(new player_ghost{ world_,transform_.position() });
            std::cout << "Stop " << hp_ << "\n";
        }else
        {
            change_state(Damage, MotionDamage, false);
            std::cout << "Hit paladin " << hp_<<" "<<transform_.position().z << "\n";
        }
        gsPlaySE(SE_ParadinDamage);
        return true;
    }
    return false;
}

const float Velocity = 0.07f;
const GSvector3 gravity{ 0.0f, 0.01f, 0.0f };


void player_paladin::update(float delta_time)
{
    bar_.update(delta_time);
    base_bar_.update(delta_time);
    Actor* camera = world_->find_actor("Camera");
	if(camera!=nullptr)
		camera_pos_ = camera->transform().position();

    //重力
    velocity_ -= gravity * delta_time;
	//死亡後の消滅処理
    if (state_ == Dead && mesh_.motion_end_time() + 180 <= state_timer_)
        dead_=true;
    //起動
    if (is_active()) {

        GSvector2 inputVelocity = get_input();
        inputVelocity.y = 0;

    	//SE

        if(state_ == Attack)
        {
            std::cout << "Attack Progress " <<state_timer_<<"\n";
            if (30 <= state_timer_&&attack_se_)
            {
                //SE
                gsPlaySE(SE_Attack);
                attack_se_ = false; //今のPaladinモデルのVertexFormatの左から3桁目の数字を3にするとSkiningMeshのシェーダーできれいに映ります。

            }
        }
    	
    	//エフェクト
    	if(state_ == Wake)
    	{
            std::cout << "pos " << mesh_.bone_matrices(1).position()<<"\n";
            world_->particle_manager()->possession_light(mesh_.bone_matrices(0).position()+transform_.position());
    	}
    	
        //起動時 攻撃中は入力を飛ばす
        if (state_ == Attack|| state_ == Wake)
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
            //攻撃開始
            if (gsGetKeyState(GKEY_F) || gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_X))
                attack();
        }

        if ((gsGetKeyTrigger(GKEY_E) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_Y)) && state_ != Attack)
        {
            stop();
            change_state(Dead, MotionExit, false);
            world_->particle_manager()->possession_release_light(collider().center);
            world_->add_actor(new player_ghost{ world_,transform_.position() ,base_hp_});
        }
    	

        //ジャンプキャンセル
        if (state_ == Jump && on_ground_ && jump_count_ <= 0)
            change_state(Idle, MotionIdle);
        //デフォ以外なら
        if (state_ != Idle && state_ != Move)
        {
            if (mesh_.motion_end_time() <= state_timer_)
            {
                if (motion_loop_)
                    state_timer_ = 0;
                else
                {
                    change_state(Idle, MotionIdle);
                }
            }
            sound_timer_ = -1;
        }
        else
        {
            //速度でアニメーション変更
            if (inputVelocity.x != 0)
            {
                change_state(Move, MotionWalk);
                const int soundtime = (int)state_timer_ / ((int)mesh_.motion_end_time() / 2);
            	//アニメーションで２回鳴る
                if ((int)state_timer_ % ((int)mesh_.motion_end_time() / 2) > mesh_.motion_end_time() / 4&&sound_timer_ < soundtime)
                {
                    gsPlaySE(SE_ParadinMove);
                    sound_timer_ = soundtime;
                }
                std::cout << "timer "<< (int)state_timer_ % ((int)mesh_.motion_end_time() / 2) <<" " <<"\n";               
            }
            else
            {
                change_state(Move, MotionIdle);
                sound_timer_ = -1;
            }
            //攻撃開始
            if (gsGetKeyState(GKEY_F) == GS_TRUE || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X))
                attack();
        }

        //地上で下入力を許可しない
        if (on_ground_)
        {
            if (gsGetKeyState(GKEY_SPACE) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_A))
            {
                if (jump_count_ <= 0)
                {
                    gsPlaySE(SE_Jump);
                    jump(3, 0.08f);
                    change_state(Jump, 6, false);
                }
            }
            inputVelocity.y = MAX(inputVelocity.y, 0);
        }
        // 移動量を計算
        float speed = 0.04f;    // 移動スピード
        velocity_ += inputVelocity * delta_time * speed;
        velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);
        //velocity_.y = CLAMP(velocity_.y, -Velocity, Velocity);
    	
    }else
    {
        velocity_ *= 0.9;
    }
    update_physics(delta_time);
    //タイマー加算
    state_timer_ += delta_time;
    //メッシュの更新
    mesh_.update(delta_time);
    //行列を設定
    GSmatrix4 dummy;
    dummy.identity();
    mesh_.transform(dummy);
}
