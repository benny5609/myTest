// bennyTest.cpp : CbennyTest 的实现
#include "stdafx.h"
#include "bennyTest.h"


// CbennyTest
LRESULT CbennyTest::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	char _hwnd[100];
	sprintf(_hwnd, "%d", int(m_hWnd));
	SetCurrentDirectoryA("E:\\study\\ogre_171\\Ogre_sln\\bin\\debug\\");
	ShellExecuteA(NULL, "open", "E:\\study\\ogre_171\\Ogre_sln\\bin\\debug\\TinyOgre.exe", _hwnd, NULL, SW_SHOW);
	return 0;
}

LRESULT CbennyTest::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	return 0;
}