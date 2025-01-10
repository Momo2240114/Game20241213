#pragma once


#include "System/ModelRenderer.h"
#include "MapDate.h"
#include <vector>

struct  MAPDate
{
    static const int mapX = 40;
    static const int mapZ = 40;
    static const int mapY = 10;
    int  BlockID[mapY][mapX][mapZ] = {};
    bool moveForward[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 position[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 orgposition[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 angle[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 scale = {1.0f,  1.0f, 1.0f };
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
    ////ブロックの初期設置
    void SetMapdate(int Level);
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
   /* bool BlockRayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal
        ,int& HitBlok);*/
    //最強レイキャスト
    bool UnifiedRayCast(
        const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        DirectX::XMFLOAT3& hitPosition, DirectX::XMFLOAT3& hitNormal, DirectX::XMFLOAT3& HitBlockAngle,
        int& hitBlock, bool isPlayer, bool checkBlocks = false);

    float GetBlockSize() { return Blocksize; }
    DirectX::XMFLOAT3 GetBlockscale() { return MapDate.scale; }
    int GetMapXsize() { return MAPDate::mapX; }
    int GetMapZsize() { return MAPDate::mapZ; }

    void MoveBlockUpdate(float elapsedTime);

    DirectX::XMFLOAT3 GetStatePos();

    void BlockChange();

    DirectX::XMFLOAT3 SearchPairPoint(int currentBlockID, const DirectX::XMFLOAT3& currentPosition);
private:
    Stage() {};
    ~Stage() {};
    Model* model = nullptr;
    Model* Putmodel = nullptr;
    DirectX::XMFLOAT3		position = {-0.5, -20, -0.5 };
    DirectX::XMFLOAT3		angle = { 0, 0, 0 };
    DirectX::XMFLOAT3		scale = { 0.1f,0.1f,0.1f };
    DirectX::XMFLOAT4X4		transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    MAPDate MapDate;//プレイヤーが置くブロック
    int L = 0;
    int PutType = 0;
    DirectX::XMFLOAT3		PutPos = { 0, -1, 0 };
    DirectX::XMFLOAT3		PutAng = { 0, 0, 0 };
    DirectX::XMFLOAT3		Putscale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4		PutTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    const float Blocksize = 1;
    //設置するブロック
    Model* Blockmodel1 = nullptr;//何もなし
    Model* Blockmodel2 = nullptr;//坂
    Model* Blockmodel3 = nullptr;//ジャンプ
    Model* Blockmodel4 = nullptr;//方向転換
    Model* Blockmodel5 = nullptr;//方向転換　＋　ジャンプ
    Model* Blockmodel6 = nullptr;//停止
    Model* Blockmodel7 = nullptr;//加速

    //固定するブロック
    Model* StartPoint = nullptr;//開始位置
    Model* GoalPoint =  nullptr;//開始位置

    Model* BlueBlock = nullptr;//ブルーブロック
    Model* BlueWaku = nullptr;//ブルー枠   
    Model* RedBlock = nullptr;//レッドブロック
    Model* RedWaku = nullptr;//レッド枠
    Model* Switch = nullptr;//スイッチ
    int OnBlockColer = 0;//色ブロックの管理用色が増えてもいいようにintで

    Model* Warpmdl1 = nullptr;//ワープ1
    Model* Warpmdl2 = nullptr;//ワープ2
    Model* Warpmdl3 = nullptr;//ワープ3
    Model* Warpmdl4 = nullptr;//ワープ4
    Model* Warpmdl5 = nullptr;//ワープ5

    Model* SpikeMdl = nullptr;//スパイクブロック
    Model* SpikeHit = nullptr;//スパイクブロック当たり判定    
    Model* SpikeFloorMdl = nullptr;//スパイク床ブロック
    Model* SpikeFloorHit = nullptr;//スパイク床ブロック当たり判定

    int timer = 0;

};


enum OnBlockColer
{
    Blue = 0,
    Red = 1
};

