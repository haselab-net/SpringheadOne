// CRSupportArea.cpp: CRSaftyArea クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRSupportArea.h"
#include "CRHuman.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRSupportArea::CRSupportArea()
{
	collision = NULL;
	bAdd = false;
	bClear = false;
}

CRSupportArea::~CRSupportArea()
{

}

void CRSupportArea::Step(){
	if(coVetxs.size() > 0){
		CalcSupportArea();
	}
	bClear = true;	//描画後にcoVetxsをクリアする。そのbool
}

void CRSupportArea::Load(SGScene* scene,CRHuman* crHuman){
	if(bAdd){
		ClearCoVetxs();
		collision = NULL;
		bAdd = false;
	}
	if(!bAdd){
		AddListener(scene);
		SelectCDFrame(crHuman);
	}
}

void CRSupportArea::AddListener(SGScene* scene){
	if(!bAdd && scene){
		scene->GetBehaviors().Find(collision);
		if(collision){
		listenerPos = collision->AddListener(this);
		bAdd = true;
		}
	}
}

void CRSupportArea::Analyzed(SGScene* scene, CDFramePairWithRecord* fr,
								 CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){
	if(bClear){
		ClearCoVetxs();
	}
	bClear = false;

	for(CDQHPlane<CDFace>* it = analyzer->planes.begin; it != analyzer->planes.end;++it){
		//双対変換をしている。
		//it->deletedがtrueになっていると使わなくなった面を示す。
		if(it->dist != 0 && !it->deleted){
			coVetxs.push_back(it->normal / it->dist + conv->commonPoint);
		}
	}
}

void CRSupportArea::ClearCoVetxs(){
	coVetxs.clear();
}

void CRSupportArea::SelectCDFrame(CRHuman* crHuman){
	if (collision){
		for(CDCollisionEngine::TFrameIt it = collision->FramePairBegin(); it != collision->FramePairEnd(); ++it){
			if (!*it) continue;
			static int iii =0;
			bool b0 = (*it)->frame[0] && HasFrame((*it)->frame[0]->frame,crHuman);
			bool b1 = (*it)->frame[1] && HasFrame((*it)->frame[1]->frame,crHuman);
			if (!((b0 && b1) || (!b0 && !b1))){
				collision->AddActive((*it)->frame[0]->frame,(*it)->frame[1]->frame, listenerPos);
			}
		}
		collision->Init();
	}
}

bool CRSupportArea::HasFrame(SGFrame* f,CRHuman* crHuman){
	for(int i=0; i < crHuman->solids.size(); i++){
		if (crHuman->solids[i] && crHuman->solids[i]->GetFrame() == f) return true;
	}
	return false;
}

void CRSupportArea::DrawCollisionVetxs(GRRender* render){
	if(render){
		render->SetModelMatrix(Spr::Affinef());
		render->SetLineWidth(6);
		render->SetMaterial(GRMaterialData(Spr::Vec4f(1,0,0,1)));
		
		//接触点を描画
		for(int i = 0; i < coVetxs.size(); ++i){
			render->DrawDirect(GRRender::POINTS,&coVetxs[i],&coVetxs[i]+1);
		}
		
		//　安定領域の描画
		if(supportVetxs.size() > 0){
			supportVetxs.push_back(supportVetxs[0]);
			for(int i = 0; i < supportVetxs.size()-1; ++i){
				render->DrawDirect(GRRender::LINES, &supportVetxs[i], &supportVetxs[i+2]);
			}
		}
	}
	//bClear = true;	//描画後にcoVetxsをクリアする。そのbool
	if(bClear){
		ClearCoVetxs();
	}
}
/*
void CRSupportArea::CalcSupportArea(){

	supportVetxs.clear();

	// coVetxsを二次元化
	for(int i = 0; i < coVetxs.size(); i++){
		coVetxs[i].y = 0.0f;
	}

	// coVetxsの重心座標を計算
	Vec3f cog = Vec3f();
	for(int i = 0; i < coVetxs.size(); i++){
		cog += coVetxs[i];
	}
	cog /= coVetxs.size();

	// 重心から一番離れている点
	float maxPos = 0;				// 比較に使われる絶対値の最大値
	int startVetx = 0;				// 安定領域の最初の点
	for(int i = 0; i < coVetxs.size(); i++){
		float c = (cog - coVetxs[i]).norm();
		if(maxPos < c){
			maxPos = c;
			startVetx = i;
		}
	}
	supportVetxs.push_back(coVetxs[startVetx]);	//重心から離れている点を外形の第一の点とする
	
	// 重心と一番離れている点からの角度が一番大きい点を探す
	Vec3f newVetx;	//次の安定領域の点

	Vec3f zero_cog = cog - supportVetxs[0]; //第一の点から重心へのベクトル
	float cos_min=1;				//余弦の最小値
	for(int i = 0; i < coVetxs.size(); i++){
		if(!(i == startVetx)){	// 第一点以外
		//if(!(coVetxs[i] == supportVetxs[0])){	// 第一点以外
			//余弦を求める
			Vec3f zero_other = coVetxs[i] - supportVetxs[0];	//第一の点からそれぞれの点へのベクトル
			float cos_buf = (zero_cog*zero_other)/(abs(zero_cog.norm())*abs(zero_other.norm()));
			//余弦最小値
			if(cos_buf < cos_min){
				newVetx = coVetxs[i];
				cos_min = cos_buf;
			}
		}
	}
	supportVetxs.push_back(newVetx);	//第二点目


	//他の安定領域の点を検索
	for(int i = 0; i < coVetxs.size(); i++){
		Vec3f supportAreaLine = supportVetxs[i+1] - supportVetxs[i];	//外形の基準となるベクトル
		cos_min=1.0f;	//cos_min 初期化
		
		//ある基準点から見て
		for(int j = 0; j < coVetxs.size(); j++){
			//すでに見つけた安定領域の点を除く
			if(coVetxs[i] != supportVetxs[i+1] && coVetxs[i] != supportVetxs[i]){
				// 基準の点から他の点へのベクトル
				Vec3f  supportAreaLineBuf = coVetxs[i] - supportVetxs[i+1];
				//余弦を求める
				double cos_buf = (supportAreaLineBuf*supportAreaLine)/(supportAreaLineBuf.norm()*supportAreaLine.norm());
				//余弦最小値
				if(cos_buf<cos_min)
				{
					newVetx = coVetxs[i];
					cos_min=cos_buf;
				}
			}
		}
		supportVetxs.push_back(newVetx);
		if(newVetx ==supportVetxs[0])
			break;
	}
}
*/
void CRSupportArea::CalcSupportArea()
{

	supportVetxs.clear();

	// --------------------------
	// データをvectorに入れる　 -
	// --------------------------

	point_number=coVetxs.size();

	for(int j=0;j<coVetxs.size();j++)
	{
		
		M_vec.resize(point_number);
		
		M_vec[j].x = coVetxs[j].x;
		M_vec[j].y = coVetxs[j].z;
		M_vec[j].n =j;
		//dout<<"ポイント個数："<< point_number <<eol;
		for(int i=0;i<point_number;i++)
		{
		//	dout<<"X"<<i+1<<":" << M_vec[i].x <<eol;
		//	dout<<"Y"<<i+1<<":" << M_vec[i].y <<eol;
		}
	}
	// --------------------------


	int point_number_calc = point_number-1;
		//マウスの値をとるときに点の個数point_numberは最後に１個多くなる
	
	// --------------------------
	// 重心の座標位置を計算する -
	// --------------------------
	M_point sum_point={0,0,0};		//合計値＆平均値(重心座標)
	for(int i=0;i<point_number_calc;i++)
	{
		sum_point.x += M_vec[i].x;
		sum_point.y += M_vec[i].y;
	}
	sum_point.x = sum_point.x/(double)(point_number_calc);
	sum_point.y = sum_point.y/(double)(point_number_calc);
	coCog.x = sum_point.x;
	coCog.y = sum_point.y;

	//dout<<"重心X："<<sum_point.x <<"重心Y："<<sum_point.y <<eol;
	// --------------------------

	// --------------------------
	// 重心から一番離れている点 -
	// --------------------------
	double max_p=0;					//絶対値の最大値
	M_point max_val={0,0,0};		//絶対値の一番大きい座標とその番号
	for(int i=0;i<point_number_calc;i++)
	{
		if(max_p < sqrt(pow((sum_point.x - M_vec[i].x),2)+pow((sum_point.y - M_vec[i].y),2)))
		{
			max_val.x = M_vec[i].x;
			max_val.y = M_vec[i].y;
			max_val.n=i;
			max_p = sqrt(pow((sum_point.x - M_vec[i].x),2)+pow((sum_point.y - M_vec[i].y),2));
		}
	}
	search_number=1;				//重心から離れている点を外形の第一の点とする
	S_vec.resize(search_number);
	S_vec[0].x=max_val.x;
	S_vec[0].y=max_val.y;
	S_vec[0].n=max_val.n;
//	dout<<"Xmax："<<(int)S_vec[0].x <<" Ymax："<<(int)S_vec[0].y <<" number："<<(int)S_vec[0].n <<eol;
	// --------------------------

	// ------------------------------------------------------
	// 重心と一番離れている点からの角度が一番大きい点を探す -
	// ------------------------------------------------------
	M_point center_vec;				//第一の点から重心へのベクトル
	M_point each_buf_vec;			//第一の点からそれぞれの点へのベクトル

	double cos_min=1;				//余弦の最小値
					
	//第一の点から重心へのベクトル　center_vec
	center_vec.x = sum_point.x - S_vec[0].x;
	center_vec.y = sum_point.y - S_vec[0].y;
	
	search_number=2;				//外形となる点をもうひとつ決める
	S_vec.resize(search_number);

	for(int i=0;i<point_number_calc;i++)
	{
		if(i != S_vec[0].n)
		{
			//第一の点からそれぞれの点へのベクトル	each_buf_vec
			each_buf_vec.x = M_vec[i].x - S_vec[0].x;
			each_buf_vec.y = M_vec[i].y - S_vec[0].y;
			//余弦を求める
			double cos_buf = ((center_vec.x*each_buf_vec.x)+(center_vec.y*each_buf_vec.y))
				/(sqrt(pow(each_buf_vec.x,2)+pow(each_buf_vec.y,2))*sqrt(pow(center_vec.x,2)+pow(center_vec.y,2)));
			//余弦最小値
			if(cos_buf<cos_min)
			{
				S_vec[1].x=M_vec[i].x;
				S_vec[1].y=M_vec[i].y;
				S_vec[1].n=M_vec[i].n;

				cos_min=cos_buf;
			//	dout<<"cos_min" << cos_min <<eol;
			}
		}
	}
//	dout<<"Xnext1：" <<eol;
//	dout<<"Xn1："<<(int)S_vec[1].x <<" Yn1："<<(int)S_vec[1].y <<" nn1："<<(int)S_vec[1].n <<eol;

	M_point each_vec;			//外形の基準となるベクトル
	

	int i=0;
//	int flag_num=0;
	while(1)
	{
		each_vec.x = S_vec[i].x - S_vec[i+1].x;
		each_vec.y = S_vec[i].y - S_vec[i+1].y;
		cos_min=1.0f;				//cos_min 初期化
		search_number++;			//外形となる点をもうひとつ決める
		S_vec.resize(search_number);

		for(int j=0;j<point_number_calc;j++)
		{	
//			flag_num=0;
			if((j!=S_vec[i].n)&&(j!=S_vec[i+1].n))
			{
				each_buf_vec.x = M_vec[j].x - S_vec[i+1].x;
				each_buf_vec.y = M_vec[j].y - S_vec[i+1].y;
				//余弦を求める
				double cos_buf = ((each_vec.x*each_buf_vec.x)+(each_vec.y*each_buf_vec.y))
					/(sqrt(pow(each_buf_vec.x,2)+pow(each_buf_vec.y,2))*sqrt(pow(each_vec.x,2)+pow(each_vec.y,2)));
				//余弦最小値
				if(cos_buf<cos_min)
				{
					S_vec[i+2].x=M_vec[j].x;
					S_vec[i+2].y=M_vec[j].y;
					S_vec[i+2].n=M_vec[j].n;

					cos_min=cos_buf;
				}
//				flag_num++;
			}
		}
//	dout<<"Xnext："<< i <<eol;
//	dout<<"Xni："<<(int)S_vec[i].x <<" Yni："<<(int)S_vec[i].y <<" nni："<<(int)S_vec[i].n <<eol;
	i++;
//	if(flag_num==0)	break;
	if((S_vec[i+1].n==S_vec[0].n)&&(i!=0))	break;
	}


	// --------------------------------
	// データをvectorにから取り出す　 -
	// --------------------------------

	for(int j=0;j<S_vec.size()-1;j++)
	{
		Vec3f temp = Vec3f();
		temp.x = S_vec[j].x;
		temp.z = S_vec[j].y;
		supportVetxs.push_back(temp);
	}

}




}//	end namespace Spr

