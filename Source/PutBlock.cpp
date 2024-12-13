#include "PutBlock.h"
#include "System/Input.h"


PutBlock& PutBlock::Instance()
{
	static PutBlock Instance;
	return Instance;
}

//�O�̂��

//{
	////
	////void PutBlock::putBlock()
	////{
	////	performRaycast(RayState, RayEnd);
	////
	////		�X�e�[�W�Ƃ̃��C�L���X�g���s���z�u���W�����߂�
	////		HitPos, HitNor;
	////
	////		if (Stage::Instance().RayCast(
	////			RayState, RayEnd,
	////			HitPos, HitNor)) {
	////			UpdateTransform();
	////		}
	////		if (Input::Instance().GetMouse().GetButtonDown() & Mouse::BTN_LEFT)
	////		{
	////			Stage::Instance().putBlock(Type, HitPos, Angle);
	////			Stage::Instance().UpdateBlockTransform();
	////	}
	////}
//}

void PutBlock::putBlockOnBlock()
{
	performRaycast(RayState, RayEnd);
		//�X�e�[�W�Ƃ̃��C�L���X�g���s���z�u���W�����߂�
		HitPos, HitNor;

		/*if (Stage::Instance().BlockRayCast(
			RayState, RayEnd,
			HitPos, HitNor, HitType)) {
			UpdateTransform();
		}*/
		if(Stage::Instance().UnifiedRayCast(RayState, RayEnd,
			HitPos, HitNor, HitType, true))
		{
			UpdateTransform();
		}
		if (Input::Instance().GetMouse().GetButtonDown() & Mouse::BTN_LEFT)
		{
			Stage::Instance().putBlock(Type, HitPos, Angle);
			Stage::Instance().UpdateBlockTransform();
		}
}

void PutBlock::SetBlock()
{
	const int maxType = 4;
	const int minType = 0;

	if (Input::Instance().GetMouse().GetButtonDown() & Mouse::BTN_MIDDLE)
	{
		Angle.y += 45 * DirectX::XM_PI / 2.0f;

		// 360�x�𒴂��Ȃ��悤�ɒ��� (���b�v�A���E���h)
		if (Angle.y > 45 * DirectX::XM_PI / 2.0f * 4) {
			Angle.y -= 45 * DirectX::XM_PI / 2.0f * 4;
		}
	}
	if (Input::Instance().GetMouse().GetButtonDown() & Mouse::BTN_RIGHT)
	{
		Angle.y = 0;
		Type++;
		if (Type > maxType) 
		{
			Type = minType;
		}
	}
}

void PutBlock::Initialize()
{
	PutBlockModel1 = new Model("Data/Model/Block/Block1.mdl");
	PutBlockModel2 = new Model("Data/Model/Block/Block2.mdl");
	PutBlockModel3 = new Model("Data/Model/Block/Block3.mdl");
	PutBlockModel4 = new Model("Data/Model/Block/Block4.mdl");
	EraseBlockModel = new Model("Data/Model/Block/Erase.mdl");
}

void PutBlock::Finalize()
{
	delete PutBlockModel;
	delete PutBlockModel1;
	delete PutBlockModel2;
	delete PutBlockModel3;
	delete PutBlockModel4;
	delete EraseBlockModel;
	PutBlockModel = nullptr;
	PutBlockModel1 = nullptr;
	PutBlockModel2 = nullptr;
	PutBlockModel3 = nullptr;
	PutBlockModel4 = nullptr;
	EraseBlockModel = nullptr;
}

void PutBlock::render(const RenderContext& rc, ModelRenderer* renderer)
{
	switch (Type)
	{
	case 0:
		renderer->Render(rc, transform, EraseBlockModel, ShaderId::Lambert);
		break; 
	case 1:
		renderer->Render(rc, transform, PutBlockModel1, ShaderId::Lambert);
		break;
	case 2:
		renderer->Render(rc, transform, PutBlockModel2, ShaderId::Lambert);
		break;
	case 3:
		renderer->Render(rc, transform, PutBlockModel3, ShaderId::Lambert);
		break;
	case 4:
		renderer->Render(rc, transform, PutBlockModel4, ShaderId::Lambert);
		break;
	}
}

void PutBlock::UpdateTransform()
{
	float Blocksize = Stage::Instance().GetBlockSize();
	int mapX = Stage::Instance().GetMapXsize();
	int mapZ = Stage::Instance().GetMapZsize();
	scale = Stage::Instance().GetBlockscale();

	int Putx = static_cast<int>(HitPos.x / (Blocksize * scale.x));
	int Puty = static_cast<int>(HitPos.y / (Blocksize * scale.y));
	int Putz = static_cast<int>(HitPos.z / (Blocksize * scale.z));

	// �ʒu���u���b�N�̃O���b�h�ɍ��킹�邽�߂ɁAHitPos��␳
	HitPos.x = static_cast<float>(Putx) * Blocksize * scale.x;
	HitPos.y = static_cast<float>(Puty) * Blocksize * scale.y;
	HitPos.z = static_cast<float>(Putz) * Blocksize * scale.z;

	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(Angle.x,Angle.y, Angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(HitPos.x, HitPos.y, HitPos.z);
	DirectX::XMMATRIX WorldTransform = S * R * T;
	DirectX::XMStoreFloat4x4(&transform, WorldTransform);
}

void PutBlock::Update(float elapsedTime)
{
	SetBlock();
	//putBlock();
	putBlockOnBlock();
	UpdateTransform();
}

void PutBlock::performRaycast(DirectX::XMFLOAT3& RayState, DirectX::XMFLOAT3& RayEnd)
{

	screenWidth = Graphics::Instance().GetScreenWidth();
	screenHeight = Graphics::Instance().GetScreenHeight();

	// �}�E�X�J�[�\���ʒu�̎擾
	POINT cursor;
	::GetCursorPos(&cursor);
	::ScreenToClient(Graphics::Instance().GetWindowHandle(), &cursor);

	//�e�s����擾
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&Camera::Instance().GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//�X�N���[�����W�̐ݒ�
	ScreenPosition, WorldPosition;
	screenPosition;

	screenPosition.x = static_cast<float> (cursor.x);
	screenPosition.y = static_cast<float> (cursor.y);

	//�X�N���[�����W�����[���h���W�ϊ����A���C�̎n�_�ƃ��C�̏I�_�����߂�
	mouseX = static_cast<float>(cursor.x);
	mouseY = static_cast<float>(cursor.y);
	// �}�E�X�̃X�N���[�����W (Z=0.0f �̓j�A���ʁAZ=1.0f �̓t�@�[�t���X�g)
	DirectX::XMVECTOR ScreenPositionNear = DirectX::XMVectorSet(mouseX, mouseY, 0.0f, 1.0f);
	DirectX::XMVECTOR ScreenPositionFar = DirectX::XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);

	// �X�N���[����Ԃ��烏�[���h��Ԃւ̕ϊ� (�j�A���ʂ̓_)
	DirectX::XMVECTOR WorldPositionNear = DirectX::XMVector3Unproject(
		ScreenPositionNear,
		0.0f, 0.0f, screenWidth, screenHeight, 0.0f, 1.0f,
		Projection, View, World
	);

	// �X�N���[����Ԃ��烏�[���h��Ԃւ̕ϊ� (�t�@�[�t���X�g�̓_)
	WorldPositionFar = DirectX::XMVector3Unproject(
		ScreenPositionFar,
		0.0f, 0.0f, screenWidth, screenHeight, 0.0f, 1.0f,
		Projection, View, World
	);

	//�X�N���[�����W�����[���h���W�ϊ����A���C�̎n�_�ƃ��C�̏I�_�����߂�
	DirectX::XMStoreFloat3(&RayState, WorldPositionNear);
	DirectX::XMStoreFloat3(&RayEnd, WorldPositionFar);
}