// SUD.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "GameManager.h"
#include "crtdbg.h"


// window���� �����ϴ� ����� �����, ���� ���� ����� �ֵ���ǥ. �����쿡�� �����ϴ� ���� .h���� �ȴ�
// ���� �� �� �� ������ Ŭ���ؼ� �ֵ� ��Ŭ��� �ص� �ȴ�

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(3615);
	CGameManager gameManager;
	//_crtBreakAlloc = 202;
	gameManager.Init();
	gameManager.Run(); //���� ������ �־��. 
	gameManager.Release();
	_CrtDumpMemoryLeaks();
	
	return 0;
}

