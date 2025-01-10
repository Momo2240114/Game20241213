#include "Stage.h"
#include "Collision.h"
#include "PlayerManager.h"

void Stage::init()
{
    //ステージモデルを読み込み
    /*model = new Model("Data/Model/Stage/ExampleStage.mdl");*/
    //model = new Model("Data/Model/Stage/stagekari2.mdl");
    model = new Model("Data/Model/Stage/StageGrassland.mdl");
    Blockmodel1 = new Model("Data/Model/Block/Block1.mdl");
    Blockmodel2 = new Model("Data/Model/Block/Block2.mdl");
    Blockmodel3 = new Model("Data/Model/Block/Block3.mdl");
    Blockmodel4 = new Model("Data/Model/Block/Block4.mdl");
    Blockmodel5 = new Model("Data/Model/Block/Block5.mdl");
    Blockmodel6 = new Model("Data/Model/Block/BlockStop.mdl");
    Blockmodel7 = new Model("Data/Model/Block/AccelBlock.mdl");
    StartPoint = new Model("Data/Model/Block/StatePoint.mdl");
    GoalPoint = new Model("Data/Model/Block/Goal.mdl");
    BlueBlock = new Model("Data/Model/Block/BlueBlock.mdl");
    BlueWaku = new Model("Data/Model/Block/BlueWaku.mdl");   
    RedBlock = new Model("Data/Model/Block/RedBlock.mdl");
    RedWaku = new Model("Data/Model/Block/RedWaku.mdl");
    Switch = new Model("Data/Model/Block/Switch.mdl");
    Warpmdl1 = new Model("Data/Model/Block/Warp/WarpPoint1.mdl");
    Warpmdl2 = new Model("Data/Model/Block/Warp/WarpPoint2.mdl");
    Warpmdl3 = new Model("Data/Model/Block/Warp/WarpPoint3.mdl");
    Warpmdl4 = new Model("Data/Model/Block/Warp/WarpPoint4.mdl");
    Warpmdl5 = new Model("Data/Model/Block/Warp/WarpPoint5.mdl");
    SpikeMdl = new Model("Data/Model/Block/SpikeBlockMdl.mdl");
    SpikeHit = new Model("Data/Model/Block/SpikeBlockHit.mdl");    
    SpikeFloorMdl = new Model("Data/Model/Block/SpikeBlockFloorMdl.mdl");
    SpikeFloorHit = new Model("Data/Model/Block/SpikeBlockFloorHit.mdl");
    // 初期化処理
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) 
            {
                MapDate.BlockID[PY][PX][PZ] = 0;
            }
        }
    }
    SetMapdate(0);
    UpdateBlockTransform();
}

bool Stage::RayCast(const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end, 
    DirectX::XMFLOAT3& hitPosition,
    DirectX::XMFLOAT3& hitNormal)
{
    return Collision::RayCast(start, end, transform, model, hitPosition, hitNormal);
}

bool Stage::UnifiedRayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
    DirectX::XMFLOAT3& hitPosition, 
    DirectX::XMFLOAT3& hitNormal, DirectX::XMFLOAT3& HitBlockAngle, int& hitBlock,
    bool isPlayer,bool checkBlocks)
{
    // ブロックチェックが必要な場合にBlockRayCastを実行
    if (checkBlocks) {
        for (int PY = MAPDate::mapY-1; PY >= 0 ; --PY) {
            for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
                for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                    // ブロックが存在しない場合はスキップ
                    if (MapDate.BlockID[PY][PX][PZ] == 0) {
                        continue;
                    }

                    // 各ブロックのトランスフォームを取得
                    DirectX::XMFLOAT4X4 blockTransform = MapDate.transform[PY][PX][PZ];

                    // 使用するブロックモデルを決定
                    Model* blockModel = nullptr;
                    switch (MapDate.BlockID[PY][PX][PZ]) {
                    case 1: blockModel = Blockmodel1; break;
                    case 2: blockModel = Blockmodel2; break;
                    case 3: blockModel = Blockmodel3; break;
                    case 4: blockModel = Blockmodel4; break;
                    case 5: blockModel = Blockmodel5; break;
                    case 6: blockModel = Blockmodel6; break;
                    case 7: blockModel = Blockmodel7; break;
                    case 101: blockModel = StartPoint; break;
                    case 105: blockModel = GoalPoint; break;
                    case 102:
                        if (OnBlockColer == OnBlockColer::Blue) {
                            blockModel = BlueBlock;
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case 103:
                        if (OnBlockColer == OnBlockColer::Red) {
                            blockModel = RedBlock;
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case 104:
                        blockModel = Switch;
                        break;
                    case 110:blockModel = Warpmdl1; break;
                    case 111:blockModel = Warpmdl2; break;
                    case 112:blockModel = Warpmdl3; break;
                    case 113:blockModel = Warpmdl4; break;
                    case 114:blockModel = Warpmdl5; break;
                    case 106:blockModel = SpikeHit; break;
                    case 107:blockModel = SpikeFloorHit; break;
                    }


                    // ブロックモデルが無効な場合はスキップ
                    if (!blockModel) {
                        continue;
                    }

                    // レイキャスト処理
                    if (Collision::RayCast(start, end, blockTransform, blockModel, hitPosition, hitNormal)) {
                        hitBlock = MapDate.BlockID[PY][PX][PZ];
                        HitBlockAngle = MapDate.angle[PY][PX][PZ];

                        return true; // 衝突があった場合は即座に true を返す
                    }
                }
            }
        }
    }

    // 最後にステージモデルとの衝突判定を行う
    if (Collision::RayCast(start, end, transform, model, hitPosition, hitNormal)) {
        hitBlock = 0;  // 衝突があった場合、ステージモデルにヒット
        return true;
    }

    // 衝突がなければ false を返す
    return false;
}

void Stage::MoveBlockUpdate(float elapsedTime)
{
    constexpr float MoveSpeed = 4.0f;   // 移動速度
    constexpr float MoveRange = 3.0f;   // 移動範囲
    constexpr float anglepattern = DirectX::XMConvertToRadians(90.0f);


    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                if (MapDate.BlockID[PY][PX][PZ] != 106) continue;

                // 移動処理
                DirectX::XMFLOAT3& pos = MapDate.position[PY][PX][PZ];
                DirectX::XMFLOAT3& orgPos = MapDate.orgposition[PY][PX][PZ];

                // 移動方向制御

           
                // 角度に応じた移動方向を変化させ、位置を補正する
                if (std::abs(MapDate.angle[PY][PX][PZ].y - 0.0f) < 0.01f) {
                    if (MapDate.moveForward[PY][PX][PZ]) {
                        pos.z += MoveSpeed * elapsedTime;
                        if (pos.z >= orgPos.z + MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = false; // 範囲に到達したら逆方向へ
                        }
                    }
                    else {
                        pos.z -= MoveSpeed * elapsedTime;
                        if (pos.z <= orgPos.z - MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = true; // 範囲に到達したら逆方向へ
                        }
                    }
                }
                else if (std::abs(MapDate.angle[PY][PX][PZ].y - anglepattern) < 0.01f) {
                    if (MapDate.moveForward[PY][PX][PZ]) {
                        pos.x += MoveSpeed * elapsedTime;
                        if (pos.x >= orgPos.x + MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = false; // 範囲に到達したら逆方向へ
                        }
                    }
                    else {
                        pos.x -= MoveSpeed * elapsedTime;
                        if (pos.x <= orgPos.x - MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = true; // 範囲に到達したら逆方向へ
                        }
                    }
                }
                else if (std::abs(MapDate.angle[PY][PX][PZ].y - anglepattern * 2) < 0.01f) {
                    if (MapDate.moveForward[PY][PX][PZ]) {
                        pos.z -= MoveSpeed * elapsedTime;
                        if (pos.z <= orgPos.z - MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = false; // 範囲に到達したら逆方向へ
                        }
                    }
                    else {
                        pos.z += MoveSpeed * elapsedTime;
                        if (pos.z >= orgPos.z + MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = true; // 範囲に到達したら逆方向へ
                        }
                    }
                }
                else if (std::abs(MapDate.angle[PY][PX][PZ].y - anglepattern * 3) < 0.01f) {
                    if (MapDate.moveForward[PY][PX][PZ]) {
                        pos.x -= MoveSpeed * elapsedTime;
                        if (pos.x <= orgPos.x - MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = false; // 範囲に到達したら逆方向へ
                        }
                    }
                    else {
                        pos.x += MoveSpeed * elapsedTime;
                        if (pos.x >= orgPos.x + MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = true; // 範囲に到達したら逆方向へ
                        }
                    }
                }
        
                DirectX::XMMATRIX BS = DirectX::XMMatrixScaling(
                    MapDate.scale.x * 2, MapDate.scale.y * 2, MapDate.scale.z * 2
                );
                DirectX::XMMATRIX BR = DirectX::XMMatrixRotationRollPitchYaw(
                    MapDate.angle[PY][PX][PZ].x,
                    MapDate.angle[PY][PX][PZ].y,
                    MapDate.angle[PY][PX][PZ].z
                );
                DirectX::XMMATRIX BT = DirectX::XMMatrixTranslation(
                    MapDate.position[PY][PX][PZ].x,
                    MapDate.position[PY][PX][PZ].y,
                    MapDate.position[PY][PX][PZ].z
                );
                DirectX::XMMATRIX BWorldTransform = BS * BR * BT;
                DirectX::XMStoreFloat4x4(&MapDate.transform[PY][PX][PZ], BWorldTransform);

            }
        }
    }
}
//プレイヤーの初期位置
DirectX::XMFLOAT3 Stage::GetStatePos()
{
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                if (MapDate.BlockID[PY][PX][PZ] == 101)
                {
                    return MapDate.position[PY][PX][PZ];
                }
            }
        }
    }
    return DirectX::XMFLOAT3{0,0,0};
}

void Stage::BlockChange()
{
    OnBlockColer++;
    if (OnBlockColer > 1)OnBlockColer = 0;
}

DirectX::XMFLOAT3 Stage::SearchPairPoint(int currentBlockID, const DirectX::XMFLOAT3& currentPosition)
{
    int currentPutx = static_cast<int>((currentPosition.x / MapDate.scale.x) / Blocksize);
    int currentPuty = static_cast<int>((currentPosition.y / MapDate.scale.y) / Blocksize);
    int currentPutz = static_cast<int>((currentPosition.z / MapDate.scale.z) / Blocksize);

    for (int y = 0; y < MAPDate::mapY; ++y) {
        for (int x = 0; x < MAPDate::mapX; ++x) {
            for (int z = 0; z < MAPDate::mapZ; ++z) {
                if (MapDate.BlockID[y][x][z] == currentBlockID) {
                    // 現在位置と異なるブロックを対象にする
                    if (x != currentPutx || y != currentPuty || z != currentPutz) {
                        // 新しい座標を計算して返す
                        DirectX::XMFLOAT3 targetPosition;
                        targetPosition.x = (x - MAPDate::mapX / 2) * Blocksize * MapDate.scale.x;
                        targetPosition.y = y * Blocksize * MapDate.scale.y;
                        targetPosition.z = (z - MAPDate::mapZ / 2) * Blocksize * MapDate.scale.z;
                        return targetPosition;
                    }
                }
            }
        }
    }

    // 対になるワープ床が見つからなかった場合は現在位置を返す
    return currentPosition;
}


Stage& Stage::Instance()
{
    static Stage instance;
    return instance;
}

void Stage::Finalize()
{
    //ステージモデルを破棄
    delete model;
    delete Blockmodel1;
    Blockmodel1 = nullptr;
    delete Blockmodel2;
    Blockmodel2 = nullptr;
    delete Blockmodel3;
    Blockmodel3 = nullptr;   
    delete Blockmodel4;
    Blockmodel4 = nullptr;   
    delete Blockmodel5;
    Blockmodel5 = nullptr;  
    delete Blockmodel6;
    Blockmodel6 = nullptr;     
    delete Blockmodel7;
    Blockmodel7 = nullptr;    
    delete   StartPoint;
    StartPoint = nullptr;   
    delete  GoalPoint;
    GoalPoint = nullptr;
    delete BlueBlock;
    BlueBlock = nullptr;
    delete BlueWaku;
    BlueWaku = nullptr;
    delete RedBlock;
    RedBlock = nullptr;
    delete RedWaku;
    RedWaku = nullptr;  
    delete Switch;
    Switch = nullptr;
    delete Warpmdl1;
    Warpmdl1 = nullptr;   
    delete Warpmdl2;
    Warpmdl2= nullptr;   
    delete Warpmdl3;
    Warpmdl3 = nullptr;   
    delete Warpmdl4;
    Warpmdl4 = nullptr;   
    delete Warpmdl5;
    Warpmdl5 = nullptr;
    delete SpikeMdl;
    SpikeMdl = nullptr;   
    delete SpikeHit;
    SpikeHit = nullptr;   
    delete SpikeFloorMdl;
    SpikeFloorMdl = nullptr;
    delete SpikeFloorHit;
    SpikeFloorHit = nullptr;
}

void Stage::putBlock(int Type, const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT3& Angle) 
{
    int Putx = static_cast<int>((Position.x / MapDate.scale.x) / Blocksize);
    int Puty = static_cast<int>((Position.y / MapDate.scale.y) / Blocksize);
    int Putz = static_cast<int>((Position.z / MapDate.scale.z) / Blocksize);

    if (MapDate.BlockID[Puty][Putx][Putz] > 100 ||
        MapDate.BlockID[Puty + 1][Putx][Putz] > 100 ){ return; }

    int SetType = Type;
    DirectX::XMFLOAT3 SetAngle = Angle;
    if ((MapDate.BlockID[Puty][Putx][Putz] == 3 && Type == 4) ||
        (MapDate.BlockID[Puty][Putx][Putz] == 4 && Type == 3)) {

        SetType = 5;
        if (Type == 3)
        {
            SetAngle = MapDate.angle[Puty][Putx][Putz];
        }
    }
    else if (MapDate.BlockID[Puty][Putx][Putz] != 0 && Type!= 0)return;

    // ブロックIDと回転角度を設定
    MapDate.BlockID[Puty][Putx][Putz] = SetType;
    MapDate.angle[Puty][Putx][Putz] = SetAngle;

    // ブロック位置を計算
    MapDate.position[Puty][Putx][Putz] = {
        (Putx) * Blocksize * MapDate.scale.x,
        (Puty)*Blocksize * MapDate.scale.y,
        (Putz) * Blocksize * MapDate.scale.z
    };
    MapDate.orgposition[Puty][Putx][Putz] = MapDate.position[Puty][Putx][Putz];
}

void Stage::Update(float elapsedTime)
{
    //MoveBlockUpdate(elapsedTime);
}

//真布チップでテスト用
void Stage::SetMapdate(int Level)
{
    L = Level;
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                if (Mapid[L][PY][PX][PZ] == 0) { continue; }

                int ID = 0;
                ID = Mapid[L][PY][PX][PZ];
                MapDate.BlockID[PY][PX][PZ] = ID + 100;//+100することで設置ブロックと分けやすくする

                //ブロックの位置計算
                MapDate.position[PY][PX][PZ] = {
                   (PX) * Blocksize * MapDate.scale.x,
                   (PY) * Blocksize * MapDate.scale.y,
                   (PZ) * Blocksize * MapDate.scale.z
                };
            }
        }
    }
}

void Stage::Render(const RenderContext& rc, ModelRenderer* renderer)
{
    renderer->Render(rc, transform, model, ShaderId::Lambert);
}

void Stage::BlockRender(const RenderContext& rc, ModelRenderer* renderer)
{
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                if (MapDate.BlockID[PY][PX][PZ] != 0)
                {
                    //if (MapDate.BlockModels[PY][PZ][PX])
                    //{
                    //    renderer->Render(rc, MapDate.transform[PY][PZ][PX], MapDate.BlockModels[PY][PZ][PX], ShaderId::Lambert);
                    //}
                    switch (MapDate.BlockID[PY][PX][PZ])
                    {
                    case 1:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Blockmodel1, ShaderId::Lambert);
                        break;   
                    case 2:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Blockmodel2, ShaderId::Lambert);
                        break;       
                    case 3:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Blockmodel3, ShaderId::Lambert);
                        break;  
                    case 4:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Blockmodel4, ShaderId::Lambert);
                        break;            
                    case 5:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Blockmodel5, ShaderId::Lambert);
                        break;               
                    case 6:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Blockmodel6, ShaderId::Lambert);
                        break; 
                    case 7:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Blockmodel7, ShaderId::Lambert);
                        break;
                    case 101:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], StartPoint, ShaderId::Lambert);
                        break;               
                    case 105:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ],GoalPoint, ShaderId::Lambert);
                        break;
                    case 102:
                        if (OnBlockColer == OnBlockColer::Blue) {
                            renderer->Render(rc, MapDate.transform[PY][PX][PZ], BlueBlock, ShaderId::Lambert);
                        }
                        else {
                            renderer->Render(rc, MapDate.transform[PY][PX][PZ], BlueWaku, ShaderId::Lambert);
                        }
                    break;
                    case 103:
                        if (OnBlockColer == OnBlockColer::Red) {
                            renderer->Render(rc, MapDate.transform[PY][PX][PZ], RedBlock, ShaderId::Lambert);
                        }
                        else {
                            renderer->Render(rc, MapDate.transform[PY][PX][PZ], RedWaku, ShaderId::Lambert);
                        }
                        break;
                    case 104:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Switch, ShaderId::Lambert);
                        break;
                    case 106:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], SpikeMdl, ShaderId::Lambert);
                        break;     
                    case 107:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], SpikeFloorMdl, ShaderId::Lambert);
                        break;
                    case 110:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Warpmdl1, ShaderId::Lambert);
                        break;                    
                    case 111:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Warpmdl2, ShaderId::Lambert);
                        break;                    
                    case 112:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Warpmdl3, ShaderId::Lambert);
                        break;                    
                    case 113:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Warpmdl4, ShaderId::Lambert);
                        break;                    
                    case 114:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], Warpmdl5, ShaderId::Lambert);
                        break;                                       
                    }
                }
            }
        }
    }
}
//
void Stage::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX WorldTransform = S * R * T;
    DirectX::XMStoreFloat4x4(&transform, WorldTransform);
}

void Stage::UpdateBlockTransform()
{
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                if (MapDate.BlockID[PY][PX][PZ] != 0)
                {
                    DirectX::XMMATRIX BS = DirectX::XMMatrixScaling(
                        MapDate.scale.x, MapDate.scale.y, MapDate.scale.z
                    );
                    DirectX::XMMATRIX BR = DirectX::XMMatrixRotationRollPitchYaw(
                        MapDate.angle[PY][PX][PZ].x,
                        MapDate.angle[PY][PX][PZ].y,
                        MapDate.angle[PY][PX][PZ].z
                    );
                    DirectX::XMMATRIX BT = DirectX::XMMatrixTranslation(
                        MapDate.position[PY][PX][PZ].x,
                        MapDate.position[PY][PX][PZ].y,
                        MapDate.position[PY][PX][PZ].z
                    );
                    DirectX::XMMATRIX BWorldTransform = BS * BR * BT;
                    DirectX::XMStoreFloat4x4(&MapDate.transform[PY][PX][PZ], BWorldTransform);
                }
            }
        }
    }
}
