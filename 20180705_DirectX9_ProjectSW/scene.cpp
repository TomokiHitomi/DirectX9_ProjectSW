//=============================================================================
//
// シーン処理 [scene.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "scene.h"

/* Scene */
#include "title.h"
#include "game.h"
#include "result.h"

/* System */
#include "camera.h"
#include "light.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

/* Ui */

/* Object */

/* デバッグ */
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
//始めはシーンを空にしておく
BaseScene *SceneManager::m_pScene = NULL;

SceneManager::SCENE	SceneManager::m_eScene;
float				SceneManager::m_fRate;
float				SceneManager::m_fRateInBonus;
int					SceneManager::m_nWin;
int					SceneManager::m_nAiLevel;
int					SceneManager::m_nPlayerNum;
int					SceneManager::m_nPlayerType;

int					SceneManager::m_nTotalBurnBonus;
int					SceneManager::m_nTotalWetBonus;
int					SceneManager::m_nTotalBurnChain;

//=============================================================================
// シーン管理処理
//=============================================================================
void SceneManager::ChangeScene(SCENE scene)
{

	if (m_pScene != NULL)
	{
		delete m_pScene;
	}

	// SEの停止処理
	EndSeS();

	// 選択されたシーンを保管
	m_eScene = scene;

	switch (scene)
	{	//引数のシーン
	case SCENE::TITLE:
		SetSoundBgm(SOUND_BGM_NON);
		m_pScene = new TitleScene();	// タイトルシーンを現在のシーンにする
		break;
	case SCENE::GAME:
		SetSoundBgm(SOUND_BGM_NON);
		m_pScene = new GameScene();		// ゲームシーンを現在のシーンにする
		break;
	case SCENE::RESULT:
		SetSoundBgm(SOUND_BGM_NON);
		m_pScene = new ResultScene();	// リザルトシーンを現在のシーンにする
		break;
	default:
		break;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
void SceneManager::Init(HINSTANCE hInst, HWND hWnd)
{
	InitLight();			// ライト
	InitInput(hInst, hWnd);	// 入力
	InitSound(hWnd);		// サウンド
	InitFade();				// フェード
	Camera::Init();			// カメラ(終了処理はシーンマネージャーのみ）

	ChangeScene(m_eScene);	// 初期シーン設定

#ifdef _DEBUG
	InitDebugProc();		// デバッグ
#endif
}

//=============================================================================
// 終了処理
//=============================================================================
void SceneManager::Uninit(void)
{

	UninitInput();			// 入力
	Camera::Uninit();			// カメラ
	UninitSound();			// サウンド
	UninitFade();			// フェード

#ifdef _DEBUG
	UninitDebugProc();		// デバッグ
#endif
}

//=============================================================================
// 更新処理
//=============================================================================
void SceneManager::Update(void)
{
	UpdateInput();			// 入力
	UpdateFade();			// フェード

#ifdef _DEBUG
	DebugScene();			// デバッグ用
#endif

	m_pScene->Update();		// 現在のシーンの更新関数

	Camera::Update();			// カメラ
	UpdateSound();			// サウンド
}

//=============================================================================
// 描画処理
//=============================================================================
void SceneManager::Draw(void)
{
	Camera::Set();			// カメラ

	m_pScene->Draw();		// 現在のシーンの描画関数

	DrawFade();				// フェード

#ifdef _DEBUG
	DrawDebugProc();
#endif
}

//=============================================================================
// コンストラクタ処理
//=============================================================================
SceneManager::SceneManager(void)
{
	// 初期のシーンをTITLEに設定
	m_eScene = TITLE;

	// 各変数を初期化
	m_nWin = 0;				// 勝利プレイヤー
	m_nAiLevel = 0;			// AIの強さ
	m_fRate = 0.0f;			// 残存割合
	m_fRateInBonus = 0.0f;	// 残存割合（ボーナス込み）
	m_nPlayerNum = 0;		// プレイヤー人数
	m_nPlayerType = 0;		// プレイヤータイプ
}

//=============================================================================
// デストラクタ処理
//=============================================================================
SceneManager::~SceneManager(void)
{
	if (m_pScene != NULL)
	{
		delete m_pScene;
	}
}

//=============================================================================
// シーン管理処理
//=============================================================================
void SceneManager::EndSeS(void)
{
	EndSe(SE_FIRE1);
	EndSe(SE_WATER_SUCK);
	EndSe(SE_WATER1);
	EndSe(SE_WATER2);
	EndSe(SE_WIND_SUCK);
	EndSe(SE_WIND1);
}

#ifdef _DEBUG
//=============================================================================
// デバッグ処理
//=============================================================================
void SceneManager::DebugScene(void)
{
	// デバッグの更新処理
	UpdateDebugProc();

	// シーン情報を表示
	PrintDebugProc("【 SCENE 】\n");

	// 現在のシーンを表示
	switch (m_eScene)
	{
	case SCENE::TITLE:
		PrintDebugProc("Scene[TITLE ]  ");
		break;
	case SCENE::GAME:
		PrintDebugProc("Scene[ GAME ]  ");
		break;
	case SCENE::RESULT:
		PrintDebugProc("Scene[RESULT]  ");
		break;
	}


	// プレイヤー人数、コントローラー番号、残存割合を表示
	PrintDebugProc("Rate[%f]  1P:Num[%d]  Type[%d]\n",
		m_fRate, m_nPlayerNum, m_nPlayerType);

	//PrintDebugProc("【 Result 】\n");
	//PrintDebugProc("Rate [%f]  ", GetStageRate());
	//switch (GetStageWinPlayer())
	//{
	//case STAGE_WIN_NON:
	//	PrintDebugProc("Win[NON]\n");
	//	break;
	//case STAGE_WIN_1P:
	//	PrintDebugProc("Win[1P]\n");
	//	break;
	//case STAGE_WIN_2P:
	//	PrintDebugProc("Win[2P]\n");
	//	break;
	//}
	//PrintDebugProc("\n");
	if (GetKeyboardTrigger(DIK_F2))
	{
		SetFadeScene(TITLE);
		//ChangeScene(TITLE);
	}
	if (GetKeyboardTrigger(DIK_F3))
	{
		SetFadeScene(GAME);
		//ChangeScene(RESULT);
	}
	if (GetKeyboardTrigger(DIK_F4))
	{
		SetFadeScene(RESULT);
		//ChangeScene(SELECT);
	}
	// SEテスト（sound.hをincludeして関数を呼ぶ）
	//if (GetKeyboardTrigger(DIK_1))
	//{	// 
	//	SetSe(SE_SELECT_ERROR, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_2))
	//{	// 
	//	SetSe(SE_GAME_SELECT, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_3))
	//{	// 
	//	SetSe(SE_GAME_KETTEI, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_4))
	//{	//		
	//	SetSe(SE_RESULT_SELECT, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_5))
	//{	// 
	//	SetSe(SE_RESULT_KETTEI, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_6))
	//{	// 
	//	SetVoice(VOICE_3, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_7))
	//{	// 
	//	SetVoice(VOICE_2, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_8))
	//{	// 
	//	SetVoice(VOICE_START, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}

	PrintDebugProc("\n");
}
#endif