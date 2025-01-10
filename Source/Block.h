#pragma once


#include "System/ModelRenderer.h"
#include "MapDate.h"
#include <vector>

class BlockManager {
public:
    struct BlockData {
        int id;
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 orgPosition;
        DirectX::XMFLOAT3 angle;
        DirectX::XMFLOAT4X4 transform;
        bool moveForward;
    };

    static constexpr int mapX = 50;
    static constexpr int mapY = 10;
    static constexpr int mapZ = 50;
    static constexpr float BlockSize = 1.0f;

    BlockManager();
    ~BlockManager();

    void Initialize();
    void Update(float elapsedTime);
    void SetBlock(int x, int y, int z, int type, const DirectX::XMFLOAT3& angle);
    void RemoveBlock(int x, int y, int z);
    BlockData* GetBlockData(int x, int y, int z);
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        DirectX::XMFLOAT3& hitPosition, DirectX::XMFLOAT3& hitNormal, int& hitBlockID);

    void RenderBlocks(const RenderContext& rc, ModelRenderer* renderer);

private:
    std::vector<BlockData> blocks;
    std::vector<Model*> blockModels;

    int GetIndex(int x, int y, int z) const;
    void MoveBlocks(float elapsedTime);
};