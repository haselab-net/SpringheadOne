#include <Render/RDGlutView.h>
#include <GL/glut.h>
#include <HIS/HISpidar4angleaxisquad.h>

using namespace LVRM;
class Glut_test : public RDGlutView{
//GLUTópÇ…ópà”ÇµÇΩä÷êî
public:	
	HISpidar4angleaxisquad *sp;

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