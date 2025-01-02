
#include "System/Input.h"
#include "CameraController.h"
#include "Camera.h"

void CameraController::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();

  

    // ボタン入力の確認（例：Aボタンで回転開始）
    {
        if (ax != 0)
        {
            if (!isRotating)
            {
                isRotating = true; // 回転開始
                currentTime = 0.0f; // 経過時間をリセット
                if (ax != 0)
                {
                    if (ax < 0) AddAng = +45;
                    else if (ax > 0) AddAng = -45;
                }
              

                targetAngle.x = angle.x; // X軸はそのまま
                targetAngle.y = angle.y + DirectX::XMConvertToRadians(AddAng); // 45度回転
                targetAngle.z = angle.z; // Z軸はそのまま

                // Y軸角度を-π～πの範囲に収める（ここだけ調整）
                if (targetAngle.y < -DirectX::XM_PI)
                {
                    targetAngle.y += DirectX::XM_2PI;
                }
                if (targetAngle.y > DirectX::XM_PI)
                {
                    targetAngle.y -= DirectX::XM_2PI;
                }
            }
        }
        if (ay != 0)
        {
            range += ay * rangeChangeSpeed * elapsedTime; // ayの値でrangeを増減
            if (range < minRange) range = minRange;      // 最小値制限
            if (range > maxRange) range = maxRange;      // 最大値制限
        }

        // 回転中の処理
        if (isRotating)
        {
            currentTime += elapsedTime; // 時間を進める

            // 補間係数を計算（0～1の範囲）
            float t = currentTime / rotationDuration;
            if (currentTime >= rotationDuration) // 回転終了条件
            {
                t = 1.0f;
                isRotating = false; // 回転終了
            }

            // Y軸の角度差がπを超える場合、最短経路で補間する
            float deltaY = targetAngle.y - angle.y;
            if (deltaY > DirectX::XM_PI)
            {
                deltaY -= DirectX::XM_2PI; // -π〜πの範囲内に収める
            }
            else if (deltaY < -DirectX::XM_PI)
            {
                deltaY += DirectX::XM_2PI; // -π〜πの範囲内に収める
            }

            // 線形補間を使用して現在の角度を更新
            angle.x = angle.x + (targetAngle.x - angle.x) * t;
            angle.y = angle.y + deltaY * t; // 最短経路を使用
            angle.z = angle.z + (targetAngle.z - angle.z) * t;
        }

        // 以下は元のカメラ計算処理
        // X軸のカメラ回転を制限
        if (angle.x < minAngleX)
        {
            angle.x = minAngleX;
        }
        if (angle.x > maxAngleX)
        {
            angle.x = maxAngleX;
        }
        // 回転行列やカメラ位置の計算はそのまま
        DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
        DirectX::XMVECTOR Front = Transform.r[2];
        DirectX::XMFLOAT3 front;
        DirectX::XMStoreFloat3(&front, Front);

        DirectX::XMFLOAT3 eye;
        eye.x = target.x - front.x * range;
        eye.y = target.y - front.y * range;
        eye.z = target.z - front.z * range;

        Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

    }
}



void CameraController::UpdateVer1(float elapsedTime)
{
    Input& input = Input::Instance();

    // マウスの移動量を取得
    float deltaX = input.GetMouse().GetPositionX() - input.GetMouse().GetOldPositionX();
    float deltaY = input.GetMouse().GetPositionY() - input.GetMouse().GetOldPositionY();

    // マウスの感度を設定
    const float mouseSensitivity = 0.005f;

    // カメラの回転速度
    float speed = rollSpeed * elapsedTime;

    // マウスの移動量に応じてカメラの回転角度を更新
    angle.x += deltaY * mouseSensitivity; // 上下反転を考慮
    angle.y += deltaX * mouseSensitivity;



    // 回転をスムーズにするための補間を追加
    static DirectX::XMFLOAT3 smoothAngle = angle;
    const float smoothFactor = 0.1f; // 滑らかさを調整
    smoothAngle.x += (angle.x - smoothAngle.x) * smoothFactor;
    smoothAngle.y -= (angle.y - smoothAngle.y) * smoothFactor;


    // カメラ回転値を回転行列に変換（引数はピッチ、ヨー、ロール）
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方向ベクトルを取り出す（Z軸ベクトル）
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // マウスの左クリックが押されている間のみカメラを制御
    if (input.GetMouse().GetButton() & Mouse::BTN_LEFT)
    {
        // マウスカーソルを画面中央に戻す
        POINT center;
        center.x = input.GetMouse().GetScreenWidth() / 2;
        center.y = input.GetMouse().GetScreenHeight() / 2;
        SetCursorPos(center.x, center.y);

        // マウスの旧位置を画面中央にリセット
        input.GetMouse().Update();
    }
    // カメラに視点を注視点を設定（引数：eye , focus , up)
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateVer2(float elapsedTime)
{
    Input& input = Input::Instance();

    // マウスの移動量を取得
    static int lastMouseX = input.GetMouse().GetPositionX();
    static int lastMouseY = input.GetMouse().GetPositionY();

    int currentMouseX = input.GetMouse().GetPositionX();
    int currentMouseY = input.GetMouse().GetPositionY();

    float deltaX = static_cast<float>(currentMouseX - lastMouseX);
    float deltaY = static_cast<float>(currentMouseY - lastMouseY);

    lastMouseX = currentMouseX;
    lastMouseY = currentMouseY;

    // マウスの感度を設定
    const float mouseSensitivity = 0.02f; // 感度を調整

    // カメラの回転速度
    float speed = rollSpeed * elapsedTime;

    // マウスの移動量に応じてカメラの回転角度を更新
    angle.x += deltaY * mouseSensitivity; // 上下反転を考慮
    angle.y += deltaX * mouseSensitivity;

    //// X軸のカメラ回転を制限
    //if (angle.x < minAngleX)
    //{
    //    angle.x = minAngleX;
    //}
    //if (angle.x > maxAngleX)
    //{
    //    angle.x = maxAngleX;
    //}

    //// Y軸の回転値を-3.14～3.14に収まるようにする
    //if (angle.y < -DirectX::XM_PI)
    //{
    //    angle.y += DirectX::XM_2PI;
    //}
    //if (angle.y > DirectX::XM_PI)
    //{
    //    angle.y -= DirectX::XM_2PI;
    //}

    // 回転をスムーズにするための補間を追加
    static DirectX::XMFLOAT3 smoothAngle = angle;
    const float smoothFactor = 0.1f; // 滑らかさを調整
    smoothAngle.x += (angle.x - smoothAngle.x) * smoothFactor;
    smoothAngle.y += (angle.y - smoothAngle.y) * smoothFactor;

    // カメラ回転値を回転行列に変換（引数はピッチ、ヨー、ロール）
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(smoothAngle.x, smoothAngle.y, smoothAngle.z);

    // 回転行列から前方向ベクトルを取り出す（Z軸ベクトル）
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // カメラに視点を注視点を設定（引数：eye , focus , up)
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateVer3(float elapsedTime)
{
    Input& input = Input::Instance();

    // マウスの移動量を取得
    float deltaX = input.GetMouse().GetPositionX() - input.GetMouse().GetOldPositionX();
    float deltaY = input.GetMouse().GetPositionY() - input.GetMouse().GetOldPositionY();

    // マウスの感度を設定
    const float mouseSensitivity = 0.005f;

    // カメラの回転速度
    float speed = rollSpeed * elapsedTime;

    // マウスの移動量に応じてカメラの回転角度を更新
    angle.x += deltaY * mouseSensitivity; // 上下反転を考慮
    angle.y += deltaX * mouseSensitivity;

    // X軸のカメラ回転を制限
    if (angle.x < -360)
    {
        angle.x = 0;
    }
    if (angle.x > 360)
    {
        angle.x = 0;
    }

    // Y軸の回転値を-3.14～3.14に収まるようにする
    //if (angle.y < -360)
    //{
    //    angle.y = 0;
    //}
    //if (angle.y > 360)
    //{
    //    angle.y = 0;
    //}

    // カメラ回転値を回転行列に変換（引数はピッチ、ヨー、ロール）
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方向ベクトルを取り出す（Z軸ベクトル）
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // カメラに視点を注視点を設定（引数：eye , focus , up)
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::UpdateVer4(float elapsedTime)
{
    Input& input = Input::Instance();

    // マウスの移動量を取得
    float deltaX = input.GetMouse().GetPositionX() - input.GetMouse().GetOldPositionX();
    float deltaY = input.GetMouse().GetPositionY() - input.GetMouse().GetOldPositionY();

    // マウスの感度を設定
    const float mouseSensitivity = 0.005f;

    // マウスの移動量に応じてカメラの回転角度を更新
    angle.x += deltaY * mouseSensitivity; // 上下反転を考慮
    angle.y += deltaX * mouseSensitivity;

    // マウスホイールの入力を取得
    float wheelDelta = input.GetMouse().GetWheel();
    const float zoomSpeed = 5.0f; // ホイール感度
    range -= wheelDelta * zoomSpeed;

    // 距離の範囲制限
    const float minRange = 5.0f;  // 最小距離
    const float maxRange = 50.0f; // 最大距離

    // カメラ回転値を回転行列に変換（引数はピッチ、ヨー、ロール）
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方向ベクトルを取り出す（Z軸ベクトル）
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    // 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    // カメラに視点と注視点を設定（引数：eye , focus , up)
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}