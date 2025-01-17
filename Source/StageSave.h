#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

class StageSaveManager {
public:
    struct StageData {
        bool isCleared = false;// �X�e�[�W���N���A���ꂽ��
        float shortestTime = -1;    // �ŒZ�N���A����
        int lowestCost = -1;        // �ŏ��R�X�g
    };

    // �V���O���g���C���X�^���X�̎擾
    static StageSaveManager& Instance() {
        static StageSaveManager instance("Data/SaveDate/StageData.txt");
        return instance;
    }

    // �Z�[�u�f�[�^�����[�h
    void Load() {
        // �t�@�C�����J��
        std::ifstream scan(saveFileName);
        if (!scan.is_open()) {
            // �t�@�C�������݂��Ȃ��ꍇ�͉��������I��
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

    // �Z�[�u�f�[�^��ۑ�
    void Save() const {
        // �t�@�C�����Ȃ��ꍇ�͐��������istd::ofstream�̎d�l�j
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

    // �X�e�[�W�̃N���A��Ԃ��X�V
    void SetStageCleared(int stageID, bool isCleared) {
        stageData[stageID].isCleared = isCleared;
    }

    // �X�e�[�W�̍ŒZ���Ԃ��X�V
    void SetShortestTime(int stageID, float time) {
        if (time < stageData[stageID].shortestTime || stageData[stageID].shortestTime < 0) {
            stageData[stageID].shortestTime = time;
        }
    }

    // �X�e�[�W�̍ŏ��R�X�g���X�V
    void SetLowestCost(int stageID, int cost) {
        if (cost < stageData[stageID].lowestCost || stageData[stageID].lowestCost < 0) 
        {
            stageData[stageID].lowestCost = cost;
        }
    }

    // �X�e�[�W�̃f�[�^���擾
    StageData GetStageData(int stageID) const {

        auto it = stageData.find(stageID);
        if (it != stageData.end()) {
            return it->second;
        }
        return {}; // �f�t�H���g�l��Ԃ�
    }

    void AllSet(int stageID, bool isCleared, float time, int cost)
    {
        SetStageCleared(stageID, isCleared);
        SetShortestTime(stageID, time);
        SetLowestCost(stageID, cost);
    }

private:
    std::string saveFileName;                           // �Z�[�u�t�@�C����
    std::unordered_map<int, StageData> stageData;       // �X�e�[�WID���Ƃ̃f�[�^

    // �R���X�g���N�^�iprivate�ŃV���O���g�����j
    StageSaveManager(const std::string& filename) : saveFileName(filename) {}

    // �R�s�[�R���X�g���N�^�Ƒ�����Z�q���폜���ăV���O���g����ۏ�
    StageSaveManager(const StageSaveManager&) = delete;
    StageSaveManager& operator=(const StageSaveManager&) = delete;
};

