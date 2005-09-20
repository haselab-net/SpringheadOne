#include <Base/Env.h>
#include <Device/DRAdapio.h>
#include <Device/DRTokin3D.h>

#include <Solid/SOLID.h>
#include <VertexBase.h>
#include <Polyhedron.h>
#include <Object.h>

#include "GeoNode.h"
#include "GLView.h"

#include <GL/glut.h>
#include <crtdbg.h>
#include <strstream>
#include <io.h>
#include <penalty.h>
#include "communication.h"
#include "birdnet.h"
using namespace std;

#define KEY_ESC -27
const float floorHeight = 0.0f;
const float dt = 1.0f / 30.0f;
const float gravity = 9.8f;
const float elasticity = 0.8f;
//const float elasticity = 0.0f;
const float EYEOFFSET = 0.8f;
WBSockAddr msAddr;
struct BN_HEADER msHeader;
struct BN_HEADER recvHeader;
float posScale = 1.0f;

class CmdEye:public Command{
public:
	TGLView* view;
	CmdEye(TGLView* v):view(v),Command(&(v->cmdSocket)){}
	int Id()const{ return 1; }
	int Receive(char* data){
		Affinef& af = *(Affinef*)data;
		view->afBody = af;
		view->afBody.Pos().Y() -= EYEOFFSET;
		//		cout << af << endl;
		view->Invalidate();
		view->PrintPerformance();
		return sizeof(af);
	}
	void Send(){
		SendId();
//		cout << view->afBody << endl;
		Affinef af = view->afBody;
		af.Pos().Y() += EYEOFFSET;
		SendData(&af, sizeof(af));
		cmdSocket->Flush();
	}
};


bool TGLView::LoadSettings(){
	char* cname = getenv("COMPUTERNAME");
	char* p = cname;
	while(*p && !isdigit(*p)) p++;
	int cpNum = -1;
	if (strlen(p)) cpNum = atoi(p);
	if (cpNum == -1) return false;
	std::map<int,int> cpProj;
	cpProj[0]=0;
	cpProj[1]=1;
	cpProj[2]=2;
	cpProj[3]=3;
	cpProj[4]=0;
	cpProj[5]=1;
	cpProj[6]=2;
	cpProj[7]=3;

	cpProj[11]=8;
	cpProj[20]=9;
	cpProj[21]=10;
	cpProj[15]=11;
	cpProj[10]=12;
	cpProj[14]=13;
	cpProj[13]=14;
	cpProj[17]=15;
	
	cpProj[12]=16;
	cpProj[30]=17;
	cpProj[31]=18;
	cpProj[16]=19;
	cpProj[22]=9;
	cpProj[23]=10;
	cpProj[32]=17;
	cpProj[33]=18;

	int projNum = cpProj[cpNum];
	char filename[1024];
	ostrstream ostrfn(filename, sizeof(filename));
	ostrfn << "sf" << projNum << '\0';

	FILE *fp;
	int number;
	int set = 0;
	char ch[128];
    
    if((fp = fopen(filename, "r")) == NULL){
        cout << "Can not read or find file sf1" << endl;
 		return false;
   }
    fgets(ch,sizeof(ch),fp);

	std::istrstream cStrStrm(ch, sizeof(ch));

//	cout << ch <<endl;
	cStrStrm >> number >> setting[0] >> setting[1] >> setting[2] >> setting[3]
		>> setting[4] >> setting[5] >> setting[6];
//	cout << number << endl;
//        	for(int j=0;j<6;j++)
//	cout << setting[j] << endl;
    fclose(fp);
    return true;
}
TGLView::TGLView(bool b):bSender(b){
	lastTableAngle = 0.0f;
	bDraw = true;
	bDrawCollision = true;
	bDrawNormal = true;
	fov_v = 30.0f;
	//	アバターの設定
	radius = 0.5f;
	afBody = Affinef(0, 0, 10);
	afBody.LookAtGL(Vec3f(0, 0, 0), Vec3f(0,1,0));

	//	プロジェクタ位置データのロード
	if (LoadSettings()) SetParam();

	//	通信の初期化
	cmdSocket.Register(new CmdEye(this));
	if (bSender) cmdSocket.Init(10100, 10000);
	else cmdSocket.Init(10000, 10100);
	
	//	ターンテーブルの初期化
	bInitSuccess = false;
	if (bSender){
		dev.RPool().Register(new DRTokin3D);
		dev.RPool().Register(new DRAdapio(10));
		dev.Init();
		std::cout << dev;
		bInitSuccess = hiTurn.Init(dev);
		bInitSuccess = bInitSuccess && hiStep.Init(dev);
	}
}
void TGLView::SetParam(){
	double angleA,angleB,angleC;
	angleB = asin(setting[1]);
	angleA = asin(setting[0]/cos(angleB));
	angleC = setting[5]/180 * M_PI;
	Affinef Rzx,Rzy,Rxy;
	Rzx.Ex() = Vec3f(cos(angleA),0,-sin(angleA));
	Rzx.Ey() = Vec3f(0,1,0);
	Rzx.Ez() = Vec3f(sin(angleA),0,cos(angleA));
	Rzy.Ex() = Vec3f(1,0,0);
	Rzy.Ey() = Vec3f(0,cos(angleB),-sin(angleB));
	Rzy.Ez() = Vec3f(0,sin(angleB),cos(angleB));
	Rxy.Ex() = Vec3f(cos(angleC),sin(angleC),0);
	Rxy.Ey() = Vec3f(-sin(angleC),cos(angleC),0);
	Rxy.Ez() = Vec3f(0,0,1);
//	cout << angleB ;
	R = Rzx*Rzy*Rxy;
	fov_v = setting[3] * 2;
}
bool TGLView::OnTimer(){
	Step();
	return true;
}
void TGLView::OnIdle(){
	Sleep(10);
}
void TGLView::OnDraw(){
	glClearColor(clearColor.X(), clearColor.Y(), clearColor.Z(), 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	static Affinef R180(Rad(180),'y');
	
	Affinef afProjector = afBody * R180 * R * R180;
	
	Affinef afCam = (afProjector * afEye).inv();
	glLoadMatrixf(afCam);
//	glLoadMatrixf((afBody * afEye).inv());
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		(*it)->Draw();
//		(*it)->DrawGeometry();
	}
	if (bDrawCollision){
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		cpds.Draw();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	if (bDrawNormal){
		glEnd();
		glLoadIdentity();
		Vec3f pos(1,1,-8);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3f(1,0,0);
		for(int i=0; i<rnc.normals.size(); ++i){
			glBegin(GL_LINES);
			glVertex3fv(pos);
			glVertex3fv(pos + rnc.normals[i].unit());
			glEnd();
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	Flip();
}

void TGLView::OnSize(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov_v, (float)(width)/(float)(height) , 0.1f , 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void TGLView::OnJoystick(unsigned int button, int x, int y, int z){
	std::cout << "B" << button << " x:" << x << " y:" << y << " z:" << z << std::endl;
}

void TGLView::PrintPerformance(){
	static int first = 1;
	static int start, num_frames;
	int current;

	if ( first ){
		start = glutGet( GLUT_ELAPSED_TIME );
		num_frames = 0;
		first = 0;
	}
	num_frames++;
	current = glutGet( GLUT_ELAPSED_TIME );
	if ( current - start > 1000 ){
		double elapsed = 1e-3 * (double) ( current - start );
		double rate = (double) num_frames / elapsed;
		printf( "%5.1f fps\n", rate );
		num_frames = 0;
		start = current;
	}
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	switch(key){
	case '\x1b':
		Exit();
		break;
	case 'j':
		afBody.Pos().Y() += 10;

		break;
	case 'n':
		bDrawNormal = !bDrawNormal;
		break;
	case 'h':
		bDrawCollision = !bDrawCollision;
		break;
	case 'd':
		bDraw = !bDraw;
		break;
	case 'D':
		bDraw = false;
		break;
	case 'b':
		glutPostRedisplay();
		break;
	case 'f':
		glutFullScreen();
		break;
	case 'w':
		glutReshapeWindow(400,300);
		break;
	case 't':
	case 'c':
		hiTurn.Reset();
		break;
	case 's':
		hiTurn.Stop();
		break;
	case 'S':
		hiTurn.Start();
		break;
	default:
	break;
	}
}

void TGLView::OnMouse(Button button, bool on, int x, int y)
{
	mouseButton = button;
	mouseX = x;	mouseY = y;
	if(!on){
		mouseButton = NONE;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case LEFT:
		if(on)
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	case MIDDLE:
		if(on)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	}
}

void TGLView::OnMouseMove(bool on, int x, int y)
{
	switch(mouseButton){
	case LEFT:
		Invalidate();
		break;		
	case MIDDLE:
		Invalidate();
		break;
	case RIGHT:
		Invalidate();
		break;
	}
	mouseX = x;	mouseY = y;
}
void TGLView::OnOpen(void){

	//	GLの初期化
	float	lmamb[] = {.7, .7, .7, 1},
			l0amb[] = {0, 0, 0, 1},
			l0dif[] = {1, 1, 1, 1},
			l0spc[] = {1, 1, 1, 1},
			l0pos[] = {0, 0, 10, 0};

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmamb);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
	if (!bSender) glutFullScreen();

	SetTimer(33, &TGLView::OnTimer);
	bDrawNormal = false;

	//	MotionStar
	msAddr.Set("192.168.3.250", 5000);

	msHeader.xtype = 0;
	msHeader.errorCode = 0;
	msHeader.protocol = 3;
	msHeader.numBytes = 0;

	msHeader.sequence = htons(ntohs(msHeader.sequence)+1);
	msHeader.type = MSG_WAKE_UP;
	msSock.SendTo(&msHeader, sizeof(msHeader), msAddr);
	msSock.RecvFrom(&recvHeader, sizeof(recvHeader), msAddr);
/*
	msHeader.sequence = htons(ntohs(msHeader.sequence)+1);
	msHeader.type = MSG_GET_STATUS;
	msSock.SendTo(&msHeader, sizeof(msHeader), msAddr);
	char buf[1024*10];
	msSock.RecvFrom(&buf, sizeof(buf), msAddr);
	recvHeader = *(BN_HEADER*)buf;
	char* data = buf + sizeof(recvHeader);
	BN_SYSTEM_STATUS sys = *(BN_SYSTEM_STATUS*)data;
	data = data + sizeof(BN_SYSTEM_STATUS);
	
	for(int i=0; i<sys.chassisDevices; ++i){
		msHeader.sequence = htons(ntohs(msHeader.sequence)+1);
		msHeader.type = MSG_GET_STATUS;
		msHeader.xtype = i+1;
		msSock.SendTo(&msHeader, sizeof(msHeader), msAddr);
		char buf[1024*10];
		msSock.RecvFrom(&buf, sizeof(buf), msAddr);
		recvHeader = *(BN_HEADER*)buf;
		char* data = buf + sizeof(recvHeader);
		BN_BIRD_HEADER bird = *(BN_BIRD_HEADER*) buf;
		if (bird.model == 4){
			posScale = (float) ntohs(bird.scaling) / 32767.;
			msHeader.sequence = htons(ntohs(msHeader.sequence)+1);
			msHeader.type = MSG_SEND_SETUP;
			msHeader.xtype = i+1;
			char buf[1024*10];
			char* data = buf;
			*(BN_HEADER*)data = msHeader;	data+= sizeof(msHeader);
			bird.dataFormat = BN_POSMAT | (12 << 4);
			*(BN_BIRD_HEADER*)data = bird;	data += sizeof(bird);
			msSock.SendTo(&data, data - buf, msAddr);
			msSock.RecvFrom(&buf, sizeof(buf), msAddr);
		}
	}
	*/
	msHeader.sequence = htons(ntohs(msHeader.sequence)+1);
	msHeader.type = MSG_RUN_CONTINUOUS;
	msSock.SendTo(&msHeader, sizeof(msHeader), msAddr);
	msSock.RecvFrom(&recvHeader, sizeof(recvHeader), msAddr);

}

typedef map<DtObjectRef, Object *> ObjectList;
extern ObjectList objectList;
void TGLView::InitCollisionDetector(){
	//	SOLIDの初期化
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		TGeoNode::TVertices& vtxs = (*it)->vertices;
		if (!(*it)->faces.size()) continue;
		
		DtShapeRef shape = dtNewComplexShape();
		for(int i=0; i<(*it)->faces.size(); ++i){
			TGeoNode::TFace& face = (*it)->faces[i];
			dtBegin(DT_POLYGON);
			for(int v=0; v<face.size(); ++v){
				dtVertex(vtxs[face[v]].X(), vtxs[face[v]].Y(), vtxs[face[v]].Z());
			}
			dtEnd();
		}
		dtEndComplexShape();
		dtCreateObject(*it, shape); 
		dtLoadMatrixf((*it)->GetPosture());
	}

	//	チェックすべきオブジェクトの追加
	for (ObjectList::const_iterator i = objectList.begin(); i != objectList.end(); ++i) {
		if (i->first == &afBody) continue;
		TClosestPointDetector cpd(i->second);
		cpds.push_back(cpd);
	}
}

void TGLView::Step(){
	bool b = false;
	while(cmdSocket.Receive()) b = true;
	if (bSender && !b){
		CmdEye cmdEye(this);
		cmdEye.Send();
	}
	static int stopFileDetectCounter;
	stopFileDetectCounter ++;
	if (stopFileDetectCounter > 10){
		if (_access("stop", 00) != -1){
			Exit();
		}
		stopFileDetectCounter = 0;
	}
	while (msSock.CanRecv()){
		char buf[1024];
		int readLen = msSock.RecvFrom(&buf, sizeof(buf), msAddr);
		if (readLen>0){
			short* data = (short*) (buf+sizeof(msHeader));
			data ++;
			Vec3f pos;
			for(int i=0; i<3; ++i){
				pos[i] = (short)ntohs(*data) * posScale / 1000.0f;
				data ++;
			}
			const float angScale = M_PI / 32767.;
			Vec3f angle;
			for(int i=0; i<3; ++i){
				angle[i] = (short)ntohs(*data) * angScale;
				data ++;
			}
//			afBird.Rot() = Matrix3f(-angle[0], 'y') * Matrix3f(angle[1], 'x')
//				* Matrix3f(-angle[2], 'z');
			afBird.Rot() = Matrix3f(-angle[0], 'y');
			float s = 1.1f;
//			float s = 10.0f;
			afBird.Pos().X() = pos.Y() * s;
			afBird.Pos().Y() = pos.Z() * s;
			afBird.Pos().Z() = -pos.X() * s;
			static printCount;
			printCount ++;
			if (printCount > 20){
				DSTR.setf(std::ios::dec);
				DSTR << afBird;
			}
			static bool first = true;
			static Affinef afBirdFirst;
			if (first){
				first = false;
				afBirdFirst = afBird;
			}else{
				afBird = afBirdFirst.inv() * afBird;
				afBird.Pos().Y() -= 10.6f;
			}
		}
	}
	afBody = afBird.inv();
	if (bDraw){
		Invalidate();			//	描画要求
	}
}