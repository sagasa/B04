#include"TitleScene.h"
#include"Assets.h"
#include<iostream>
#include<GSinput.h>

//α値の代入値
const float Alpha_Value{ 0.008f };
//ボタンが出るまでの時間
const float Time{ 60.0f };
//配列の大きさ
const int Alphas_Size{ 3 };

const int Pos_Size{ 3 };

const GSvector2 pos_x_in{ 0.0f,0.0f };
const GSvector2 pos_x_out{ -1280.0f,0.0f };
const GSvector2 pos_x_in2{ 0.0f,0.0f };
const GSvector2 pos_x_out2{ 1280.0f,0.0f };
const float MovingTime{ 60.0f };
//開始
void TitleScene::start(int number) {
	fade_.start(true,3);
	//終了フラグの初期化
	is_end_ = false;
	nextScene_ = "";
	button_timer_ = 0;
	stage_number_ = number;
	state_ = State::Push_Button;
	alphas_.resize(Alphas_Size, 1.0f);
	pos_x_.resize(Pos_Size, 0.0f);
	pos_x_[1] = 2000.0f;
	select_ = 1;
	num_ = 0;
	is_move_ = false;
	move_timer_ = 0;
	//テクスチャの読み込み
	gsLoadTexture(Texture_TitleLogo, "Assets/Image/title_logo.dds");
	gsLoadTexture(Texture_PushButton, "Assets/Image/push_button.dds");
	gsLoadTexture(Texture_Game_Start, "Assets/Image/game_start.dds");
	gsLoadTexture(Texture_tutorial, "Assets/Image/how_to_play.dds");
	gsLoadTexture(Texture_How_To_Play1, "Assets/Image/tutorial_ghost.dds");
	gsLoadTexture(Texture_How_To_Play2, "Assets/Image/tutorial_paladin.dds");
	gsLoadTexture(Texture_Arrow, "Assets/Image/arrow.dds");
	gsLoadTexture(Texture_Back, "Assets/Image/back.dds");
	gsLoadTexture(Texture_Fade, "Assets/Image/black.dds");
	//BGMの読み込み
	gsLoadMusic(Music_Title, "Assets/BGM/title.wav", GS_TRUE);
	//SEの読み込み
	gsLoadSE(SE_Push, "Assets/SE/push.wav", 1, GWAVE_DEFAULT);
	gsLoadSE(SE_Select, "Assets/SE/select.wav", 1, GWAVE_DEFAULT);

	gsBindMusic(Music_Title);
	gsPlayMusic();
}

void TitleScene::update(float delta_time) {
	gsSetMusicVolume(0.8f);
	fade_.update(delta_time);
	switch (state_) {
	case State::Push_Button: update_push_button(delta_time); break;
	case State::Select: update_select(delta_time); break;
	case State::Tutorial: update_tutorial(delta_time); break;
	}
}

//描画
void TitleScene::draw() const {
#ifdef _DEBUG
	glPushMatrix();
	gsFontParameter(GS_FONT_BOLD, 16, "ＭＳ ゴシック");
	gsTextPos(0.0f, 20.0f);
	gsDrawText("1 to EnemyTestScene");
	gsTextPos(0.0f, 40.0f);
	gsDrawText("2 to BossTestScene");
	gsTextPos(0.0f, 60.0f);
	gsDrawText("3 to PlayerTestScene");
	gsTextPos(0.0f, 80.0f);
	gsDrawText("4 to GamePlayScene");
	glPopMatrix();
#endif
	GSvector2 position_titlelogo{300.0f,100.0f};
	gsDrawSprite2D(Texture_TitleLogo, &position_titlelogo, NULL, NULL, NULL, NULL, NULL);

	if (state_ == State::Push_Button && button_timer_ >= Time) {
		GSvector2 position_push_to_button{400.0f,500.0f};
		GSvector2 scale_push_to_button{0.5f,0.5f};
		GScolor color{1,1,1,alphas_[0]};
		gsDrawSprite2D(Texture_PushButton, &position_push_to_button,NULL, NULL, &color, &scale_push_to_button,NULL);
	}
	else if (state_ == State::Select && button_timer_ >= 30.0f) {
		GSvector2 position_game_start{ 400.0f,300.0f };
		GSvector2 scale_game_start{ 0.5f,0.5f };
		GScolor color_game_start{ 1,1,1,alphas_[1] };
		gsDrawSprite2D(Texture_Game_Start, &position_game_start, NULL, NULL, &color_game_start, &scale_game_start, NULL);

		GSvector2 position_tutorial{ 400.0f,500.0f };
		GSvector2 scale_tutorial{ 0.5f,0.5f };
		GScolor color_tutorial{ 1,1,1,alphas_[2] };
		gsDrawSprite2D(Texture_tutorial, &position_tutorial, NULL, NULL, &color_tutorial, &scale_tutorial, NULL);
	}
	else if (state_ == State::Tutorial) {
		const GSvector2 position_1{ pos_x_[0],0.0f };
		gsDrawSprite2D(Texture_How_To_Play1,&position_1, NULL, NULL,NULL, NULL, NULL);
		const GSvector2 position_2{pos_x_[1],0.0f };
		gsDrawSprite2D(Texture_How_To_Play2, &position_2, NULL, NULL, NULL, NULL, NULL);

		if (!is_move_) {
			const GSvector2 position_arrow_left{ 1170.0f,500.0f };
			const GSrect rect_arrow_left{ 0,0,126,100.0f };
			GScolor color_arrow_left{ 1,1,1,alphas_[0] };
			gsDrawSprite2D(Texture_Arrow, &position_arrow_left, &rect_arrow_left, NULL, &color_arrow_left, NULL, NULL);

			const GSvector2 position_arrow_right{ -40.0f,500.0f };
			const GSrect rect_arrow_right{ 126,0.0f,252.0f,100 };
			GScolor color_arrow_right{ 1,1,1,alphas_[0] };
			gsDrawSprite2D(Texture_Arrow, &position_arrow_right, &rect_arrow_right, NULL, &color_arrow_right, NULL, NULL);

			const GSvector2 position_back{ 500.0f,630.0f };
			GSvector2 scale_back{ 0.5f,0.5f };
			GScolor color_back{ 1,1,1,alphas_[0] };
			gsDrawSprite2D(Texture_Back, &position_back, NULL, NULL, &color_back, &scale_back, NULL);
		}
		
	}
	
	fade_.draw();
}

//終了しているか？
bool TitleScene::is_end() const {
	if (fade_.is_end()) {
		return is_end_;
	}else return false;//終了フラグを返す
}

//次のシーンを返す
std::string TitleScene::next() const {
	return nextScene_;//次のシーン名を返す
}

//現在のステージ番号を返す
int TitleScene::stage_number() const {
	return stage_number_;
}

//終了
void TitleScene::end() {
	gsStopMusic();
	gsDeleteMusic(Music_Title);
	gsDeleteSE(SE_Push);
	gsDeleteTexture(Texture_TitleLogo);
	gsDeleteTexture(Texture_PushButton);
	gsDeleteTexture(Texture_Game_Start);
	gsDeleteTexture(Texture_tutorial);
	gsDeleteTexture(Texture_Arrow);
	gsDeleteTexture(Texture_Back);
	gsDeleteTexture(Texture_How_To_Play1);
	gsDeleteTexture(Texture_How_To_Play2);
	button_timer_ = 0.0f;
}

void TitleScene::update_push_button(float delta_time) {
	if (fade_.is_end()) {
		if (gsGetKeyTrigger(GKEY_F) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X)) {
			gsPlaySE(SE_Push);
			state_ = State::Select;
			button_timer_ = 0;
		}
		if (button_timer_ >= Time) {
			update_alpha(0, delta_time);
		}
		else button_timer_ += delta_time;//タイマー更新
	}
}

void TitleScene::update_select(float delta_time) {
	if (button_timer_ >= 30.0f) {
		//左スティックのベクトル値
		GSvector2 vector_stick = GSvector2::zero();
		//左スティックの入力を取得
		gsXBoxPadGetLeftAxis(0, &vector_stick);
		//上下キーまたはパッドの十字ボタンまたは左スティックで選択移動
		if (select_ > 1 && !gsIsPlaySE(SE_Select) && (gsGetKeyState(GKEY_UP) || gsXBoxPadButtonState(0, GS_XBOX_PAD_UP) || vector_stick.y >= 0.5f)) {
			gsPlaySE(SE_Select);
			--select_;
		}
		else if (select_ < Alphas_Size - 1 && !gsIsPlaySE(SE_Select) && (gsGetKeyState(GKEY_DOWN) || gsXBoxPadButtonState(0, GS_XBOX_PAD_DOWN) || vector_stick.y <= -0.5f)) {
			gsPlaySE(SE_Select);
			++select_;
		}
		//FキーまたはXボタンで決定
		if (gsGetKeyTrigger(GKEY_F) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X)) {
			gsPlaySE(SE_Push);
			switch (select_)
			{
			case 1:
				nextScene_ = "GamePlayScene";
				is_end_ = true;
				fade_.change_fade_flg();
				break;
			case 2: state_ = State::Tutorial;
				break;
			}
		}
		//クランプする
		select_ = CLAMP(select_, 1, Alphas_Size - 1);
	}
	else button_timer_ += delta_time;
	update_alpha(select_, delta_time);
	
}

void TitleScene::update_tutorial(float delta_time) {
	if (!is_move_ &&gsGetKeyTrigger(GKEY_F) || gsXBoxPadButtonTrigger(0, GS_XBOX_PAD_X)) {
		state_ = State::Select;
	}
	//左スティックのベクトル値
	GSvector2 vector_stick = GSvector2::zero();
	//左スティックの入力を取得
	gsXBoxPadGetLeftAxis(0, &vector_stick);
	//上下キーまたはパッドの十字ボタンまたは左スティックで選択移動
	if (!is_move_ && num_ > 0 && !gsIsPlaySE(SE_Select) && (gsGetKeyState(GKEY_LEFT) || gsXBoxPadButtonState(0, GS_XBOX_PAD_LEFT) || vector_stick.x <= -0.5f)) {
		gsPlaySE(SE_Select);
		--num_;
		is_move_ = true;
	}
	else if (!is_move_ && num_ < Pos_Size - 1 && !gsIsPlaySE(SE_Select) && (gsGetKeyState(GKEY_RIGHT) || gsXBoxPadButtonState(0, GS_XBOX_PAD_RIGHT) || vector_stick.x >= 0.5f)) {
		gsPlaySE(SE_Select);
		++num_;
		is_move_ = true;
	}
	//クランプする
	num_ = CLAMP(num_, 0, Pos_Size - 1);
	
	if (is_move_) {
		move_timer_ = MIN(move_timer_ + delta_time, MovingTime);
		if (num_ == 1 && pos_x_[num_] > 0.0f) {
			GSvector2 position = pos_x_in.lerp(pos_x_out, move_timer_ / MovingTime);
			pos_x_[0] = position.x;
			GSvector2 position2 = pos_x_out2.lerp(pos_x_in2, move_timer_ / MovingTime);
			pos_x_[1] = position2.x;
		}
		else if (num_ == 0 && pos_x_[num_] < 0.0f) {
			GSvector2 position = pos_x_out.lerp(pos_x_in, move_timer_ / MovingTime);
			pos_x_[0] = position.x;
			GSvector2 position2 = pos_x_in2.lerp(pos_x_out2, move_timer_ / MovingTime);
			pos_x_[1] = position2.x;
		}
		else {
			is_move_ = false;
			move_timer_ = 0;
		}
	}
	else update_alpha(0, delta_time);
	
}

//α値の更新
void TitleScene::update_alpha(int num, float delta_time) {
	//α値の更新処理
	for (int i = 0; i < Alphas_Size; ++i) {
		if (i == num) {
			//α値の更新
			alphas_[i] += (alpha_flg_) ? -Alpha_Value * delta_time : Alpha_Value * delta_time;
		}
		else alphas_[i] = 1.0f;
	}

	//増減フラグを制御
	if (alphas_[num] >= 1.1f || alphas_[num] <= 0.0f) {
		alpha_flg_ = !alpha_flg_;
	}
}