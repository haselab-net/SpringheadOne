// CRBrain.h: CRBrain クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef CRBRAIN_H
#define CRBRAIN_H

#include <SceneGraph/SGScene.h>
#include <Creature/CRPuppet.h>
#include <Graphics/GRRender.h>
#include <Physics/PHSolid.h>
#include <Base/Base.h>

#include <Creature/CRVisionMotionAnalysis.h>
#include <Creature/CREye.h>
#include <Creature/CRAttention.h>
#include <Creature/CRNeckController.h>
#include <Creature/CRInternalModel.h>
#include <Creature/CRActionPlanner2.h>

namespace Spr{

	class CRBrain{
	public:
		// インターフェース
		void Load(SGScene* scene, CRPuppet* crPuppet, CRUser* crUser);
		void Step();
		void Draw(GRRender* render);
	};

	class CRBaseBrain: public CRBrain{
	public:
		// 基本インターフェース
		void Load(SGScene* scene, CRPuppet* crPuppet, CRUser* crUser);
		void Step();
		void Draw(GRRender* render);

		// 拡張インターフェース
		
		// メンバ変数
		/// 入出力器官
		SGScene*  scene;
		CRPuppet* puppet;
		CRUser*   user;

		/// 処理モジュール
		CRVisionMotionAnalysis crvMotionAnalysis; // 視覚系 動きの解析
		CRAttention            crAttention;       // 注意の機構
		CREye                  crEye;             // 目の動き
		CRNeckController       crNeckController;  // 首＝頭部の動き

		// デバッグ用
		PHSolid*  soLEye;
	};

	class CRSimulatingBrain: public CRBaseBrain{
	public:
		// 基本インターフェース
		void Load(SGScene* scene, CRPuppet* crPuppet, CRUser* crUser);
		void Step();
		void Draw(GRRender* render);
		
		// 拡張インターフェース
		void SetFilename(std::string filename){this->filename = filename;}
		void SetRender(GRRender* render){this->render = render;}
		CRActionPlanner* GetPlanner(){return &crPlanner;}
		void ToggleEyeMode();
		
		// メンバ変数
		/// 処理モジュール
		CRInternalModel  crInternalModel; // 予測用の内部モデル
		CRActionPlanner crPlanner; // 予測機構

		/// InternalModelの初期化のために保持
		std::string filename;
		GRRender*   render;
	};

} // namespace Spr

#endif // ifndef CRBRAIN_H

