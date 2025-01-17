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
    int  BlockEnergy[mapY][mapX][mapZ] = {};
    bool moveForward[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 position[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 orgposition[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 angle[mapY][mapX][mapZ] = {};
    DirectX::XMFLOAT3 scale = {
       0.2f,  0.2f,  0.2f};
    DirectX::XMFLOAT4X4 transform[mapY][mapX][mapZ] = {};
};
struct BlockInfo {
    DirectX::XMFLOAT3 Position;
    float Timer;
};
//�X�e�[�W
class Stage
{
public:
    static Stage& Instance();
    void Finalize();
    //�u���b�N�̐ݒu
    bool putBlock(int Type,const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT3& Angle,int cost);
    //�X�V����
    void Update(float elapsedTime);
    ////�u���b�N�̏����ݒu
    void SetMapdate(int Level);
    //�`�揈��
    void Render(const RenderContext& rc, ModelRenderer* renderer);   
    void BlockRender(const RenderContext& rc, ModelRenderer* renderer);   
    //3D�}�b�v�̍X�V
    void UpdateTransform();
    //3D�}�b�v�`�b�v�̍X�V
    void UpdateBlockTransform();
    //������
    void init();
    //���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal);
   /* bool BlockRayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal
        ,int& HitBlok);*/
    //�ŋ����C�L���X�g
    bool UnifiedRayCast(
        const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        DirectX::XMFLOAT3& hitPosition, DirectX::XMFLOAT3& hitNormal, DirectX::XMFLOAT3& HitBlockAngle,
        int& hitBlock, bool isPlayer, bool checkBlocks = false);

    //�u���b�N�֌W�̒l�̎擾
    float GetBlockSize() { return Blocksize; }
    DirectX::XMFLOAT3 GetBlockscale() { return MapDate.scale; }
    int GetMapXsize() { return MAPDate::mapX; }
    int GetMapZsize() { return MAPDate::mapZ; }

    //�����u���b�N�i����΁j�X�V
    void MoveBlockUpdate(float elapsedTime);

    DirectX::XMFLOAT3 GetStatePos();
    //�ԂƐ̃u���b�N�̐؂�ւ�
    void BlockChange();
    //�΂ɂȂ郏�[�v�u���b�N�̍��W�̎擾
    DirectX::XMFLOAT3 SearchPairPoint(int currentBlockID, const DirectX::XMFLOAT3& currentPosition);
    //���x����ݒ�
    void SetstageLevel(int SetLevel) { L = SetLevel; };
    int GetLevel() { return L; }
    //�R�X�g�̑���
    void AddCost(int Type);
    void SubCost(int DeleteType, DirectX::XMFLOAT3 Position);
    int GetUseCost() { return  UseCost; };
    int GetNowCost() { return NowCost; };
    //float GetLastPutTimer() { return CostTimer; };

    void DamageBlock(DirectX::XMFLOAT3 pos);//�ʒu�ɉ������u���b�N�̃G�l���M�[������������

    bool BlockEnergy(DirectX::XMFLOAT3 pos);//�ʒu�ɉ������u���b�N�̃G�l���M�[��0�����`�F�b�N
    int GetEnergy(DirectX::XMFLOAT3 pos);//�ʒu�ɉ������u���b�N�̎c�G�l���M�[���擾

    DirectX::XMFLOAT3 GetNextStartPos(); // ���ԂɎ擾����֐�
    void FindAllStartPositions();       // �����ʒu�����ׂĒT���֐�
private:
    Stage() {};
    ~Stage() {};
    Model* model = nullptr;
    Model* Putmodel = nullptr;

    DirectX::XMFLOAT3		position = {-1, -11,-1 };
    DirectX::XMFLOAT3		angle = { 0, 0, 0 };
    DirectX::XMFLOAT3		scale = { 0.2f,0.2f,0.2f };
    DirectX::XMFLOAT4X4		transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    MAPDate MapDate;//�v���C���[���u���u���b�N
    int L = 0;
    int PutType = 0;
    DirectX::XMFLOAT3		PutPos = { 0,0,0};
    DirectX::XMFLOAT3		PutAng = { 0, 0, 0 };
    DirectX::XMFLOAT3		Putscale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4		PutTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    const float Blocksize = 10;

    //�ݒu����u���b�N�̗���
    std::vector<BlockInfo> LastPutPositions;
    const int MaxStoredPositions = 10; // �ۑ��ł���ő吔
    void SavingPutPosUpdateTimers(float deltaTime);
    void SaveLastPutPos(DirectX::XMFLOAT3 Position);
    int UseCost = 0;
    int NowCost = 0;
    const int Max_Cost = 100;

    Model* Blockmodel1 = nullptr;//�����Ȃ�
    Model* Blockmodel2 = nullptr;//��
    Model* Blockmodel3 = nullptr;//�W�����v
    Model* Blockmodel4 = nullptr;//�����]��
    Model* Blockmodel5 = nullptr;//�����]���@�{�@�W�����v
    Model* Blockmodel6 = nullptr;//��~
    Model* Blockmodel7 = nullptr;//����

    //�Œ肷��u���b�N
    Model* StartPoint = nullptr;//�J�n�ʒu
    Model* GoalPoint =  nullptr;//�J�n�ʒu

    Model* BlueBlock = nullptr;//�u���[�u���b�N
    Model* BlueWaku = nullptr;//�u���[�g   
    Model* RedBlock = nullptr;//���b�h�u���b�N
    Model* RedWaku = nullptr;//���b�h�g
    Model* Switch = nullptr;//�X�C�b�`
    Model* OnSwitch = nullptr;//�I���X�C�b�`    
    Model* Lever = nullptr;//���o�[
    Model* OnLever = nullptr;//�I�����o�[
    int OnBlockColer = 0;//�F�u���b�N�̊Ǘ��p�F�������Ă������悤��int��

    Model* Warpmdl1 = nullptr;//���[�v1
    Model* Warpmdl2 = nullptr;//���[�v2
    Model* Warpmdl3 = nullptr;//���[�v3
    Model* Warpmdl4 = nullptr;//���[�v4
    Model* Warpmdl5 = nullptr;//���[�v5

    Model* BlockFixed = nullptr;//�X�p�C�N�u���b�N�����蔻��    
    Model* SpikeFloorMdl = nullptr;//�X�p�C�N���u���b�N
    Model* SpikeFloorHit = nullptr;//�X�p�C�N���u���b�N�����蔻��

    int timer = 0;

    std::vector<DirectX::XMFLOAT3> startPositions; // �����ʒu�̃��X�g
    size_t currentStartIndex = 0;                  // ���݂̏����ʒu�̃C���f�b�N�X
};


enum OnBlockColer
{
    Blue = 0,
    Red = 1
};

