#include "Stage.h"
#include "Collision.h"
#include "PlayerManager.h"
#include "PutBlock.h"

void Stage::init()
{
    //�X�e�[�W���f����ǂݍ���
    /*model = new Model("Data/Model/Stage/ExampleStage.mdl");*/
    //model = new Model("Data/Model/Stage/stagekari2.mdl");
    switch (L)
    {
    case 0:
        model = new Model("Data/Model/Stage/StageGrassland.mdl");
        break;
    case 1:
        model = new Model("Data/Model/Stage/StageGrassland.mdl");
        break;
    }
 
    Blockmodel1 = new Model("Data/Model/Block/field_dirt_asset.mdl");
    Blockmodel2 = new Model("Data/Model/Block/gimmick_stairs_asset.mdl");
    Blockmodel3 = new Model("Data/Model/Block/gimmick_jump_asset.mdl");
    Blockmodel4 = new Model("Data/Model/Block/gimmick_direction_asset.mdl");
    Blockmodel5 = new Model("Data/Model/Block/Block5.mdl");
    Blockmodel6 = new Model("Data/Model/Block/gimmick_stop_asset.mdl");
    Blockmodel7 = new Model("Data/Model/Block/gimmick_axel_asset.mdl");
    StartPoint = new Model("Data/Model/Block/StartPoint.mdl");
    GoalPoint = new Model("Data/Model/Block/Goal.mdl");
    BlueBlock = new Model("Data/Model/Block/gimmick_lever_block_blue_ground_asset.mdl");
    BlueWaku = new Model("Data/Model/Block/BlueWaku.mdl");   
    RedBlock = new Model("Data/Model/Block/gimmick_lever_block_Red_ground_asset.mdl");
    RedWaku = new Model("Data/Model/Block/RedWaku.mdl"); 
    Switch = new Model("Data/Model/Block/gimmick_lever_block_blue_asset.mdl");
    OnSwitch = new Model("Data/Model/Block/gimmick_lever_block_red_asset.mdl");
    Lever = new Model("Data/Model/Block/gimmick_lever_switching_asset.mdl");//���o�[
    OnLever = new Model("Data/Model/Block/gimmick_lever_asset.mdl");//�I�����o�[
    Warpmdl1 = new Model("Data/Model/Block/Warp/WarpPoint1.mdl");
    Warpmdl2 = new Model("Data/Model/Block/Warp/WarpPoint2.mdl");
    Warpmdl3 = new Model("Data/Model/Block/Warp/WarpPoint3.mdl");
    Warpmdl4 = new Model("Data/Model/Block/Warp/WarpPoint4.mdl");
    Warpmdl5 = new Model("Data/Model/Block/Warp/WarpPoint5.mdl");
    BlockFixed = new Model("Data/Model/Block/field_glass_asset.mdl");
    SpikeFloorMdl = new Model("Data/Model/Block/gimmick_thorn_asset.mdl");
    SpikeFloorHit = new Model("Data/Model/Block/SpikeBlockHit.mdl");
    // ����������
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) 
            {
                MapDate.BlockID[PY][PX][PZ] = 0;
                MapDate.BlockEnergy[PY][PX][PZ] = 0;
            }
        }
    }
    SetMapdate(L);
    UpdateBlockTransform();
    UpdateTransform();

    UseCost = 0;
    NowCost = 0;

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
                        if (OnBlockColer == OnBlockColer::Blue) {
                            blockModel = Switch;
                        }
                        else {
                            blockModel = OnSwitch;
                        }
                        break;
                    case 110:blockModel = Warpmdl1; break;
                    case 111:blockModel = Warpmdl2; break;
                    case 112:blockModel = Warpmdl3; break;
                    case 113:blockModel = Warpmdl4; break;
                    case 114:blockModel = Warpmdl5; break;
                    case 106:blockModel = SpikeFloorHit; break;
                    case 107:blockModel = BlockFixed; break;
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
    constexpr float MoveSpeed = 4.0f;   // �ړ����x
    constexpr float MoveRange = 3.0f;   // �ړ��͈�
    constexpr float anglepattern = DirectX::XMConvertToRadians(90.0f);


    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                if (MapDate.BlockID[PY][PX][PZ] != 106) continue;

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
//�v���C���[�̏����ʒu
DirectX::XMFLOAT3 Stage::GetStatePos()
{ 
    return GetNextStartPos();
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
                    // ���݈ʒu�ƈقȂ�u���b�N��Ώۂɂ���
                    if (x != currentPutx || y != currentPuty || z != currentPutz) {
                        // �V�������W���v�Z���ĕԂ�
                        DirectX::XMFLOAT3 targetPosition;
                        targetPosition.x = (x ) * Blocksize * MapDate.scale.x;
                        targetPosition.y = (y - 0.5f) * Blocksize * MapDate.scale.y;
                        targetPosition.z = (z) * Blocksize * MapDate.scale.z;
                        return targetPosition;
                    }
                }
            }
        }
    }

    // �΂ɂȂ郏�[�v����������Ȃ������ꍇ�͌��݈ʒu��Ԃ�
    return currentPosition;
}

void Stage::AddCost(int Type)
{
    NowCost += PutBlock::Instance().CostGet(Type);
    UseCost += PutBlock::Instance().CostGet(Type);
}

void Stage::SubCost(int DeleteType, DirectX::XMFLOAT3 Position)
{
    bool found = false;

    for (auto& block : LastPutPositions) {
        if (block.Position.x == Position.x && block.Position.y == Position.y && block.Position.z == Position.z) {
            if (block.Timer > 0) {
                UseCost -= PutBlock::Instance().CostGet(DeleteType);
                found = true;
                break;
            }
        }
    }

    if (!found) {
        if (LastPutPositions.size() >= MaxStoredPositions) {
            LastPutPositions.erase(LastPutPositions.begin()); // �ŌÂ̗v�f���폜
        }
        LastPutPositions.push_back({ Position, 10.0f }); // �V�����u���b�N��ǉ�
    }

    NowCost -= PutBlock::Instance().CostGet(DeleteType);
}

void Stage::DamageBlock(DirectX::XMFLOAT3 pos)
{
    int Px = static_cast<int>((pos.x / MapDate.scale.x) / Blocksize);
    int Py = static_cast<int>((pos.y / MapDate.scale.y) / Blocksize);
    int Pz = static_cast<int>((pos.z / MapDate.scale.z) / Blocksize);

    MapDate.BlockEnergy[Py][Px][Pz]--;
    if (MapDate.BlockEnergy[Py][Px][Pz] <= 0)
    {
        MapDate.BlockEnergy[Py][Px][Pz] = 0;
    }
}

bool Stage::BlockEnergy(DirectX::XMFLOAT3 pos)
{
    int Px = static_cast<int>((pos.x / MapDate.scale.x) / Blocksize);
    int Py = static_cast<int>((pos.y / MapDate.scale.y) / Blocksize);
    int Pz = static_cast<int>((pos.z / MapDate.scale.z) / Blocksize);

    return(MapDate.BlockEnergy[Py][Px][Pz] > 0);
}

int Stage::GetEnergy(DirectX::XMFLOAT3 pos)
{
    int Px = static_cast<int>((pos.x / MapDate.scale.x) / Blocksize);
    int Py = static_cast<int>((pos.y / MapDate.scale.y) / Blocksize);
    int Pz = static_cast<int>((pos.z / MapDate.scale.z) / Blocksize);

    return MapDate.BlockEnergy[Py][Px][Pz];
}

DirectX::XMFLOAT3 Stage::GetNextStartPos(){
    if (startPositions.empty()) {
        return DirectX::XMFLOAT3{ 0, 0, 0 }; // �����ʒu�����݂��Ȃ��ꍇ
    }

    // ���݂̈ʒu���擾
    DirectX::XMFLOAT3 nextPos = startPositions[currentStartIndex];

    // �C���f�b�N�X���X�V
    currentStartIndex = (currentStartIndex + 1) % startPositions.size();

    return nextPos;
}

void Stage::FindAllStartPositions()
{
    startPositions.clear(); // ������
    currentStartIndex = 0;  // �C���f�b�N�X�����Z�b�g

    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PX = 0; PX < MAPDate::mapX; ++PX) {
            for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {               
                if (MapDate.BlockID[PY][PX][PZ] == 101) {
                    DirectX::XMFLOAT3 pos = MapDate.position[PY][PX][PZ];
                    pos.y += 1;
                    startPositions.push_back(pos); // ���X�g�ɒǉ�
                }
            }
        }
    }
}

void Stage::SavingPutPosUpdateTimers(float deltaTime)
{
    for (auto& block : LastPutPositions) {
        block.Timer -= deltaTime;
    }
    // �^�C�}�[��0�ȉ��̃u���b�N���폜
    LastPutPositions.erase(
        std::remove_if(LastPutPositions.begin(), LastPutPositions.end(), [](const BlockInfo& block) {
            return block.Timer <= 0;
            }),
        LastPutPositions.end()
    );
}

void Stage::SaveLastPutPos(DirectX::XMFLOAT3 Position)
{
    // �����ʒu�����ɑ��݂��邩�`�F�b�N
    auto it = std::find_if(LastPutPositions.begin(), LastPutPositions.end(), [&](const BlockInfo& block) {
        return block.Position.x == Position.x && block.Position.y == Position.y && block.Position.z == Position.z;
        });

    if (it != LastPutPositions.end()) {
        // ���ɑ��݂���ꍇ�̓^�C�}�[�����Z�b�g
        it->Timer = 10.0f;
    }
    else {
        // �V�����ʒu��ǉ�
        if (LastPutPositions.size() >= MaxStoredPositions) {
            LastPutPositions.erase(LastPutPositions.begin()); // �ŌÂ̗v�f���폜
        }
        LastPutPositions.push_back({ Position, 10.0f });
    }
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
    delete OnSwitch;
    OnSwitch = nullptr;
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
    delete BlockFixed;
    BlockFixed = nullptr;     
    delete SpikeFloorMdl;
    SpikeFloorMdl = nullptr;
    delete SpikeFloorHit;
    SpikeFloorHit = nullptr;
}

bool Stage::putBlock(int Type, const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT3& Angle, int cost)
{
    if (NowCost + PutBlock::Instance().CostGet(Type) > Max_Cost)return false;

    int Putx = static_cast<int>((Position.x / MapDate.scale.x) / Blocksize);
    int Puty = static_cast<int>((Position.y / MapDate.scale.y) / Blocksize);
    int Putz = static_cast<int>((Position.z / MapDate.scale.z) / Blocksize);

    if (MapDate.BlockID[Puty][Putx][Putz] > 100 ||
        MapDate.BlockID[Puty + 1][Putx][Putz] > 100 ){ return false; }

    int SetType = Type;
    DirectX::XMFLOAT3 SetAngle = Angle;
    //if ((MapDate.BlockID[Puty][Putx][Putz] == 3 && Type == 4) ||
    //    (MapDate.BlockID[Puty][Putx][Putz] == 4 && Type == 3)) {

    //    SetType = 5;
    //    if (Type == 3)
    //    {
    //        SetAngle = MapDate.angle[Puty][Putx][Putz];
    //    }
    //}
    if (MapDate.BlockID[Puty][Putx][Putz] != 0 && Type!= 0)return false;

    if(Type != 0)
    {
        AddCost(Type);
    }
    else
    {
        SubCost(MapDate.BlockID[Puty][Putx][Putz], MapDate.position[Puty][Putx][Putz]);
    }

    // �u���b�NID�Ɖ�]�p�x��ݒ�
    MapDate.BlockID[Puty][Putx][Putz] = SetType;
    MapDate.angle[Puty][Putx][Putz] = SetAngle;

    // �u���b�N�ʒu���v�Z
    MapDate.position[Puty][Putx][Putz] = {
        (Putx)*Blocksize* MapDate.scale.x,
        (Puty)*Blocksize * MapDate.scale.y,
        (Putz)*Blocksize * MapDate.scale.z
    };

    MapDate.orgposition[Puty][Putx][Putz] = MapDate.position[Puty][Putx][Putz];//�u���b�N�̏����l�n�_��ݒ�
    MapDate.BlockEnergy[Puty][Putx][Putz] = 3;//�u���b�N�̃G�l���M�[��ݒ�
    SaveLastPutPos(MapDate.position[Puty][Putx][Putz]);

    return true;
}

void Stage::Update(float elapsedTime)
{
    //MoveBlockUpdate(elapsedTime);
    SavingPutPosUpdateTimers(elapsedTime);
}

//�^�z�`�b�v�Ńe�X�g�p
void Stage::SetMapdate(int Level)
{
    for (int PY = 0; PY < MAPDate::mapY; ++PY) {
        for (int PZ = 0; PZ < MAPDate::mapZ; ++PZ) {
            for (int PX = 0; PX < MAPDate::mapX; ++PX) {
                if (Mapid[L][PY][PX][PZ] == 0) { continue; }
                L;
                int ID = 0;
                ID = Mapid[L][PY][PX][PZ];
                MapDate.BlockID[PY][PX][PZ] = ID + 100;//+100���邱�ƂŐݒu�u���b�N�ƕ����₷������
                MapDate.BlockEnergy[PY][PX][PZ] = 3;
    
                //�u���b�N�̈ʒu�v�Z
                MapDate.position[PY][PX][PZ] = {
                   (PX) * Blocksize * MapDate.scale.x,
                   (PY) * Blocksize * MapDate.scale.y,
                   (PZ) * Blocksize * MapDate.scale.z
                };
            }
        }
    }
    FindAllStartPositions();
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

                        if (MapDate.BlockID[PY][PX][PZ] == 104)
                        {
                            BT = DirectX::XMMatrixTranslation(
                                MapDate.position[PY][PX][PZ].x,
                                MapDate.position[PY][PX][PZ].y - 2,
                                MapDate.position[PY][PX][PZ].z + 2
                            );
                        }
                        DirectX::XMMATRIX BWorldTransform = BS * BR * BT;

                        DirectX::XMFLOAT4X4 SwichPos;
                        DirectX::XMStoreFloat4x4(&SwichPos, BWorldTransform);
                        
                        if (OnBlockColer == OnBlockColer::Blue) {
                            renderer->Render(rc, MapDate.transform[PY][PX][PZ], Switch, ShaderId::Lambert);
                            renderer->Render(rc, SwichPos, Lever, ShaderId::Lambert);

                        }
                        else {
                            renderer->Render(rc, MapDate.transform[PY][PX][PZ], OnSwitch, ShaderId::Lambert);
                            renderer->Render(rc, SwichPos, OnLever, ShaderId::Lambert);
                        }
                        break;
                    case 106:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], SpikeFloorMdl, ShaderId::Lambert);
                        break;     
                    case 107:
                        renderer->Render(rc, MapDate.transform[PY][PX][PZ], BlockFixed, ShaderId::Lambert);
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
                    int A = 0;
                    if (MapDate.BlockID[PY][PX][PZ] == 1)
                    {
                        A = 1;
                    }
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

                    if (MapDate.BlockID[PY][PX][PZ] == 104)
                    {
                        BT = DirectX::XMMatrixTranslation(
                            MapDate.position[PY][PX][PZ].x,
                            MapDate.position[PY][PX][PZ].y - 2,
                            MapDate.position[PY][PX][PZ].z
                        );
                    }

                    DirectX::XMMATRIX BWorldTransform = BS * BR * BT;
                    DirectX::XMStoreFloat4x4(&MapDate.transform[PY][PX][PZ], BWorldTransform);
                }
            }
        }
    }
}
