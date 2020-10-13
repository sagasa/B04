#ifndef GSLIB_GS_GAME_H_
#define GSLIB_GS_GAME_H_

#include <gslib.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <functional>
#include <string>
#include <stdexcept>

#pragma comment(linker, "/entry:mainCRTStartup")

namespace gslib {

// GSLIBウィンドウクラス
class Window {
public:
	// コンストラクタ
	Window(int width, int height, const std::string& name, bool fullScreen, float refreshRate) : window_(NULL) {
		if (!glfwInit()) {
			throw std::runtime_error("error glfwInit");
		}
//		glfwWindowHint(GLFW_SAMPLES, 4);
		auto monitor = fullScreen ? glfwGetPrimaryMonitor() : NULL;
        glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);
		window_ = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);
		if (!window_)  {
			glfwTerminate();
			throw std::runtime_error("error glfwCreateWindow");
		}
        // 現在のモニターを取得
        monitor_ = glfwGetWindowMonitor(window_);
        if (monitor_ == NULL) {
            monitor_ = glfwGetPrimaryMonitor();
        }
        // 現在のリフレッシュレートを保存
        const  GLFWvidmode * mode = glfwGetVideoMode(monitor_);
        refreshRate_ = mode->refreshRate;
		glfwMakeContextCurrent(window_);
		glfwSetWindowUserPointer(window_, this);
	}
	// デストラクタ
	~Window() {
		glfwTerminate();
	}
	// ウィンドウを閉じたか？
	bool isClose() const {
		return glfwWindowShouldClose(window_) != 0;
	}
	// ダブルバッファの交換
	void swapBuffers() {
		glfwSwapBuffers(window_);
	}
	// イベントのポーリング
	void pollEvents() {
		glfwPollEvents();
	}
	// スワップインターバルの設定
	void swapInterval(int interval) {
		glfwSwapInterval(interval);
	}
	// フォーカス時のコールバックを設定
	void registerForcusCallback(std::function<void(bool)> fn) {
		forcusCallback_ = fn;
		glfwSetWindowFocusCallback(window_, &forcusCallback);
	}
	// リサイズ時のコールバックを設定
	void registerResizeCallback(std::function<void(int, int)> fn) {
		resizeCallback_ = fn;
		glfwSetWindowSizeCallback(window_, &resizeCallback);
	}
	// ウィンドウハンドラの取得
	HWND getHWND() const {
		return glfwGetWin32Window(window_);
	}
	// タイマの取得
	double getTime() const {
		return glfwGetTime();
	}
    // リフレッシュレートの取得
	double getRefreshRate() const {
        return refreshRate_;
    }

private:
	static void forcusCallback(GLFWwindow* window, int focused) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->forcusCallback_(focused == GL_TRUE);
	}
	static void resizeCallback(GLFWwindow* window, int width, int height) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->resizeCallback_(width, height);
	}
private:
	// コピー禁止
	Window(const Window& other);
	Window& operator = (const Window& other);

private:
	// GLFWのウィンドウ
	GLFWwindow* window_;
    // 現在のモニター
    GLFWmonitor* monitor_;
	// アクティブになった時の処理
	std::function<void(bool)> forcusCallback_;
	// サイズ変更時の処理
	std::function<void(int, int)> resizeCallback_;
	// クローズ時の処理
	std::function<void(void)> closeCallback_;
    // 実際のリフレッシュレート
    double refreshRate_;
};

// GSLIBデバイスクラス
class Device {
public:
	// コンストラクタ
	Device(int width, int height, bool fullScreen, float refreshRate) :
		window_(width, height, "Game Window", fullScreen, refreshRate),
		frameTime_(1.0 / window_.getRefreshRate()),
		previosTime_(window_.getTime()),
		deltaTime_(0.0) {
		// ウィンドウサイズ変更
		resize(width, height);
		// ウィンドウサイズ変更時の関数を設定
		window_.registerResizeCallback([=](int w, int h) { resize(w, h); });
		// ウィンドウアクティブ時の関数を設定
		window_.registerForcusCallback([=](bool flag) { activate(flag); });
		// Ｖｓｙｎｃを有効にする
		window_.swapInterval(1);
		// ＯｐｅｎＧＬの初期化
		initilaizeOpneGL();
		// 乱数の初期化
		gsRandamize();
		// サウンドシステムの初期化
		gsInitSound(window_.getHWND());
		// 入力デバイスシステムの初期化
		gsInitInput(window_.getHWND());
		// グラフィックスシステムの初期化
		gsInitGraphics();
	}
	// デストラクタ
	~Device() {
		// グラフィックスシステムの終了処理
		gsFinishGraphics();
		// 入力デバイスシステムの終了処理
		gsFinishInput();
		// サウンドシステムの終了処理
		gsFinishSound();
	}
	// バッファをスワップ
	void swap() {
		window_.swapBuffers();
		window_.pollEvents();
		gsReadInput();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 最低時間のウェイト
		while ((window_.getTime() - previosTime_) < frameTime_) ;
		deltaTime_ = window_.getTime() - previosTime_;
		previosTime_ = window_.getTime();
	}
	// クローズするか？
	bool isClose() {
		return window_.isClose();
	}
    // リフレッシュレートの取得
	double getRefreshRate() const {
        return window_.getRefreshRate();
    }
	// デルタタイムの取得
	double getDeltaTime() const {
		return deltaTime_;
	}

private:
	void initilaizeOpneGL() {
		// 面カリングの処理を有効にする
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		// デプスバッファを1.0でクリアする
		glClearDepth(1.0);
		// テプステストを有効にする
		glEnable(GL_DEPTH_TEST);
		// パースペクティブ補正を行う
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		// ブレンドを有効
		glEnable(GL_BLEND);
		// デフォルトのブレンド式を設定
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// アルファテストを有効
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
		// スペキュラの計算を分離
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		// デフォルトの視点変換の変換行列を設定する
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			0.0f, 0.0f, 40.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		);
		// デフォルトのライトの設定
		static const float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		static const float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		static const float lightSpeclar[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		static const float lightPosition[] = { 100.0f, 100.0f, 100.0f, 0.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpeclar);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		// 画面消去
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void activate(bool state) {
		// 入力デバイスシステムにアクティブ状態を伝える
		gsActivateInput(state ? GS_TRUE : GS_FALSE);
		// サウンドシステムにアクティブ状態を伝える
		gsActivateSound(state ? GS_TRUE : GS_FALSE);
	}
	void resize(int width, int height) {
		// 高さが０にならないように調整
		height = (height == 0) ? 1 : height;
		// ビューポートの設定
		glViewport(0, 0, width, height);
		// 透視射影の変換行列を設定する
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(
			45.0f,
			(float)width / (float)height,
			0.3f,
			1000.0f
		);
		// モデルビューモードにする
		glMatrixMode(GL_MODELVIEW);
	}
private:
	// コピー禁止
	Device(const Device& other);
	Device& operator = (const Device& other);
private:
	// ウィンドウクラス
	Window window_;
	// フレームタイマ
	double frameTime_;
	// 前フレームの時間
	double previosTime_;
	// デルタタイム
	double deltaTime_;
};

// ゲームクラス
class Game {
protected:
	// コンストラクタ
	Game(int width = 640, int height = 480, bool fullScreen = false, float refreshRate = 60.0f) :
		width_(width),
		height_(height),
		fullScreen_(fullScreen),
        refreshRate_(refreshRate),
		device_(width, height, fullScreen, refreshRate) {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);// メモリリーク検出
	}

public:
	// 実行
	int run() {
		start();
		while (!device_.isClose() && isRunning()) {
			float deltaTime = device_.getDeltaTime() / (1.0 / 60.0);
			deltaTime = (deltaTime > 2.0) ? 2.0 : deltaTime;
			update((float)deltaTime);
			draw();
			device_.swap();
		}
		end();
		return 0;
	}

private:
	// 開始
	virtual void start() {}
	// 更新
	virtual void update(float time) { (void)time; }
	// 描画
	virtual void draw() {}
	// 終了
	virtual void end() {}
	// 実行中か
	virtual bool isRunning() { return GetAsyncKeyState(VK_ESCAPE) == 0; }

private:
	// コピー禁止
	Game(const Game& other);
	Game& operator = (const Game& other);

private:
	// ウィンドウの幅
	int width_;
	// ウィンドウの高さ
	int height_;
	// フルスクリーンか？
	bool fullScreen_;
	// FPS
	float refreshRate_;
	// GSLIBデバイスクラス
	Device device_;
};

} // namespace gslib

#endif
