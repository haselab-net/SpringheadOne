#include "WBHid.h"
namespace Spr{;

void WBHidDevice::Print(std::ostream& os) const{
	os << "VenderID:" << attr.VendorID << " ProductID:" << attr.ProductID;
	os << " Version:" << attr.VersionNumber;
	os << " Path:" << data->DevicePath << std::endl;
}

UTString WBHidDevice::GetError(){
	DWORD err = GetLastError();
	char errMsg[1024];
	FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, errMsg, sizeof(errMsg), NULL);
	std::cout << errMsg << std::endl;
	return errMsg;
}
HANDLE WBHidDevice::Create(DWORD dwDesiredAccess, DWORD dwShareMode){
	ZeroMemory(&attr, sizeof(attr));
	attr.Size = sizeof(attr);

	HANDLE h = CreateFile(data->DevicePath, dwDesiredAccess, dwShareMode,
		NULL, OPEN_EXISTING, 0, NULL);
	if ((int) h == EOF){
		GetError();
		return h;
	}
	if (!HidD_GetAttributes(h, &attr)){
		GetError();
		return h;
	}
	void* pdata;
	if (!HidD_GetPreparsedData(h, (PHIDP_PREPARSED_DATA*) &pdata)){
		GetError();
		return h;
	}
	HidP_GetCaps((PHIDP_PREPARSED_DATA)pdata, &caps);
	return h;
}
void WBHid::Init(){
	devices.clear();
	HidD_GetHidGuid(&guidHid);
	std::cout << "HidClass: " << guidHid << std::endl;
	hDevInfo = SetupDiGetClassDevs(&guidHid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
	devInfoData;
	ZeroMemory(&devInfoData, sizeof(devInfoData));
	devInfoData.cbSize = sizeof(devInfoData);
}

void WBHid::Enum(){
	for(int i=0;; ++i){
		if (SetupDiEnumDeviceInterfaces (hDevInfo, 0, &guidHid, i, &devInfoData)){
			std::cout << "InterfaceClass: " << devInfoData.InterfaceClassGuid << std::endl;
			DWORD length=0;
			SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, NULL, 0, &length, NULL);
			SP_DEVICE_INTERFACE_DETAIL_DATA* detail = (SP_DEVICE_INTERFACE_DETAIL_DATA*) new char [length];
			ZeroMemory(detail, length);
			DWORD req=0;
			detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			bool rv = (SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, detail, length, &req, NULL) == TRUE);
			if (rv){
				devices.push_back(new WBHidDevice(detail));
				std::cout << detail->DevicePath << std::endl;
			}else{
				DWORD err = GetLastError();
				char errMsg[1024];
				FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, errMsg, sizeof(errMsg), NULL);
				std::cout << errMsg << std::endl;
			}
		}else{
			DWORD err = GetLastError();
			char errMsg[1024];
			FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, errMsg, sizeof(errMsg), NULL);
			std::cout << errMsg << std::endl;
			break;
		}
	}
}

}

