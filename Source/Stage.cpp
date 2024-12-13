#include "Stage.h"
#include "Collision.h"

void Stage::init()
{
    //ステージモデルを読み込み
    /*model = new Model("Data/Model/Stage/ExampleStage.mdl");*/
    model = new Model("Data/Model/Stage/stagekari2.mdl");
    Blockmodel1 = new Model("Data/Model/Block/Block1.mdl");
    Blockmodel2 = new Model("Data/Model/Block/Block2.mdl");
    Blockmodel3 = new Model("Data/Model/Block/Block3.mdl");
    Blockmodel4 = new Model("Data/Model/Block/Block4.mdl");
    // 初期化処理
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) 
            {
                MapDate.BlockID[PY][PX][PZ] = 0;
            }
        }
    }
}

bool Stage::RayCast(const DirectX::XMFLOAT3& start,
    const DirectX::XMFLOAT3& end, 
    DirectX::XMFLOAT3& hitPosition,
    DirectX::XMFLOAT3& hitNormal)
{
    return Collision::RayCast(start, end, transform, model, hitPosition, hitNormal);
}

bool Stage::BlockRayCast(
    const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
    DirectX::XMFLOAT3& hitPosition, DirectX::XMFLOAT3& hitNormal,
    int& HitBlok)
{
    // ブロック全体をループ
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                // ブロックが存在しない場合はスキップ
                if (MapDate.BlockID[PY][PX][PZ] == 0)
                {
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
                }

                //// ブロックモデルが無効な場合はスキップ
                if (!blockModel) {
                    continue;
                }

                // レイキャスト処理
                if (Collision::RayCast(start, end, blockTransform, blockModel, hitPosition, hitNormal)) {
                    HitBlok = MapDate.BlockID[PY][PX][PZ];
                   
                    return true; // 衝突があった場合は即座に true を返す
                }
            }
        }
    }
}

bool Stage::UnifiedRayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
    DirectX::XMFLOAT3& hitPosition, 
    DirectX::XMFLOAT3& hitNormal, int& hitBlock, 
    bool checkBlocks)
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
                    }

                    // ブロックモデルが無効な場合はスキップ
                    if (!blockModel) {
                        continue;
                    }

                    // レイキャスト処理
                    if (Collision::RayCast(start, end, blockTransform, blockModel, hitPosition, hitNormal)) {
                        hitBlock = MapDate.BlockID[PY][PX][PZ];
                       
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
}

void Stage::putBlock(int Type, const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT3& Angle) 
{
    //int Putx = static_cast<int>(Position.x / (Blocksize * MapDate.scale.x) + MAPDate::mapX / 2);
    //int Puty = static_cast<int>(Position.y / (Blocksize * MapDate.scale.y));
    //int Putz = static_cast<int>(Position.z / (Blocksize * MapDate.scale.z) + MAPDate::mapZ / 2);

    //if (MapDate.BlockID[Puty][Putx][Putz] != 0)Puty += 1;

    //// ブロックIDと回転角度を設定
    //MapDate.BlockID[Puty][Putx][Putz] = Type;
    //MapDate.angle[Puty][Putx][Putz] = Angle;
    //    // ブロック位置を計算
    //    MapDate.position[Puty][Putx][Putz] = {
    //        (Putx) * Blocksize * MapDate.scale.x,
    //        (Puty) * Blocksize * MapDate.scale.y, // オフセット調整
    //        (Putz) * Blocksize * MapDate.scale.z
    //    };

    int Putx = static_cast<int>((Position.x / MapDate.scale.x) / Blocksize + MAPDate::mapX / 2);
    int Puty = static_cast<int>((Position.y / MapDate.scale.y) / Blocksize);
    int Putz = static_cast<int>((Position.z / MapDate.scale.z) / Blocksize + MAPDate::mapZ / 2);

    // ブロックIDと回転角度を設定
    MapDate.BlockID[Puty][Putx][Putz] = Type;
    MapDate.angle[Puty][Putx][Putz] = Angle;

    // ブロック位置を計算
    MapDate.position[Puty][Putx][Putz] = {
        (Putx - MAPDate::mapX / 2) * Blocksize * MapDate.scale.x,
        (Puty)*Blocksize * MapDate.scale.y,
        (Putz - MAPDate::mapZ / 2) * Blocksize * MapDate.scale.z
    };
}
void Stage::Update(float elapsedTime)
{
    //今は特にやることはない
    /*UpdateTransform();*/


}

void Stage::SetMapdate(int Level)
{
    L = Level;
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
               /* int ID = 0;*/
                /*ID = Mapid[L][PY][PZ][PX];*/
                 /*MapDate.BlockID[PY][PZ][PX] = ID;*/
                // //ブロックIDに対応するモデルを設定
                //switch (MapDate.BlockID[PY][PZ][PX])
                //{
                //case 1:
                //    MapDate.BlockModels[PY][PZ][PX] = new Model(*Blockmodel1); // 個別のインスタンスを作成
                //    break;
                //case 2:
                //    MapDate.BlockModels[PY][PZ][PX] = new Model(*Blockmodel2);
                //    break;
                //case 3:
                //    MapDate.BlockModels[PY][PZ][PX] = new Model(*Blockmodel3);
                //    break;
                //default:
                //    MapDate.BlockModels[PY][PZ][PX] = nullptr;
                //    break;
                //}

                // ブロックの位置計算
                if (MapDate.BlockID[PY][PX][PZ] != 0) {
                    MapDate.position[PY][PX][PZ] = {
                        (float)PX,
                        (float)PY,
                        (float)PZ
                    };
                }
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
//
//void Stage::UpdateBlockTransform()
//{
//    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
//        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
//            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
//                if (MapDate.BlockID[PY][PX][PZ] != 0)
//                {
//                    DirectX::XMMATRIX BS = DirectX::XMMatrixScaling(MapDate.scale.x, MapDate.scale.y, MapDate.scale.z);
//                    DirectX::XMMATRIX BR = DirectX::XMMatrixRotationRollPitchYaw(
//                        MapDate.angle[PY][PX][PZ].x,
//                        MapDate.angle[PY][PX][PZ].y,
//                        MapDate.angle[PY][PX][PZ].z
//                    );
//
//                        DirectX::XMMATRIX BT = DirectX::XMMatrixTranslation(
//                            MapDate.position[PY][PX][PZ].x - MAPDate::mapX / 2,
//                            MapDate.position[PY][PX][PZ].y,
//                            MapDate.position[PY][PX][PZ].z - MAPDate::mapZ / 2
//                        );
//                        if (MapDate.BlockID[PY][PX][PZ] == 3 || MapDate.BlockID[PY][PX][PZ] == 4)
//                        {
//                            BT = DirectX::XMMatrixTranslation(
//                                MapDate.position[PY][PX][PZ].x - MAPDate::mapX / 2,
//                                MapDate.position[PY][PX][PZ].y - 0.98f,
//                                MapDate.position[PY][PX][PZ].z - MAPDate::mapZ / 2);
//                        }
//                    DirectX::XMMATRIX BWorldTransform = BS * BR * BT;
//                    DirectX::XMStoreFloat4x4(&MapDate.transform[PY][PX][PZ], BWorldTransform);
//                }
//            }
//        }
//    }
//}

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
