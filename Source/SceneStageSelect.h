#pragma once

#include "Scene.h"
#include <DirectXMath.h>
#include "System/ShapeRenderer.h"
#include "System/Sprite.h"
#include "System/Graphics.h"

constexpr int TotalLevels = 200;// 全ステージの数  

//levelセレクト
class SceneStageSelect :public Scene
{
public:
    SceneStageSelect() {};
    ~SceneStageSelect() {};
    void Initialize()override;
    void Finalize()override;
    void Update(float elapsedTime)override;
    void Render()override;
    void DrawGUI() override{};

private:
    void UpdatePageOffset(float elapsedTime);
    bool IsMouseOnRightEdge();
    bool IsMouseOnLeftEdge();

    // サムネイル画像をロードする
    void LoadThumbnailImages();

    //キーが押されたときの検知用
    bool IsKeyPressed(int key) {
        return (GetAsyncKeyState(key) & 0x8000) != 0;
    }

    int Lev = 0;

    int CurrentPage = 0;
    float PageCool = 0;
    float SwipeOffset = 0.0f;   // ページのオフセット
    float swipeTime = 0.0f;     // スワイプの進行状況
    float SwipeSpeed = 70;
    float pageOffset = 0;
    // レベル毎の設定

    const int LevelWide = 3;       // 横に並べるサムネイル数
    const int LevelHeight = 3;     // 縦に並べるサムネイル数
    const int LevelsPerPage = (LevelWide * LevelHeight);  // 1ページに表示するレベル数
    const int TotalPages = (TotalLevels + LevelsPerPage - 1) / LevelsPerPage; // 総ページ数

    bool IsSwiping = false;

    // サムネイルの通常サイズ
    const  int thumbnailWidth = 256;
    const  int thumbnailHeight = 256;

    // サムネイルの拡大時の幅と高さ
    const int thumbnailBigWidth = 300;  // 幅
    const int thumbnailBigHeight = 300; // 高さ

    Sprite* sprite = nullptr;

    std::vector<Sprite> sprites; // スプライトの配列

};
