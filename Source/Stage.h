#pragma once


#include "System/ModelRenderer.h"
#include "MapDate.h"
#include <vector>

struct  MAPDate
{
    static const int mapX = 45;
    static const int mapZ = 45;
    static const int mapY = 10;
    int  BlockID[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 position[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 angle[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 scale = { 2.0f,  2.0f, 2.0f };
    DirectX::XMFLOAT4X4 transform[mapY][mapX][mapZ] = {};

};

//ステージ
class Stage
{
public:

    static Stage& Instance();

    void Finalize();
    //ブロックの設置
    void putBlock(int Type,const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT3& Angle);
    //更新処理
    void Update(float elapsedTime);
    ////ブロックの設置
    //void SetMapdate(int Level);
    //描画処理
    void Render(const RenderContext& rc, ModelRenderer* renderer);   
    void BlockRender(const RenderContext& rc, ModelRenderer* renderer);   
    //3Dマップの更新
    void UpdateTransform();
    //3Dマップチップの更新
    void UpdateBlockTransform();
    //初期化
    void init();
    //レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal);
    bool BlockRayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal
        ,int& HitBlok);
    //最強レイキャスト
    bool UnifiedRayCast(
        const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        DirectX::XMFLOAT3& hitPosition, DirectX::XMFLOAT3& hitNormal,
        int& hitBlock, bool isPlayer, bool checkBlocks = false);

    float GetBlockSize() { return Blocksize; }
    DirectX::XMFLOAT3 GetBlockscale() { return MapDate.scale; }
    int GetMapXsize() { return MAPDate::mapX; }
    int GetMapZsize() { return MAPDate::mapZ; }
private:
    Stage() {};
    ~Stage() {};
    Model* model = nullptr;
    Model* Putmodel = nullptr;
    DirectX::XMFLOAT3		position = { 0, -2, 0 };
    DirectX::XMFLOAT3		angle = { 0, 0, 0 };
    DirectX::XMFLOAT3		scale = { 2, 2, 2 };
    DirectX::XMFLOAT4X4		transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    MAPDate MapDate;
    int L = 0;
    int PutType = 0;
    DirectX::XMFLOAT3		PutPos = { 0, -1, 0 };
    DirectX::XMFLOAT3		PutAng = { 0, 0, 0 };
    DirectX::XMFLOAT3		Putscale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4		PutTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    const float Blocksize = 1;
    Model* Blockmodel1 = nullptr;
    Model* Blockmodel2 = nullptr;
    Model* Blockmodel3 = nullptr;
    Model* Blockmodel4 = nullptr;
    Model* Blockmodel5 = nullptr;
    Model* Blockmodel6 = nullptr;
};


