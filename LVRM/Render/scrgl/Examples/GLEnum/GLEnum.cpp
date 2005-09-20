//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("GLEnum.res");
USEFORM("main.cpp", MainForm);
USEFORM("prof.cpp", ProgressForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        Application->Initialize();
        Application->CreateForm(__classid(TMainForm), &MainForm);
        Application->CreateForm(__classid(TProgressForm), &ProgressForm);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------

