#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

class StageSaveManager {
public:
    struct StageData {
        bool isCleared = false;// ステージがクリアされたか
        float shortestTime = -1;    // 最短クリア時間
        int lowestCost = -1;        // 最少コスト
    };

    // シングルトンインスタンスの取得
    static StageSaveManager& Instance() {
        static StageSaveManager instance("Data/SaveDate/StageData.txt");
        return instance;
    }

    // セーブデータをロード
    void Load() {
        // ファイルを開く
        std::ifstream scan(saveFileName);
        if (!scan.is_open()) {
            // ファイルが存在しない場合は何もせず終了
            return;
        }

        int stageID;
        bool isCleared;
        float shortestTime;
        int lowestCost;

        while (scan >> stageID >> isCleared >> shortestTime >> lowestCost) {
            StageData data;
            data.isCleared = isCleared;
            data.shortestTime = shortestTime;
            data.lowestCost = lowestCost;
            stageData[stageID] = data;
        }
        scan.close();
    }

    // セーブデータを保存
    void Save() const {
        // ファイルがない場合は生成される（std::ofstreamの仕様）
        std::ofstream put(saveFileName, std::ios::out);
        if (!put) {
            return;
        }

        for (const auto& [stageID, data] : stageData) {
            put << stageID << " " << data.isCleared << " "
                << data.shortestTime << " " << data.lowestCost << "\n";
        }
        put.close();
    }

    // ステージのクリア状態を更新
    void SetStageCleared(int stageID, bool isCleared) {
        stageData[stageID].isCleared = isCleared;
    }

    // ステージの最短時間を更新
    void SetShortestTime(int stageID, float time) {
        if (time < stageData[stageID].shortestTime || stageData[stageID].shortestTime < 0) {
            stageData[stageID].shortestTime = time;
        }
    }

    // ステージの最少コストを更新
    void SetLowestCost(int stageID, int cost) {
        if (cost < stageData[stageID].lowestCost || stageData[stageID].lowestCost < 0) 
        {
            stageData[stageID].lowestCost = cost;
        }
    }

    // ステージのデータを取得
    StageData GetStageData(int stageID) const {

        auto it = stageData.find(stageID);
        if (it != stageData.end()) {
            return it->second;
        }
        return {}; // デフォルト値を返す
    }

    void AllSet(int stageID, bool isCleared, float time, int cost)
    {
        SetStageCleared(stageID, isCleared);
        SetShortestTime(stageID, time);
        SetLowestCost(stageID, cost);
    }

private:
    std::string saveFileName;                           // セーブファイル名
    std::unordered_map<int, StageData> stageData;       // ステージIDごとのデータ

    // コンストラクタ（privateでシングルトン化）
    StageSaveManager(const std::string& filename) : saveFileName(filename) {}

    // コピーコンストラクタと代入演算子を削除してシングルトンを保証
    StageSaveManager(const StageSaveManager&) = delete;
    StageSaveManager& operator=(const StageSaveManager&) = delete;
};

