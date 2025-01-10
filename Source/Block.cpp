#include "Block.h"
#include "Collision.h"
#include <cmath>

BlockManager::BlockManager() {
    Initialize();
}

BlockManager::~BlockManager() {
    for (auto& model : blockModels) {
        delete model;
    }
    blockModels.clear();
}

void BlockManager::Initialize() {
    blocks.resize(mapX * mapY * mapZ);

    for (int y = 0; y < mapY; ++y) {
        for (int x = 0; x < mapX; ++x) {
            for (int z = 0; z < mapZ; ++z) {
                blocks[GetIndex(x, y, z)] = { 0, {}, {}, {}, {}, false };
            }
        }
    }

    blockModels = {
        new Model("Data/Model/Block/Block1.mdl"),
        new Model("Data/Model/Block/Block2.mdl"),
        new Model("Data/Model/Block/Block3.mdl"),
        new Model("Data/Model/Block/Block4.mdl"),
        new Model("Data/Model/Block/Block5.mdl"),
        new Model("Data/Model/Block/BlockStop.mdl"),
        new Model("Data/Model/Block/AccelBlock.mdl")
    };
}

int BlockManager::GetIndex(int x, int y, int z) const {
    return y * mapX * mapZ + z * mapX + x;
}

void BlockManager::SetBlock(int x, int y, int z, int type, const DirectX::XMFLOAT3& angle) {
    if (x < 0 || x >= mapX || y < 0 || y >= mapY || z < 0 || z >= mapZ) return;

    BlockData& block = blocks[GetIndex(x, y, z)];
    block.id = type;
    block.angle = angle;
    block.position = {
        (x - mapX / 2) * BlockSize,
        y * BlockSize,
        (z - mapZ / 2) * BlockSize
    };
    block.orgPosition = block.position;
    DirectX::XMStoreFloat4x4(
        &block.transform,
        DirectX::XMMatrixTranslation(block.position.x, block.position.y, block.position.z)
    );
}

void BlockManager::RemoveBlock(int x, int y, int z) {
    if (x < 0 || x >= mapX || y < 0 || y >= mapY || z < 0 || z >= mapZ) return;

    BlockData& block = blocks[GetIndex(x, y, z)];
    block = { 0, {}, {}, {}, {}, false };
}

BlockManager::BlockData* BlockManager::GetBlockData(int x, int y, int z) {
    if (x < 0 || x >= mapX || y < 0 || y >= mapY || z < 0 || z >= mapZ) return nullptr;

    return &blocks[GetIndex(x, y, z)];
}

void BlockManager::Update(float elapsedTime) {
    MoveBlocks(elapsedTime);
}

void BlockManager::MoveBlocks(float elapsedTime) {
    constexpr float MoveSpeed = 2.0f;
    constexpr float MoveRange = 6.0f;

    for (BlockData& block : blocks) {
        if (block.id != 8) continue;

        if (block.moveForward) {
            block.position.z += MoveSpeed * elapsedTime;
            if (block.position.z >= block.orgPosition.z + MoveRange) {
                block.moveForward = false;
            }
        }
        else {
            block.position.z -= MoveSpeed * elapsedTime;
            if (block.position.z <= block.orgPosition.z - MoveRange) {
                block.moveForward = true;
            }
        }

        DirectX::XMStoreFloat4x4(
            &block.transform,
            DirectX::XMMatrixTranslation(block.position.x, block.position.y, block.position.z)
        );
    }
}

bool BlockManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
    DirectX::XMFLOAT3& hitPosition, DirectX::XMFLOAT3& hitNormal, int& hitBlockID) {
    for (const BlockData& block : blocks) {
        if (block.id == 0) continue;

        Model* model = blockModels[block.id - 1];
        if (Collision::RayCast(start, end, block.transform, model, hitPosition, hitNormal)) {
            hitBlockID = block.id;
            return true;
        }
    }
    return false;
}

void BlockManager::RenderBlocks(const RenderContext& rc, ModelRenderer* renderer) {
    for (const BlockData& block : blocks) {
        if (block.id == 0) continue;

        Model* model = blockModels[block.id - 1];
        renderer->Render(rc, block.transform, model, ShaderId::Lambert);
    }
}
