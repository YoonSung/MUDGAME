 // SUD.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "GameManager.h"
#include "crtdbg.h"
#ifdef  _DEBUG
#define new new(_CLIENT_BLOCK,__FILE__, __LINE__)
#endif //  _DEBUG


// window���� �����ϴ� ����� �����, ���� ���� ����� �ֵ���ǥ. �����쿡�� �����ϴ� ���� .h���� �ȴ�
// ���� �� �� �� ������ Ŭ���ؼ� �ֵ� ��Ŭ��� �ص� �ȴ�

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 214;

	CGameManager gameManager;
	gameManager.Init();
	gameManager.Run(); //���� ������ �־��. 
	gameManager.Release();

	_CrtDumpMemoryLeaks();
	//getchar();
	return 0;
}

