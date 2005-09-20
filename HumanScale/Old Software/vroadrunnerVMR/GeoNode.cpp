#include "GeoNode.h"
#include "TGeoNodeVash.h"
#include "TGeoNodeNum.h"
#include "TGeoNodeSeesaw.h"
#include "TGeoNodeMoveFloor.h"
#include "TGeoNodeVashPoint.h"
#include "GLView.h"
#include "main.h"
#include <strstream>
using namespace LVRM;

///////////////////////////////////////////////////////////////
// Class Member Funcs

//----------------------------------------------------------------------------
//	TGeoNode
void TGeoNode::Draw(){
#ifdef _DEBUG
	if( strcmp( "Gengon02",this->Name().c_str())==0){
		static double i=0;
		//printf("TGeoNode:%s\n",this->Name().c_str());
		i+=0.2;
		//if(i==4000) i=0;
		Affinef r(Rad(i),'y'); // y軸周りに180度回転するアフィン行列
		Affinef t(10,0,0);
		solid->prev = solid->curr;
		solid->curr = Transform(r*t*posture);
		glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
		glMultMatrixf( r * t*posture );	// 位置・姿勢を変更する。
		//glMultMatrixf(posture);	// 位置・姿勢を変更する。
		glCallList(displayList);
		glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
	}
	else if( strcmp( "Gengon03",this->Name().c_str())==0){
		static double j=0;
		//printf("TGeoNode:%s\n",this->Name().c_str());
		j+=0.2;
		Affinef r(Rad(j),'y'); // y軸周りに180度回転するアフィン行列
		Affinef t(1,0,1);
		solid->prev = solid->curr;
		solid->curr = Transform(t*r*posture);
		glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
		glMultMatrixf(  t*r *posture );	// 位置・姿勢を変更する。
		glCallList(displayList);
		glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
	}
	else{ //default object
#endif //_DEBUG
		//Affinef r(Rad(0),'y'); // y軸周りに180度回転するアフィン行列

		glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
		glMultMatrixf( posture /* * r*/  );	// 位置・姿勢を変更する。
		glCallList(displayList);
		glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )

#ifdef _DEBUG
	}
#endif //_DEBUG
}

/**/
void TGeoNode::DrawGeometry(){
	glPushMatrix();	// 位置座標の退避 ( 1つ上の座標系を退避 )
	glMultMatrixf( posture );	// 位置・姿勢を変更する。
	for(TFaces::iterator itf = faces.begin(); itf != faces.end(); ++itf){
		glBegin(GL_POLYGON);
		for(vector<int>::iterator itv = itf->begin(); itv!= itf->end(); ++itv){
			glVertex3fv(vertices[*itv]);
		}
		glEnd();
	}
	glPopMatrix();	// 位置座標の復帰 ( 1つ上の座標系へ復帰 )
}


//----------------------------------------------------------------------------
//	TGeoNodeBuilder
TGeoNodeBuilder::TGeoNodeBuilder(){
	afStack.push_back(Affinef());
	bDontDisplay = false;
	countID = 0;
}
TGeoNode* TGeoNodeBuilder::CreateTGeoNode(TNode* n){
	TField* f = n->GetField("nodeID");
	string name;
	if (f){
		name = (*f->begin())->GetString();
	}
	//
	// vash のために変更
	TGeoNode* rv;
	if(name[2] == '_' && name[3] == 'c' && name[4] == '_'){
		//printf("test:%s\n",name.c_str());
		if(name[5] == 's'){
			rv = new TGeoNodeVash10;
		}
		else if(name[5] == 'm'){
			rv = new TGeoNodeVash50;
		}
		else if(name[5] == 'l'){
			rv = new TGeoNodeVash100;
		}
		else{
			rv = new TGeoNodeVash;
		}
	}
	else if(name[2] == '_' && name[3] == 'v'){
		rv = new TGeoNodeVashV;
	}
	else if(
		name[3] == 's' &&
		name[4] == 'e' &&
		name[5] == 'e' &&
		name[6] == 's' &&
		name[7] == 'a' &&
		name[8] == 'w'
		)
	{
		if(name[10] == 'x'){
			rv = new TGeoNodeSeesawZ;
			if(name[12] == '5'){
				((TGeoNodeSeesaw*)rv)->SetCenter(Vec3f(429.395, -54.562, 488.534));
			}
			else if(name[12] == '6'){
				((TGeoNodeSeesaw*)rv)->SetCenter(Vec3f(520.87,-54.562,585.6));
				((TGeoNodeSeesaw*)rv)->startState = -1;
			}
			else if(name[12] == '7'){
				((TGeoNodeSeesaw*)rv)->SetCenter(Vec3f(520.87,-54.562,647.736));
			}
			else if(name[12] == '8'){
				((TGeoNodeSeesaw*)rv)->SetCenter(Vec3f(584.667,-54.562,647.736));
			}
		}
		else if(name[10] == 'y'){
			rv = new TGeoNodeSeesawX;
			if(name[12] == '1'){// y01
				//cout << "dfasfjd;sadfjds;afjkdsafjkdsfkjdsafkjdsfkds;afkjsfdsafjdsadsf";
				((TGeoNodeSeesaw*)rv)->SetCenter(Vec3f(345.742, -54.562, 607.364));
			}
			else if(name[12] == '2'){
				((TGeoNodeSeesaw*)rv)->SetCenter(Vec3f(490.301,-54.562 ,617.868 ));
			}
			else if(name[12] == '3'){
				((TGeoNodeSeesaw*)rv)->SetCenter(Vec3f(314.143,-54.532 , 530.76));
			}
			else if(name[12] == '4'){
				((TGeoNodeSeesaw*)rv)->SetCenter(Vec3f(553.666, -54.532, 554.504));
			}
		}
		else{
			rv = new TGeoNodeSeesaw;
		}
	}
	/*	*/
	else if(strcmp(name.c_str(),"nc_10point") == 0){//nc_10point
		//cout << "test:nc_10point" << endl;
		rv = new TGeoNodeVashPoint;
		vroadrunner::p_VashPoint10 = (TGeoNodeVashPoint*)rv;
	}
	else if(strcmp(name.c_str(),"nc_30point") == 0){//nc_30point
		//cout << "test:nc_30point" << endl;
		rv = new TGeoNodeVashPoint;
		vroadrunner::p_VashPoint30 = (TGeoNodeVashPoint*)rv;
	}
	else if(strcmp(name.c_str(),"nc_100point") == 0){//nc_100point
		//cout << "test:nc_100point" << endl;
		rv = new TGeoNodeVashPoint;
		vroadrunner::p_VashPoint100 = (TGeoNodeVashPoint*)rv;
	}

#ifdef _USE_NUM3D //**** vrmlを使った時間表示（テストマップ GRAPHICS\garden_2.wrl)
	else if(name[2] == '_' && name[3] == 't'){
		TGeoNodeTimer* pNode;
		pNode = new TGeoNodeTimer();
		pNode->InitInstance(name);
		rv = pNode;
	}
//***** vrmlを使った点数表示（テストマップ GRAPHICS\garden_2.wrl)
	else if(name[2] == '_' && name[3] == 's'){
		TGeoNodeScore* pNode;
		pNode = new TGeoNodeScore();
		pNode->InitInstance(name);
		rv = pNode;
	}
#endif // _USE_NUM3D

//#ifdef _OKADA_TEST_MOVE //**** 移動床 *************//
	else if(name[2] == '_' && name[3] == 'm'){
		TGeoNodeMoveFloor* pNode;
		pNode = new TGeoNodeMoveFloor();
		pNode->InitInstance(name);
		rv = pNode;
	}
//#endif //_OKADA_TEST_MOVE

	else if(name[2] == '_' && name[3] == 'p' && name[4] == '_'){
		rv = new TGeoNodePointer;
		
	}
	else{
		rv = new TGeoNode;
	}
	//
	//
	rv->Name(name);
	rv->SetPosture(afStack.back());
	return rv;
}
void TGeoNodeBuilder::FoundTransformBefore(TNode *pn){
	afStack.push_back(afStack.back());
	Affinef& GetPosture = afStack.back();
	float		center[] = {0, 0, 0};
	float		translation[] = {0, 0, 0};
	float		rotation[] = {0, 0, 1, 0};
	float		scale[] = {1, 1, 1};
	float		scaleOrientation[] = {0, 0, 1, 0};
		
	CopyElm(pn, "center", center);
	CopyElm(pn, "translation", translation);
	CopyElm(pn, "rotation", rotation);
	CopyElm(pn, "scale", scale);
	CopyElm(pn, "scaleOrientation", scaleOrientation);
	
	GetPosture.Pos() += GetPosture.Rot() * (Vec3f&) translation;
	
	GetPosture.Pos() += GetPosture.Rot() * (Vec3f&) center;
	if(rotation[3]){
		Affinef rot(rotation[3], (Vec3f&)rotation);
		GetPosture = GetPosture * rot;
	}
	
	if(scaleOrientation[3]){
		Affinef rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
		GetPosture = GetPosture * rot;
	}
	if(scale[0] != 1 || scale[1] != 1 || scale[2] != 1){
		Affinef sc;
		sc.Ex() *= scale[0];
		sc.Ey() *= scale[1];
		sc.Ez() *= scale[2];
		GetPosture = GetPosture * sc;
	}
	if(scaleOrientation[3]){
		Affinef rot(scaleOrientation[3], (Vec3f&)scaleOrientation);
		GetPosture = GetPosture * rot;
	}
	GetPosture.Pos() -= GetPosture.Rot() * (Vec3f&) center;
}
void TGeoNodeBuilder::FoundTransformAfter(TNode *pn){
	afStack.pop_back();
}
void TGeoNodeBuilder::FoundNamed(TNode *pn){
	if (!pn->GetParent()
		|| strcmp(pn->GetType()->GetName(), "Transform")==0
		|| strcmp(pn->GetType()->GetName(), "Group")==0){
		FoundTransformBefore(pn);
	
		TGeoNode* node = CreateTGeoNode(pn);
		node->vrmlNode = pn;
		allNodes.push_back(node);
		pn->GetType()->Traverse(TType::EnumNamedNode, pn);

		FoundTransformAfter(pn);
	}else{
		pn->GetType()->Traverse(TType::EnumNamedNode, pn);
	}
}
bool TGeoNodeBuilder::FoundTypeNode(TNode *pn){
	if (strcmp(pn->GetType()->GetName(), "Background")==0){
		CopyElm(pn, "skyColor", (float*)&clearColor);
	}else if (strcmp(pn->GetType()->GetName(), "IndexedFaceSet")==0){
		vector<int> v;
		vector<float> p;
/*		// Xの最大値、最小値
		float maxX = -999;
		float minX = 999;
		// Zの最大値、最小値
		float maxZ = -999;
		float minZ = 999;*/
		TSts* pSts = 0;
		PushbackElm(pn, "coordIndex", v);
		if(CopyElm(pn, "coord", &pSts)) PushbackElm(*pSts->begin(), "point", p);
		if(v.size() == 0){
			cerr << "IndexedFaceSet has invalid data. skipping..." << endl;
			return false;				// error
		}
		if(v.back() >= 0) v.push_back(-1);
		int idBase = curNode->vertices.size();
		for(vector<float>::iterator it = p.begin(); it != p.end(); it+=3){
			Vec3f v;
			v[0] = it[0];
			v[1] = it[1];
			v[2] = it[2];
			v = afStack.back() * v;
			curNode->vertices.push_back(v);
	/*		if(maxX < v[0]){
				maxX = v[0];
			}
			if(minX > v[0]){
				minX = v[0];
			}
			if(maxZ < v[3]){
				maxZ = v[3];
			}
			if(minZ > v[3]){
				minZ = v[3];
			}*/
		}
		/*Affinef afGlobalPosture = curNode->GetGlobalPosture();
		curNode->VecObjectBox[0] = afGlobalPosture * Vec4f(minX, 0, maxZ, 1);
		curNode->VecObjectBox[1] = afGlobalPosture * Vec4f(minX, 0, minZ, 1);
		curNode->VecObjectBox[2] = afGlobalPosture * Vec4f(maxX, 0, minZ, 1);
		curNode->VecObjectBox[3] = afGlobalPosture * Vec4f(maxX, 0, maxZ, 1);
		for (int m = 0; m < 4; m++)
			cout << "VecObjectBox[" << m << "] = " << curNode->VecObjectBox[m] << endl;
		cout << endl;*/
		vector<int> oneFace;
		for(vector<int>::iterator it = v.begin(); it != v.end(); ++it){
			if (*it < 0){
				curNode->faces.push_back(oneFace);
				oneFace.clear();
			}else{
				oneFace.push_back(*it + idBase);
			}
		}
	}
	return true;
}
namespace LVRM{
	extern GLUquadricObj* qobj;
}
void TGeoNodeBuilder::DontDisplay(){
	bDontDisplay = true;
}

///////////////////////////////////////////////////////////////////////////////
//
void TGeoNodeBuilder::Build(TSts* sts){
	if (!sts) return;
	int t_count=0; //? test
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){

		printf("%d:%s\n",t_count,(*it)->typeName.c_str()); t_count++; //? test
		
		TNode* pn = (*it);
		FoundTransformBefore(pn); //

		TGeoNode* node = CreateTGeoNode(pn);

		node->vrmlNode = pn;
		allNodes.push_back(node);
		FoundTransformAfter(pn); //
	}
	printf("sts size:%d\n",t_count); //? test

#if 0 // 0ならば、コンパイルすらされない!!! -->
	TType::listener = this;
	//	名前の無いルートノードに名前を付けてしまう．
	int num=0;
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
		TField* f = (*it)->GetField("nodeID");
		if (!f){
			char name[1024];
			ostrstream(name, sizeof(name)) << "noname_root_" << num << '\0';
			TField f;
			UTRef<TFieldBase> str = new TFieldBaseString(name);
			f.push_back(str);
			(*it)->SetElm("nodeID", &f);
			num ++;
		}
		else{
			printf("hey!:%s\n",(*it)->typeName.c_str()); //Transformがプリントされる //? test
		}
	}
	printf("num:%d\n",num);

	//	名前付きTNodeからTGeoNodeを作る．
	t_count =0;
	for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
		printf("%d::%s\n",t_count,(*it)->typeName.c_str()); t_count++; //? test
		TNode* pn = *it;
		pn->GetType()->EnumNamedNode(pn);
	}

	//	見つけたTNodeを親から切り離す．
	t_count = 0;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		printf("%d:::%s\n",t_count,(*it)->name.c_str()); t_count++; //? test
		TNode* cur = (*it)->VrmlNode();
		TNode* parent = cur->GetParent();
		if (parent){
			printf("cur:%s parent:%s\n",cur->typeName.c_str(),parent->typeName.c_str());
			TField* field = parent->GetField("children");
			if (field){
				for(TField::iterator it = field->begin(); it != field->end(); ++it){
					TSts* sts = (*it)->GetSts();
					for(TSts::iterator it = sts->begin(); it != sts->end(); ++it){
						if (*it == cur){
							it = sts->erase(it);
						break;
						}
					}
				}
			}
		}
	}
	printf("見つけたTNodeを親から切り離す．:%d\n",t_count);
#endif // <-- 0ならば、コンパイルすらされない!!! 

	//	DrawListを作る．
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, GL_TRUE);
	static int nDisplayList = 0;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		///////////////////////////////////////////////////////////////////////
		glNewList(++nDisplayList, GL_COMPILE);
		TNode* node = (*it)->VrmlNode();
		if (!bDontDisplay){
			if (strcmp(node->GetType()->GetName(), "Transform") == 0){
				node->GetType()->Traverse(TType::Draw, node);
			}else{
				node->GetType()->Draw(node);
			}
		}
		glEndList();
		///////////////////////////////////////////////////////////////////////
		(*it)->displayList = nDisplayList;
	}
	gluDeleteQuadric(qobj);

	//	geometryを取り出す．
	TType::listener = this;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		curNode=*it;
		TNode* node = (*it)->VrmlNode();
		if (strcmp(node->GetType()->GetName(), "Transform") == 0){
			node->GetType()->Traverse(TType::EnumTypeNode, node);
		}else{
			node->GetType()->EnumTypeNode(node);
		}
	}
	
	///  バウンディングボックスの作成
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		// Xの最大値、最小値
		float maxX = -999;
		float minX = 999;
		// Zの最大値、最小値
		float maxZ = -999;
		float minZ = 999;
		// Yの最大値
		float maxY = -999;

		for(int i = 0; i < (*it)->vertices.size(); i++){
			if(maxX < (*it)->vertices[i].X())
				maxX = (*it)->vertices[i].X();
			if(minX > (*it)->vertices[i].X())
				minX = (*it)->vertices[i].X();
			if(maxZ < (*it)->vertices[i].Z())
				maxZ = (*it)->vertices[i].Z();
			if(minZ > (*it)->vertices[i].Z())
				minZ = (*it)->vertices[i].Z();
			if(maxY < (*it)->vertices[i].Y())
				maxY = (*it)->vertices[i].Y();
		}
		Affinef afGlobalPosture = (*it)->GetGlobalPosture();
		(*it)->VecObjectBox[0] = afGlobalPosture * Vec4f(minX, maxY, maxZ, 1);
		(*it)->VecObjectBox[1] = afGlobalPosture * Vec4f(minX, maxY, minZ, 1);
		(*it)->VecObjectBox[2] = afGlobalPosture * Vec4f(maxX, maxY, minZ, 1);
		(*it)->VecObjectBox[3] = afGlobalPosture * Vec4f(maxX, maxY, maxZ, 1);
		for (int m = 0; m < 4; m++)
			cout << "VecObjectBox[" << m << "] = " << (*it)->VecObjectBox[m] << endl;
		cout << endl;
		(*it)->objectID = countID;
		countID++;
	}
}

void TGeoNode::InitInstance()
{

}

