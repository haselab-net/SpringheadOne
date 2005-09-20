#include <Render/RDGlutView.h>
#include <GL/glut.h>
#include <HIS/HISpidar4anglequad.h>

using namespace LVRM;
class Glut_test : public RDGlutView{
//GLUT用に用意した関数
public:	
	HISpidar4anglequad *sp;

private:
	void OnIdle();
	void OnDraw();
	void OnSize(int width, int height);
	bool OnTimer();
	void OnOpen();
	float glut_X;
	float glut_Y;
	float glut_Z;
};