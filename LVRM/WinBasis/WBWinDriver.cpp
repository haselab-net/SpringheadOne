#include "WBWinDriver.h"
#include "WBWin.h"
namespace LVRM {
//---------------------------------------------------------------
//	WBLoadWindowsDriver

WBLoadWindowsDriver::WBLoadWindowsDriver(){
	hSCMan = NULL;
	hService = NULL;
}
WBLoadWindowsDriver::~WBLoadWindowsDriver(){
	Cleanup();
}
void WBLoadWindowsDriver::Path(const char* p){
	path.Path(p);
	drvName = path.Main();
}
void WBLoadWindowsDriver::File(const char* f){
	if (path.Search(f)){
		Path(path.Path());
	}
}

void WBLoadWindowsDriver::Init(){
	hSCMan = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCMan){
		DSTR << "Can't connect to service control manager" << std::endl;
	}
}
void WBLoadWindowsDriver::Cleanup(){
	if (hSCMan){
		CloseServiceHandle(hSCMan);
		hSCMan = NULL;
	}
}
void WBLoadWindowsDriver::Install(){
	Init();
	// add to service control manager's database
	hService = CreateService(hSCMan, drvName.c_str(), 
      drvName.c_str(), SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
      SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, path.Path(), 
      NULL, NULL, NULL, NULL, NULL);
	if (!hService){
         DWORD dwStatus = GetLastError();
		 DSTR << "Cannot create service from \'" << path.Path();
		 DSTR << "\' . Error#=" << dwStatus << std::endl;
	}
}
void WBLoadWindowsDriver::Open(){
	if (hService) return;
	Init();
	hService = OpenService(hSCMan, drvName.c_str(), SERVICE_ALL_ACCESS);
}
void WBLoadWindowsDriver::Start(){
	Open();
	if (!StartService(hService, 0, NULL)){
         DWORD dwStatus = GetLastError();
		 DSTR << "Cannot start service \'" << drvName.c_str()
			 << "\' . Error#=" << dwStatus << std::endl;
	}
}
void WBLoadWindowsDriver::Stop(){
	Open();
	SERVICE_STATUS serviceStatus;
	BOOL rv = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);
	if (!rv){
         DWORD dwStatus = GetLastError();
		 DSTR << "Cannot stop service \'" << drvName.c_str()
			 << "\' . Error#=" << dwStatus << std::endl;
	}
}
void WBLoadWindowsDriver::Remove(){
	Open();
	if (!DeleteService(hService)){
         DWORD dwStatus = GetLastError();
		 DSTR << "Cannot remove service \'" << drvName.c_str()
			 << "\' . Error#=" << dwStatus << std::endl;
	}
}
bool WBLoadWindowsDriver::IsOpen(){
	return hService != NULL;
}
bool WBLoadWindowsDriver::IsStart(){
	if (!IsOpen() ) Open();
	  // handle of service
	SERVICE_STATUS st;
	if (QueryServiceStatus(hService, &st)){
		return st.dwCurrentState == SERVICE_RUNNING;
	}
	return false;
}

}	//	namespace LVRM
