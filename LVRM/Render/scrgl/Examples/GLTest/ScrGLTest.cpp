//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("ScrGLTest.res");
USEFORM("Unit1.cpp", Form1);
USELIB("..\..\..\..\VRBCB.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TForm1), &Form1);
		Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
