#ifndef LOADBMP_H
#define LOADBMP_H
extern "C"{
int __declspec(dllexport) LoadBmpCreate(const char* fn);
int __declspec(dllexport) LoadBmpRelease(int hBmp);
int __declspec(dllexport) LoadBmpGetBmp(int hBmp, char* bmp);
int __declspec(dllexport) LoadBmpGetWidth(int hBmp);
int __declspec(dllexport) LoadBmpGetHeight(int hBmp);
int __declspec(dllexport) LoadBmpGetBytePerPixel(int hBmp);
int __declspec(dllexport) LoadBmpGetSize(int hBmp);
}
#endif
