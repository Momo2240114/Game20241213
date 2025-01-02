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
    DirectX::XMFLOAT3 scale = { 2.0f,  2.0f, 2.0f };
    DirectX::XMFLOAT4X4 transform[mapY][mapX][mapZ] = {};
};

//�X�e�[�W
class Stage
{
public:

    static Stage& Instance();

    void Finalize();
    //�u���b�N�̐ݒu
    void putBlock(int Type,const DirectX::XMFLOAT3& Position, const DirectX::XMFLOAT3& Angle);
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
    bool BlockRayCast(const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal
        ,int& HitBlok);
    //�ŋ����C�L���X�g
    bool UnifiedRayCast(
        const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        DirectX::XMFLOAT3& hitPosition, DirectX::XMFLOAT3& hitNormal,
        int& hitBlock, bool isPlayer, bool checkBlocks = false);

    float GetBlockSize() { return Blocksize; }
    DirectX::XMFLOAT3 GetBlockscale() { return MapDate.scale; }
    int GetMapXsize() { return MAPDate::mapX; }
    int GetMapZsize() { return MAPDate::mapZ; }

    void MoveBlockUpdate(float elapsedTime);

    DirectX::XMFLOAT3 GetStatePos();

    void BlockChange();
private:
    Stage() {};
    ~Stage() {};
    Model* model = nullptr;
    Model* Putmodel = nullptr;
    DirectX::XMFLOAT3		position = { 0, -2, 0 };
    DirectX::XMFLOAT3		angle = { 0, 0, 0 };
    DirectX::XMFLOAT3		scale = { 2, 2, 2 };
    DirectX::XMFLOAT4X4		transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    MAPDate MapDate;//�v���C���[���u���u���b�N
    MAPDate StageDate;//�X�e�[�W�ɌŒ肳���u���b�N
    int L = 0;
    int PutType = 0;
    DirectX::XMFLOAT3		PutPos = { 0, -1, 0 };
    DirectX::XMFLOAT3		PutAng = { 0, 0, 0 };
    DirectX::XMFLOAT3		Putscale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4		PutTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    const float Blocksize = 1;
    //�ݒu����u���b�N
    Model* Blockmodel1 = nullptr;//�����Ȃ�
    Model* Blockmodel2 = nullptr;//��
    Model* Blockmodel3 = nullptr;//�W�����v
    Model* Blockmodel4 = nullptr;//�����]��
    Model* Blockmodel5 = nullptr;//�����]���@�{�@�W�����v
    Model* Blockmodel6 = nullptr;//��~

    //�Œ肷��u���b�N
    Model* StatePoint = nullptr;//�J�n�ʒu

    Model* BlueBlock = nullptr;//�u���[�u���b�N
    Model* BlueWaku = nullptr;//�u���[�g   
    Model* RedBlock = nullptr;//���b�h�u���b�N
    Model* RedWaku = nullptr;//���b�h�g
    Model* Switch = nullptr;//�X�C�b�`
    int OnBlockColer = 0;

    int timer = 0;
};


enum OnBlockColer
{
    Blue = 0,
    Red = 1
};

