#pragma once

#include "Scene.h"
#include <DirectXMath.h>
#include "System/ShapeRenderer.h"
#include "System/Sprite.h"
#include "System/Graphics.h"

constexpr int TotalLevels = 4;// 全ステージの数  

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
    void Select(float elapsedTime);
private:
    void UpdatePageOffset(float elapsedTime);

    // サムネイル画像をロードする
    void LoadThumbnailImages();

    int Lev = 0;

    int CurrentPage = 0;
    float PageCool = 0;
    float SwipeOffset = 0.0f;   // ページのオフセット
    float swipeTime = 0.0f;     // スワイプの進行状況
    float SwipeSpeed = 70;
    float pageOffset = 0;
    bool IsScore = false;
    // レベル毎の設定

    const int LevelWide = 1;       // 横に並べるサムネイル数
    const int LevelHeight = 1;     // 縦に並べるサムネイル数
    const int LevelsPerPage = (LevelWide * LevelHeight);  // 1ページに表示するレベル数
    const int TotalPages = (TotalLevels + LevelsPerPage - 1) / LevelsPerPage; // 総ページ数

    bool IsSwiping = false;

    // サムネイルの通常サイズ
    const  int thumbnailWidth = 500;
    const  int thumbnailHeight = 500;

    // サムネイルの拡大時の幅と高さ
    const int thumbnailBigWidth = 500;  // 幅
    const int thumbnailBigHeight = 500; // 高さ



    std::vector<Sprite> sprites; // スプライトの配列

};
