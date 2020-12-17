#ifndef GS_GAME_GLUT_H_
#define GS_GAME_GLUT_H_

#include <string>
#include <gslib.h>
#include <sstream>

#pragma comment(linker, "/entry:mainCRTStartup")

// ゲームクラス
class Game {
public:
    int run();
protected:
    Game(int width = 640, int height = 480, bool fullScreen = false, float refreshRate = 60.0f);
    virtual ~Game() {}
    virtual void start() {}
    virtual void update(float frameTimer) {}
    virtual void draw() {}
    virtual void end() {}

private:
    void initilaizeGL(void);
    void setSwapInterval(int interval);
    static void activate(int state);
    static void display(void);
    static void idle(void);
    static void reshape(int width, int height);
    static void destroy(void);
    void wait();
    static GSdouble getTime();

private:
    std::string mWindowTitle;
    int         mWindowPositionX;
    int         mWindowPositionY;
    int         mWindowWidth;
    int         mWindowHeight;
    bool        mIsFullScreenMode;
    float       mPerspectiveFov;
    float       mPerspectiveNear;
    float       mPerspectiveFar;
    double      mPreviosTime;
    double      mFrameTime;
    double      mDeltaTime;

    static Game* mInstance;
};

inline Game::Game(int width, int height, bool fullScreen, float refreshRate) :
    mWindowTitle("GameWindow"),
    mWindowPositionX(0),
    mWindowPositionY(0),
    mWindowWidth(width),
    mWindowHeight(height),
    mIsFullScreenMode(fullScreen),
    mPerspectiveFov(45.0f),
    mPerspectiveNear(0.3f),
    mPerspectiveFar(1000.0f),
    mFrameTime(1.0 / refreshRate),
    mPreviosTime(0.0),
    mDeltaTime(0.0f) {
    // 自分自身のインスタンスを設定
    mInstance = this;

    // ＧＬＵＴの初期化
    int argc = 1;
    char name[] = "game.exe";
    char* argv[] = { name };
    glutInit(&argc, argv);

    // ウィンドウの設定を行う
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(mWindowPositionX, mWindowPositionY);
    glutInitWindowSize(mWindowWidth, mWindowHeight);
    // フルスクリーンモードか？
    if (mIsFullScreenMode) {
        std::ostringstream mode;
        // フルスクリーンモード設定文字列を作成
        mode << mWindowWidth << "x" << mWindowHeight << "@" << (int)refreshRate;
        // フルスクリーンモードに設定
        glutGameModeString(mode.str().c_str());
        glutEnterGameMode();
        // フルスクリーンの場合はマウスカーソルを消す
        glutSetCursor(GLUT_CURSOR_NONE);
    } else {
        // ウィンドウモード
        glutCreateWindow(mWindowTitle.c_str());
    }
    // ＯｐｅｎＧＬの初期化
    initilaizeGL();
    // 乱数の初期化
    gsRandamize();
    // グラフィックスシステムの初期化
    gsInitGraphics();
    // サウンドシステムの初期化
    gsInitSound((HWND)_glutGetHWND());
    // 入力デバイスシステムの初期化
    gsInitInput((HWND)_glutGetHWND());
    // Ｖｓｙｎｃを有効にする
    setSwapInterval(1);
}

inline int Game::run() {
    // アプリケーションの初期化処理
    start();
    // 終了処理の関数を設定する
    _glutDestroyFunc(destroy);
    // ウィンドウサイズ変更時の関数を設定
    glutReshapeFunc(reshape);
    // 表示処理の関数を設定
    glutDisplayFunc(display);
    // イベントのない時の関数を設定
    glutIdleFunc(idle);
    // ウィンドウがアクティブになった時の関数を設定
    _glutActivateFunc(activate);
    // フレームタイマのリセット
    gsFrameTimerReset();
    // メインループ処理を行う
    glutMainLoop();
    return 0;
}

inline void Game::initilaizeGL() {
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

inline void Game::destroy(void) {
    // ゲーム終了処理
    mInstance->end();
    // 入力デバイスシステムの終了処理
    gsFinishInput();
    // サウンドシステムの終了処理
    gsFinishSound();
    // グラフィックスシステムの終了処理
    gsFinishGraphics();
}

inline void Game::reshape(int width, int height) {
    // 高さが０にならないように調整
    height = (height == 0) ? 1 : height;
    // ビューポートの設定
    glViewport(0, 0, width, height);
    // 透視射影の変換行列を設定する
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        mInstance->mPerspectiveFov,
        (float)width / (float)height,
        mInstance->mPerspectiveNear,
        mInstance->mPerspectiveFar
    );
    // モデルビューモードにする
    glMatrixMode(GL_MODELVIEW);
    // ウィンドウの幅と高さを更新する
    mInstance->mWindowWidth = width;
    mInstance->mWindowHeight = height;
}

inline void Game::idle(void) {
    // 入力デバイスの読み込み
    gsReadInput();
    // ゲーム更新処理
    double deltaTime = mInstance->mDeltaTime * 60.0;
    deltaTime = (deltaTime > 2.0) ? 2.0 : deltaTime;
    mInstance->update((float)deltaTime);
    // エスケープキーが押されたか？
    if (GetAsyncKeyState(VK_ESCAPE) != 0) {
        if (mInstance->mIsFullScreenMode) {
            // ゲームモードを終了する
            glutLeaveGameMode();
        } else {
            // 終了処理を呼び出す
            mInstance->destroy();
        }
        // 強制終了する
        std::exit(0);
    }
    // 描画処理を呼び出す
    glutPostRedisplay();
}

inline void Game::display(void) {
    // 画面クリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ゲーム描画処理
    mInstance->draw();
    // ダブルバッファの切り替え
    glutSwapBuffers();
    // タイマウェイト
    mInstance->wait();
}

inline void Game::activate(int state) {
    // 入力デバイスシステムにアクティブ状態を伝える
    gsActivateInput(state);
    // サウンドシステムにアクティブ状態を伝える
    gsActivateSound(state);
}

inline void Game::setSwapInterval(int interval) {
    // wglSwapIntervalEXT拡張関数のポインタ関数型
    typedef BOOL(WINAPI * LPFNWGLSWAPINTERVALEXTPROC)(int interval);
    LPFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
    // SwapIntervalEXTの関数ポインタを取得
    wglSwapIntervalEXT = (LPFNWGLSWAPINTERVALEXTPROC)
        wglGetProcAddress("wglSwapIntervalEXT");
    // SwapIntervalEXT関数が取得できたか？
    if (wglSwapIntervalEXT != NULL) {
        // スワップのインターバルを設定
        wglSwapIntervalEXT(interval);
    }
}

inline void Game::wait() {
    // 最低限のウェイト
    while ((getTime() - mPreviosTime) < mFrameTime);
    // デルタタイムの計算
    mDeltaTime = getTime() - mPreviosTime;
    // 前回タイマを保存
    mPreviosTime = getTime();
}

inline GSdouble Game::getTime() {
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    LARGE_INTEGER		freq;
    QueryPerformanceFrequency(&freq);
    return (GSdouble)time.QuadPart / (GSdouble)freq.QuadPart;
}

// コールバック関数のための静的メンバ変数
Game* Game::mInstance = 0;

#endif

