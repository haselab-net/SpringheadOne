簡単な説明書

ごくごく簡単にですが、石川の書いたプログラム(VHInteraction)の説明をします。
引き継ぐ方には申し訳ないですが、時間の都合上詳しく説明できなくなってしまいました。
コメント文が少なく、見づらいソースコードでごめんなさい。


Creature\CRActionPlanner.cpp
    CRActionPlannerクラス：
        VHの予測行動モデル(運動予測→接触判定→行動選択→動作決定)
        
        Step()：
            運動予測開始時に手先の運動の近似多項式を計算(CRPuppet::LocusGenerator)
            予測中はPredictionAction()へ
        PredictionAction()：
            実時間運動の状態保存→MovementPrediction()→状態読み込み→行動変更するか判定(ChooseTargetAction())
        MovementPrediction()：
            予測運動の状態読み込み→PredictionStep()→ContactTest()→状態保存
        PredictionStep()：
            予測運動のsceneの更新
            VHの到達運動およびUserの手先運動(user->SetExpectedPos())に必要な力を加える
            // 予測部分は怪しいので考え直す必要あり
        ContactTest()：
            VHとUserの剛体同士の接触判定(CRPuppet::HumanContactInfo)
            接触情報(ContactInfoクラス)を保存
            ContactInfoクラス：
                VHの剛体、Userの剛体、接触位置、接触時刻、接触タイプ
        ChooseTargetAction()：
            接触情報から次の目標行動選択および動作決定(未完成)
            VHの目標行動および動作は、両腕および体の計３ヶ所への到達運動指令(CRPuppet::ReachingMovement)
            // 現在は攻撃に対しての防御行動のみ完成
            防御および回避の目標位置は、それぞれGetPointToGuard()・GetPointToAvoid()で計算

Creature\CRPuppet.cpp
    CRPuppetクラス：
        CRHumanの派生クラス
        PD制御による外力にて運動制御
        固定箇所：腰部の位置・姿勢
        
        SetSpringForce()：
            位置姿勢制御の外力計算
        Step()：
            SetSpringForce()および到達運動制御
        Attack()：
            攻撃動作の到達運動決定
            // 発生条件は基本的にランダム
        GuardTest()：
            シミュレータによる予測無しでの防御動作の実験に使用
        IsAimed()：
            CRActionPlannerにて運動予測を開始するかの判断材料
        
        PositionSpringクラス：
                剛体の位置制御のバネ(剛体、制御位置、目標位置、目標速度、加える力、...)
                AddSpringForce()：
                    PD制御による必要な力の計算
        PostureSpringクラス：
                剛体の姿勢制御のバネ(剛体、目標姿勢(クォータニオン)、目標角速度、加えるトルク、...)
                AddSpringTorque()：
                    PD制御による必要なトルクの計算
        ReachingMovementクラス：
                到達運動制御(最終目標位置、最終目標速度、目標時間、持続時間)
                // 基本はPositionSpringと同様
                Step()：
                    目標時間内は次ステップまでの仮目標を定めPD制御
                    目標時間が終了したら、持続時間が終わるまで最終目標位置に位置制御
                // 到達運動部分はやっつけ仕事で作ったので、怪しいです
        LocusGeneratorクラス：
                剛体の過去の軌跡から、今後の軌道を最小二乗法により多項式近似
                // 過去サンプル数や多項式の次数・重みは実験段階
        HumanContactInfoクラス：
                CRHuman同士の剛体の接触判定
                剛体同士の接触判定、Humanと相手剛体の接触判定、接触力の計算など

Creature\CRUser.cpp
    CRUserクラス：
        CRPuppetの派生クラス
        固定箇所：腰部の位置・姿勢
        SPIDARによる制御対象：両手先
        
        SetSpidarPos()：
            SPIDARにて計測された位置情報から、両手先の目標を決定
        SetExpectedPos()：
            CRActionPlannerにて運動予測中の両手先の目標を、LocusGeneratorから決定
        SetSpidarForce()：
            User剛体に生じた接触力を提示


 ※ 動画撮影やデモでの見栄えを良くする為に、各種パラメータや関数内容を多少いじくってあります。
    そのため論文の内容と必ずしも一致しない部分がいくつか有ると思います。
