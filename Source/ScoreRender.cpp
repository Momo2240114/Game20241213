#include "ScoreRender.h"

const LONG SCREEN_WIDTH = 1920;
const LONG SCREEN_HEIGHT = 1080;


void ScoreRender::RenderNumber(RenderContext rc, int number, int xOffsetBase, int yOffset, int maxDigits)
{
    // 桁数を計算
    int digitCount = 0;
    int temp = number;
    do {
        digitCount++;
        temp /= 10;
    } while (temp > 0);

    // 最大桁数を超える場合を考慮
    digitCount = min(digitCount, maxDigits);

    // 不足している桁数分0で埋める
    for (int i = digitCount; i < maxDigits; i++) {
        Number->Render(rc,
            xOffsetBase + (maxDigits - i - 1) * 200, yOffset + MoveY, 0,  // 表示位置
            200, 200, 0, 0,  // サイズと切り抜き位置
            NumberSizeX, NumberSizeY,  // 切り抜きサイズ
            0, 1, 1, 1, 1  // 色や透明度設定
        );
    }

    // 描画開始位置を調整 (右詰め対応)
    int xOffset = xOffsetBase + (maxDigits - digitCount) * 200;

    // 各桁を描画 (大きい桁から順に)
    temp = number;
    for (int i = 0; i < digitCount; i++) {
        int digit = temp / static_cast<int>(pow(10, digitCount - i - 1)); // 各桁の値
        temp %= static_cast<int>(pow(10, digitCount - i - 1));            // 残りの数値

        Number->Render(rc,
            xOffset, yOffset + MoveY, 0,          // 表示位置
            200, 200,                     // 表示サイズ
            digit * NumberSizeX, 0,       // 切り抜き位置
            NumberSizeX, NumberSizeY,     // 切り抜きサイズ
            0, 1, 1, 1, 1                 // 色や透明度設定
        );
        xOffset += 200; // 次の桁に進む
    }
}

void ScoreRender::ResetMoveY()
{
    MoveY = -1800;
}

void ScoreRender::UpdateMoveY(float deltaTime)
{
    // deltaTimeを基にY座標を増加（例: 1秒間で200ピクセル降下）
    MoveY += 2000.0f * deltaTime;

    // 画面下に行き過ぎたらリセット（必要に応じて変更）
    if (MoveY > 0) {
        MoveY = 0;
    }
}

void ScoreRender::init()
{
	Back = new Sprite("Data/Sprite/ScoreBack.png");
	IsClear = new Sprite("Data/Sprite/IsCleared.png");
	Number = new Sprite("Data/Sprite/Number.png");

    StageSaveManager::Instance().Load();
}

void ScoreRender::Render(int L, RenderContext rc)
{
	Back->Render(rc,
		0, 0 + MoveY, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0,
		1, 1, 1, 1
	);
    // ステージデータの取得
    auto stageData = StageSaveManager::Instance().GetStageData(L);

    Level(rc, L + 1);
   
    // クリア状況の描画
    if (stageData.isCleared) {
        // クリアしている場合
        IsClear->Render(rc,
            1150, 300 + MoveY, 0, // 表示位置
            200, 200, // 表示サイズ
            300, 0, // 切り抜き位置 (x=0, y=0)
            300, 300, // 切り抜きサイズ
            0, 1, 1, 1, 1
        );
    }
    else {
        // 未クリアの場合
        IsClear->Render(rc,
            1150, 300 + MoveY, 0, // 表示位置
            200, 200, // 表示サイズ
            0, 0, // 切り抜き位置 (x=0, y=0)
            300, 300, // 切り抜きサイズ
            0, 1, 1, 1, 1
        );
    }
    if (stageData.isCleared) {

        Time(rc, stageData.shortestTime);
        Cost(rc, stageData.lowestCost);
    }
}

void ScoreRender::Time(RenderContext rc, float Time)
{
    // 時間の整数部分のみ取得
    int timeInt = static_cast<int>(Time);

    // 最大4桁の整数部を描画
    RenderNumber(rc, timeInt, 850, 575, 4);
}

void ScoreRender::Cost(RenderContext rc, int Cost)
{
    // 最少コストの描画 (最大4桁)
    RenderNumber(rc, Cost, 850, 810, 4);
}

void ScoreRender::Level(RenderContext rc, int L)
{
    // レベルの描画 (最大4桁)
    RenderNumber(rc, L, 1100, 0, 2);
}