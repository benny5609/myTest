// dllmain.cpp : DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "atl_plugin7_i.h"
#include "dllmain.h"
#include "compreg.h"
#include "dlldatax.h"

Catl_plugin7Module _AtlModule;

class Catl_plugin7App : public CWinApp
{
public:

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(Catl_plugin7App, CWinApp)
END_MESSAGE_MAP()

Catl_plugin7App theApp;

BOOL Catl_plugin7App::InitInstance()
{
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
#endif
	return CWinApp::InitInstance();
}

int Catl_plugin7App::ExitInstance()
{
	return CWinApp::ExitInstance();
}
