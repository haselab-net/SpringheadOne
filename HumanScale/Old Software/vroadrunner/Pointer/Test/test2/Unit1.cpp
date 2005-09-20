//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	pGraph = NULL;
	pMediaControl = NULL;
	pVidWin = NULL;
}
//---------------------------------------------------------------------------

void TForm1::PlayStart()
{
	wchar_t filename[256];
	HRESULT hr;

	if( OpenDialog1->Execute() ){
		OpenDialog1->FileName.WideChar( filename,256 );

		// �t�B���^�O���t�}�l�[�W���̍쐬
		//   IGraphBuilder�ɂ́A�t�B���^�����ׂč폜�Ƃ����֐��͂Ȃ��悤�Ȃ̂ŁA
		//   ���ׂď�������ɐV�K�ɍ��B
		//   �������Ȃ��Ɖߋ���RenderFile�����f���������ɕʃE�B���h�E�ŗ����B
		CoCreateInstance( CLSID_FilterGraph,NULL,CLSCTX_INPROC,
			IID_IGraphBuilder,(void**)&pGraph );
		if( !pGraph ) Application->MessageBox( "pGraph is NULL ","",MB_OK );
		// IMediaControl�͍Đ��E��~�ɗ��p�B
		pGraph->QueryInterface( IID_IMediaControl,(void**)&pMediaControl );
		// IVideoWindow�̓r�f�I�\����ʂ̐ݒ�ɗ��p�B
		pGraph->QueryInterface( IID_IVideoWindow,(void**)&pVidWin );
		// IBasicVideo�̓r�f�I�̏c���̑傫���𓾂�̂ɗ��p�B
		pGraph->QueryInterface( IID_IBasicVideo,(void**)&pBasicVideo );

		// �O���t���\�z����
		pGraph->RenderFile( filename,NULL );

		// �I�[�f�B�I�݂̂̃t�@�C�����𔻒f
		if( !IsAudioOnly() ){
			// �r�f�I�E�B���h�E���\�z����
			if( S_OK != pVidWin->put_Owner( (OAHWND)Handle ) ){
				Application->MessageBox("put_Owner failed","",MB_OK );
			}
			if( S_OK != pVidWin->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS ) )
			{
				Application->MessageBox( "put_WindowStyle failed","",MB_OK );
			}
			if( S_OK != pVidWin->put_Visible( OATRUE ) ){
				Application->MessageBox( "put_Visible failed","",MB_OK );
			}

			// �r�f�I�̑傫���𓾂āA�E�B���h�E�T�C�Y��ύX
			long width,height;
			pBasicVideo->get_SourceWidth( &width );
			pBasicVideo->get_SourceHeight( &height );
			Width = (width < Screen->Width) ? width : Screen->Width;
			Height = (height < Screen->Height) ? height : Screen->Height;
			Left = ( Screen->Width - Width ) / 2;
			Top = ( Screen->Height - Height ) / 2;

			// �|�W�V�������Z�b�g
			pVidWin->SetWindowPosition( 0,0,ClientWidth,ClientHeight );
		}

		// �O���t�����s����
		pMediaControl->Run();
	}
}

void TForm1::PlayStop()
{
	if( pGraph ){
		// �Đ��̒�~
		pMediaControl->Stop();
		pVidWin->put_Visible(OAFALSE); //�Y���Ɖ�ʕ`�悪���������Ȃ�
		pVidWin->put_Owner(NULL); //�Y���ƃE�B���h�E�Ƀ��b�Z�[�W�����葱������

		// �C���^�[�t�F�C�X���J��
		KT_RELEASE( pBasicVideo );
		KT_RELEASE( pVidWin );
		KT_RELEASE( pMediaControl );
		KT_RELEASE( pGraph );
	}
}

// �t�@�C�����I�[�f�B�I�݂̂����`�F�b�N
// playwnd�T���v����void CheckVisibility()���Q�l�ɂ����B
bool TForm1::IsAudioOnly()
{
	// �I�[�f�B�I�݂̂̃t�@�C����ǂݍ��񂾂Ƃ���
	// VideoWindow�C���^�[�t�F�C�X�����݂��Ȃ����Ƃ�������ۂ��B
	if( !pVidWin ) return true;

	// VideoWindow�C���^�[�t�F�C�X�����݂���Ƃ��ł�
	// �I�[�f�B�I�݂̂̃t�@�C���̓r�f�I�����_�����ڑ�����Ȃ��̂�
	// �����o�֐����Ă�ł� VFW_E_NOT_CONNECTED ��Ԃ��B
	// (�ǋL)E_NOINTERFACE ��Ԃ����Ƃ�����悤���B
	// �����ł͓K����get_Width�֐����Ă�ł݂�B
	long temp;
	HRESULT hr = pVidWin->get_Width( &temp );
	return hr == VFW_E_NOT_CONNECTED || hr == E_NOINTERFACE;

/*	// �{���͂��̂悤�ɃG���[���e���`�F�b�N���������ǂ��Ǝv���B
	hr = pVidWin->get_Visible( &temp );
	if( hr == S_OK )
		Application->MessageBox( "S_OK","",MB_OK );
	else if( hr == VFW_E_NOT_CONNECTED )
		Application->MessageBox( "VFW_E_NOT_CONNECTED","",MB_OK );
	else if( hr == E_NOINTERFACE )
		Application->MessageBox( "E_NOINTERFACE","",MB_OK );
	else
		Application->MessageBox(
			(AnsiString("Unknown Error:")+AnsiString(hr)).c_str(),"",MB_OK );

	//return VFW_E_NOT_CONNECTED == pVidWin->get_Width( &temp );
	return S_OK != pVidWin->get_Width( &temp );
*/
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	CoInitialize( NULL );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	CoUninitialize();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenO1Click(TObject *Sender)
{
	PlayStop();
	PlayStart();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StopS1Click(TObject *Sender)
{
	PlayStop();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	PlayStop();
}
//---------------------------------------------------------------------------

