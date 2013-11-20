 // SUD.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "GameManager.h"
 
// window에서 제공하는 헤더는 꺽쇄로, 내가 만든 헤더는 쌍따옴표. 윈도우에서 제공하는 것은 .h빼도 된다
// 빨간 줄 뜬 거 오른쪽 클릭해서 애드 인클루드 해도 된다

int _tmain(int argc, _TCHAR* argv[])
{
	CGameManager gameManager;

	gameManager.Init();
	gameManager.Run(); //무한 루프가 있어야. 
	gameManager.Release();

	//getchar();
	return 0;
}

