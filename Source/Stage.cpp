#include "Stage.h"
#include "Collision.h"
#include "PlayerManager.h"

void Stage::init()
{
    //�X�e�[�W���f����ǂݍ���
    /*model = new Model("Data/Model/Stage/ExampleStage.mdl");*/
    model = new Model("Data/Model/Stage/stagekari2.mdl");
    Blockmodel1 = new Model("Data/Model/Block/Block1.mdl");
    Blockmodel2 = new Model("Data/Model/Block/Block2.mdl");
    Blockmodel3 = new Model("Data/Model/Block/Block3.mdl");
    Blockmodel4 = new Model("Data/Model/Block/Block4.mdl");
    Blockmodel5 = new Model("Data/Model/Block/Block5.mdl");
    Blockmodel6 = new Model("Data/Model/Block/BlockStop.mdl");
    StatePoint = new Model("Data/Model/Block/StatePoint.mdl");
    BlueBlock = new Model("Data/Model/Block/BlueBlock.mdl");
    BlueWaku = new Model("Data/Model/Block/BlueWaku.mdl");   
    RedBlock = new Model("Data/Model/Block/RedBlock.mdl");
    RedWaku = new Model("Data/Model/Block/RedWaku.mdl");
    Switch = new Model("Data/Model/Block/Switch.mdl");
    // ����������
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

bool Stage::BlockRayCast(
    const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
    DirectX::XMFLOAT3& hitPosition, DirectX::XMFLOAT3& hitNormal,
    int& HitBlok)
{
    // �u���b�N�S�̂����[�v
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                // �u���b�N�����݂��Ȃ��ꍇ�̓X�L�b�v
                if (MapDate.BlockID[PY][PX][PZ] == 0)
                {
                    continue;
                }

                // �e�u���b�N�̃g�����X�t�H�[�����擾
                DirectX::XMFLOAT4X4 blockTransform = MapDate.transform[PY][PX][PZ];

                // �g�p����u���b�N���f��������
                Model* blockModel = nullptr;
                switch (MapDate.BlockID[PY][PX][PZ]) {
                case 1: blockModel = Blockmodel1; break;
                case 2: blockModel = Blockmodel2; break;
                case 3: blockModel = Blockmodel3; break;
                case 4: blockModel = Blockmodel4; break;
                case 5: blockModel = Blockmodel5; break;
                case 6: blockModel = Blockmodel6; break;
                case 101: blockModel = StatePoint; break;
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
                }

                //// �u���b�N���f���������ȏꍇ�̓X�L�b�v
                if (!blockModel) {
                    continue;
                }

                // ���C�L���X�g����
                if (Collision::RayCast(start, end, blockTransform, blockModel, hitPosition, hitNormal)) {
                    HitBlok = MapDate.BlockID[PY][PX][PZ];
                   
                    return true; // �Փ˂��������ꍇ�͑����� true ��Ԃ�
                }
            }
        }
    }
}

bool Stage::UnifiedRayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
    DirectX::XMFLOAT3& hitPosition, 
    DirectX::XMFLOAT3& hitNormal, DirectX::XMFLOAT3& HitBlockAngle, int& hitBlock,
    bool isPlayer,bool checkBlocks)
{
    // �u���b�N�`�F�b�N���K�v�ȏꍇ��BlockRayCast�����s
    if (checkBlocks) {
        for (int PY = MAPDate::mapY-1; PY >= 0 ; --PY) {
            for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
                for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                    // �u���b�N�����݂��Ȃ��ꍇ�̓X�L�b�v
                    if (MapDate.BlockID[PY][PX][PZ] == 0) {
                        continue;
                    }

                    // �e�u���b�N�̃g�����X�t�H�[�����擾
                    DirectX::XMFLOAT4X4 blockTransform = MapDate.transform[PY][PX][PZ];

                    // �g�p����u���b�N���f��������
                    Model* blockModel = nullptr;
                    switch (MapDate.BlockID[PY][PX][PZ]) {
                    case 1: blockModel = Blockmodel1; break;
                    case 2: blockModel = Blockmodel2; break;
                    case 3: blockModel = Blockmodel3; break;
                    case 4: blockModel = Blockmodel4; break;
                    case 5: blockModel = Blockmodel5; break;
                    case 6: blockModel = Blockmodel6; break;
                    case 101: blockModel = StatePoint; break;
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
                    }


                    // �u���b�N���f���������ȏꍇ�̓X�L�b�v
                    if (!blockModel) {
                        continue;
                    }

                    // ���C�L���X�g����
                    if (Collision::RayCast(start, end, blockTransform, blockModel, hitPosition, hitNormal)) {
                        hitBlock = MapDate.BlockID[PY][PX][PZ];
                        HitBlockAngle = MapDate.angle[PY][PX][PZ];
                        return true; // �Փ˂��������ꍇ�͑����� true ��Ԃ�
                    }
                }
            }
        }
    }

    // �Ō�ɃX�e�[�W���f���Ƃ̏Փ˔�����s��
    if (Collision::RayCast(start, end, transform, model, hitPosition, hitNormal)) {
        hitBlock = 0;  // �Փ˂��������ꍇ�A�X�e�[�W���f���Ƀq�b�g
        return true;
    }

    // �Փ˂��Ȃ���� false ��Ԃ�
    return false;
}

void Stage::MoveBlockUpdate(float elapsedTime)
{
    constexpr float MoveSpeed = 2.0f;   // �ړ����x
    constexpr float MoveRange = 6.0f;   // �ړ��͈�
    constexpr float anglepattern = DirectX::XMConvertToRadians(90.0f);


    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                if (MapDate.BlockID[PY][PX][PZ] != 7) continue;

                // �ړ�����
                DirectX::XMFLOAT3& pos = MapDate.position[PY][PX][PZ];
                DirectX::XMFLOAT3& orgPos = MapDate.orgposition[PY][PX][PZ];

                // �ړ���������

           
                // �p�x�ɉ������ړ�������ω������A�ʒu��␳����
                if (std::abs(MapDate.angle[PY][PX][PZ].y - 0.0f) < 0.01f) {
                    if (MapDate.moveForward[PY][PX][PZ]) {
                        pos.z += MoveSpeed * elapsedTime;
                        if (pos.z >= orgPos.z + MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = false; // �͈͂ɓ��B������t������
                        }
                    }
                    else {
                        pos.z -= MoveSpeed * elapsedTime;
                        if (pos.z <= orgPos.z - MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = true; // �͈͂ɓ��B������t������
                        }
                    }
                }
                else if (std::abs(MapDate.angle[PY][PX][PZ].y - anglepattern) < 0.01f) {
                    if (MapDate.moveForward[PY][PX][PZ]) {
                        pos.x += MoveSpeed * elapsedTime;
                        if (pos.x >= orgPos.x + MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = false; // �͈͂ɓ��B������t������
                        }
                    }
                    else {
                        pos.x -= MoveSpeed * elapsedTime;
                        if (pos.x <= orgPos.x - MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = true; // �͈͂ɓ��B������t������
                        }
                    }
                }
                else if (std::abs(MapDate.angle[PY][PX][PZ].y - anglepattern * 2) < 0.01f) {
                    if (MapDate.moveForward[PY][PX][PZ]) {
                        pos.z -= MoveSpeed * elapsedTime;
                        if (pos.z <= orgPos.z - MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = false; // �͈͂ɓ��B������t������
                        }
                    }
                    else {
                        pos.z += MoveSpeed * elapsedTime;
                        if (pos.z >= orgPos.z + MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = true; // �͈͂ɓ��B������t������
                        }
                    }
                }
                else if (std::abs(MapDate.angle[PY][PX][PZ].y - anglepattern * 3) < 0.01f) {
                    if (MapDate.moveForward[PY][PX][PZ]) {
                        pos.x -= MoveSpeed * elapsedTime;
                        if (pos.x <= orgPos.x - MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = false; // �͈͂ɓ��B������t������
                        }
                    }
                    else {
                        pos.x += MoveSpeed * elapsedTime;
                        if (pos.x >= orgPos.x + MoveRange) {
                            MapDate.moveForward[PY][PX][PZ] = true; // �͈͂ɓ��B������t������
                        }
                    }
                }
        
                // ���W���X�V
                DirectX::XMStoreFloat4x4(
                    &MapDate.transform[PY][PX][PZ],
                    DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)
                );
            }
        }
    }
}
//�v���C���[�̏����ʒu
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

Stage& Stage::Instance()
{
    static Stage instance;
    return instance;
}

void Stage::Finalize()
{
    //�X�e�[�W���f����j��
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
    delete   StatePoint;
    StatePoint = nullptr;
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
}

void Stage::putBlock(int Type, const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT3& Angle) 
{
  
    int Putx = static_cast<int>((Position.x / MapDate.scale.x) / Blocksize + MAPDate::mapX / 2);
    int Puty = static_cast<int>((Position.y / MapDate.scale.y) / Blocksize);
    int Putz = static_cast<int>((Position.z / MapDate.scale.z) / Blocksize + MAPDate::mapZ / 2);

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

    // �u���b�NID�Ɖ�]�p�x��ݒ�
    MapDate.BlockID[Puty][Putx][Putz] = SetType;
    MapDate.angle[Puty][Putx][Putz] = SetAngle;

    // �u���b�N�ʒu���v�Z
    MapDate.position[Puty][Putx][Putz] = {
        (Putx - MAPDate::mapX / 2) * Blocksize * MapDate.scale.x,
        (Puty)*Blocksize * MapDate.scale.y,
        (Putz - MAPDate::mapZ / 2) * Blocksize * MapDate.scale.z
    };
    MapDate.orgposition[Puty][Putx][Putz] = MapDate.position[Puty][Putx][Putz];
}
void Stage::Update(float elapsedTime)
{
    //���͓��ɂ�邱�Ƃ͂Ȃ�
    /*UpdateTransform();*/
    MoveBlockUpdate(elapsedTime);

    //timer++;
    //int Cool = timer % 360;
    //if (Cool == 0)
    //{
    //    BlockChange();
    //}
}

//�^�z�`�b�v�Ńe�X�g�p
void Stage::SetMapdate(int Level)
{
    L = Level;
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                if (Mapid[L][PY][PX][PZ] == 0) { continue; }

                int ID = 0;
                ID = Mapid[L][PY][PX][PZ];
                MapDate.BlockID[PY][PX][PZ] = ID + 100;//+100���邱�ƂŐݒu�u���b�N�ƕ����₷������

                //�u���b�N�̈ʒu�v�Z
                MapDate.position[PY][PX][PZ] = {
                   (PX - (MAPDate::mapX / 2)) * Blocksize * MapDate.scale.x,
                   (PY)*Blocksize * MapDate.scale.y,
                   (PZ - (MAPDate::mapZ / 2)) * Blocksize * MapDate.scale.z
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
                    case 101:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], StatePoint, ShaderId::Lambert);
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
