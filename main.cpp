#include <Novice.h>
#include <imgui.h>
#include <KamataEngine.h>
#include "MassFunction.h"
#include "DrawFunction.h"
const char kWindowTitle[] = "学籍番号";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	Vector3 worldRotate = { 0.0f, 0.0f, 0.0f };
	Vector3 worldTraslate = { 0.0f, 0.0f, 0.0f };
	Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };
	
	Vector3 Translates[3] {
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f},
	};
	Vector3 rotates[3] {
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f},
	};
	Vector3 scales[3]{
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
	};

	unsigned int Color = WHITE; // グリッドの色

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		if (keys[DIK_W] != 0) cameraTranslate.z += 0.1f; // 前進
		if (keys[DIK_S] != 0) cameraTranslate.z -= 0.1f; // 後退	
		if (keys[DIK_A] != 0) cameraTranslate.x -= 0.1f; // 左移動
		if (keys[DIK_D] != 0) cameraTranslate.x += 0.1f; // 右移動
		if (keys[DIK_UP] != 0) cameraRotate.x += 0.1f; // 前進
		if (keys[DIK_DOWN] != 0) cameraRotate.x -= 0.1f; // 後退	
		if (keys[DIK_LEFT] != 0) cameraRotate.y -= 0.1f; // 左移動
		if (keys[DIK_RIGHT] != 0) cameraRotate.y += 0.1f; // 右移動

		ImGui::Begin("Window");
			 
		ImGui::SliderFloat3("translateS",&(Translates[0].x),-10.0f,10.0f);
		ImGui::SliderFloat3("rotateS",&(rotates[0].x),-10.0f,10.0f);
		ImGui::SliderFloat3("scaleS",&(scales[0].x),-10.0f,10.0f);
		ImGui::SliderFloat3("translateN",&(Translates[1].x),-10.0f,10.0f);
		ImGui::SliderFloat3("rotateN",&(rotates[1].x),-10.0f,10.0f);
		ImGui::SliderFloat3("scaleN",&(scales[1].x),-10.0f,10.0f);
		ImGui::SliderFloat3("translateH",&(Translates[0].x),-10.0f,10.0f);
		ImGui::SliderFloat3("rotateH",&(rotates[2].x),-10.0f,10.0f);
		ImGui::SliderFloat3("scaleH",&(scales[2].x),-10.0f,10.0f);
		
		ImGui::End();

		
			
		Matrix4x4 localMatrixs[3];
		for (int i = 0; i < 3; i++)
		{
			localMatrixs[i]=MakeAfineMatrix(scales[i],rotates[i],Translates[i]);
		}
		Matrix4x4 worldMatrixs[3];
		worldMatrixs[0]=localMatrixs[0];
		worldMatrixs[1]=Multiply(localMatrixs[1],worldMatrixs[0]);
		worldMatrixs[2]=Multiply(localMatrixs[2],worldMatrixs[1]);

			
		Matrix4x4 worldMatrix = MakeAfineMatrix({ 1.0f, 1.0f, 1.0f }, worldRotate, worldTraslate);
		//MatrixScreenPrintf(0, kRowHeight+10, worldMatrix, "worldMatrix");
		Matrix4x4 cameraMatrix = MakeAfineMatrix({ 1.0f, 1.0f, 1.0f },cameraRotate,cameraTranslate );
		//MatrixScreenPrintf(0, kRowHeight * 20, cameraMatrix, "cameraMatrix");
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		//MatrixScreenPrintf(0, kRowHeight * 30, viewMatrix, "viewMatrix");
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kwindowWidth)/static_cast <float>(kwindowHight) , 0.1f, 100.0f);
		//MatrixScreenPrintf(kColumnWidth*10, kRowHeight  , projectionMatrix, "projectionMatrix");
		Matrix4x4 worldViewProjectionMatrix = Multiply( worldMatrix,Multiply(viewMatrix,projectionMatrix));
		Matrix4x4 viewProjectionMatrix =Multiply(viewMatrix,projectionMatrix);

		//MatrixScreenPrintf(kColumnWidth*10, kRowHeight * 20, viewProjectionMatrix, "worldViewProjectionMatrix");
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kwindowWidth), static_cast<float>(kwindowHight), 0.0f, 1.0f);

		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		 
		for (int i = 0; i < 3; i++)
		{
			Sphere sphere={Vector3{ worldMatrixs[i].m[3][0], worldMatrixs[i].m[3][1] ,worldMatrixs[i].m[3][2]},0.05f};

			switch (i)
			{
			case 0:

			Color=RED;
			break;
			case 1:

			Color=GREEN;
			break;
			case 2:

			Color=BLUE;
			break;
			}


			DrawSphere(sphere,viewProjectionMatrix,viewportMatrix,Color);
		}
		DrawRail(
		Vector3{ worldMatrixs[0].m[3][0], worldMatrixs[0].m[3][1] ,worldMatrixs[0].m[3][2]},
		Vector3{ worldMatrixs[1].m[3][0], worldMatrixs[1].m[3][1] ,worldMatrixs[1].m[3][2]},
		viewProjectionMatrix,viewportMatrix,WHITE
		);
		
		DrawRail(
		Vector3{ worldMatrixs[1].m[3][0], worldMatrixs[1].m[3][1] ,worldMatrixs[1].m[3][2]},
		Vector3{ worldMatrixs[2].m[3][0], worldMatrixs[2].m[3][1] ,worldMatrixs[2].m[3][2]},
		viewProjectionMatrix,viewportMatrix,WHITE
		);
		
		
		
		DrawGrid(viewProjectionMatrix,viewportMatrix);
	
		
		
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
