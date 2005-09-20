#include <HIS/HIStepWalk.h>
#include <HIS/HITurnTableSmall.h>
#include <Device/DVDeviceManager.h>
#include <Device/DVDaBase.h>
#include <Device/DVOrientationBase.h>

using namespace LVRM;

namespace LVRM{;

bool InitTurnTable();
void KeyEvent();

DVDeviceManager dev;
HIStepWalk hiStep;
HITurnTableSmall hiTurn;

bool bRun;
bool bStep, bKeyEvent;
};