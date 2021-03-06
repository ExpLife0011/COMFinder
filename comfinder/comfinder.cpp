// comfinder.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

typedef HRESULT(__stdcall *CREATEFUNC)(REFCLSID clsid, REFIID iid, void** ppobj);

int main(int argc, char * argv[])
{
	CLSID clsid;
	IID iid;
	HRESULT hr;
	CComQIPtr<IClassFactory>  cls;
#ifdef _WIN64

	PULONGLONG  *instance;
#else

	PULONG  *instance;

#endif
	
	HMODULE base = 0;
	CoInitialize(NULL);
	base = LoadLibraryA(argv[1]);
	if (base == 0) {
		return 1;
	}
	CREATEFUNC fun = (CREATEFUNC)GetProcAddress(base, "DllGetClassObject");
	if (fun == NULL) {
		return 2;
	}
	CLSIDFromString(CComBSTR(argv[2]), &clsid);
	IIDFromString(CComBSTR(argv[3]), &iid);

	hr = fun(clsid, IID_IClassFactory, (PVOID *)&cls);


	if (hr != S_OK) {
		return 3;

	}
	hr = cls->CreateInstance(NULL, iid, (PVOID *)&instance);
	if (hr != S_OK) {
		return 4;
	}
	for (int i = 0;i < atoi(argv[4]);i++) {
#ifdef _WIN64

		printf("%016llX\n", instance[0][i] - (ULONGLONG)base);
#else

		printf("%08X\n", instance[0][i] - (ULONG)base);

#endif
		
	}
	CoUninitialize();


	return 0;
}

