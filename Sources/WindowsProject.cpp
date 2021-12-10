// WindowsProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

// Set console enable for debug
#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#include <iostream>
#include <iomanip>
#endif

#include "../Headers/framework.h"
#include "../Headers/WindowsProject.h"
#include <string>
#include <vector>
#include <cmath>

#include "../Headers/Snake.h"
#include "../Headers/ItemGenerator.h"

#define MAX_LOADSTRING 100
#define BSIZE 40

// 전역 변수:
HINSTANCE hInst;											// 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING]{_T("Tick-Tick! Snake Game")};  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];						// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 전역 문자열을 초기화합니다.
	//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	
	LoadStringW(hInstance, IDC_WINDOWSPROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT));

	MSG msg{ 0 };

	// 기본 메시지 루프입니다:
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // 메세지 해석
			DispatchMessage(&msg);  // 준비 큐에서 실행 상태로 메세지 보냄(디스패치)
		}
	}

	return (int)msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	// WNDCLASSEXW : 윈도우 클래스 타입, EX : Executive(관리자)
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);      // 윈도우 클래스 구조체 크기

	wcex.style = CS_HREDRAW | CS_VREDRAW;  // 윈도우 출력 스타일(형태)
	wcex.lpfnWndProc = WndProc;            // 참고: proc : procedure
	wcex.cbClsExtra = 0;                   // 클래스를 위한 여분 메모리 크기
	wcex.cbWndExtra = 0;                   // 윈도우를 위한 여분 메모리 크기
	wcex.hInstance = hInstance;            // 해당 Application의 인스턴스(식별번호)
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT));	// 기본 아이콘
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);                          // 기본 커서
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);						// 윈도우 배경색
																			// COLOR_WINDOW : 회색
																			// +1 : 흰색
																			// +2 : 짙은 회색
																			// +3 : 검은색

	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT);               // 메뉴 이름
	wcex.lpszClassName = szWindowClass;                                     // 클래스 이름
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));    // 작은 아이콘(Icon Small)

	return RegisterClassExW(&wcex); // RegisterClasExW 함수를 이용해 커널에 해당 윈도우 클래스를 등록, 클래스 변수 주소를 넘기면 됨
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		0,
		640,
		480,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM) - wparam : word param(16bit), lparam : long param(32bit)
//
//  용도: 주 창의 메시지를 처리합니다.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rectView;
	static RECT rectMenu{ 260, 180, 360, 280 };
	static bool isGameRan = false;
	static bool isGameOver = false;
	static bool isGamePaused = false;
	static bool canAcceptMoveInput = true;
	static Snake snake(280, 220, Direction::LEFT);
	static ItemGenerator generator;
	static Pos itemPos;
	static float deltaTime = 200;

	switch (message)
	{
	case WM_CREATE:
		{
			GetClientRect(hWnd, &rectView);
#ifdef _DEBUG
			std::cout
				<< "WINDOW::SCALE:: "
				<< "left: " << rectView.left
				<< " "
				<< "right: " << rectView.right
				<< " "
				<< "top: " << rectView.top
				<< " "
				<< "bottom: " << rectView.bottom
				<< std::endl;
#endif
			int borderX = (rectView.right - 20) / 10 * 10;
			int borderY = (rectView.bottom - 20) / 10 * 10;

			snake.setBorder(borderX, borderY);
			generator.setBorder(borderX, borderY);
			itemPos = generator.getItemPos();
		}
		break;
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 메뉴 선택을 구문 분석합니다:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkColor(hdc, RGB(0, 0, 0));

			if (!isGameRan)
			{
				DrawText(hdc,
						 _T("PRESS ENTER\nTO GAME START"),
						 _tcslen(_T("PRESS ENTER\nTO GAME START")),
						 &rectMenu,
						 DT_CENTER | DT_VCENTER | DT_NOCLIP);
			}
			else if (isGamePaused)
			{
				DrawText(hdc,
						 _T("GAME PAUSED\n\nPRESS ENTER\nTO GAME START"),
						 _tcslen(_T("GAME PAUSED\n\nPRESS ENTER\nTO GAME START")),
						 &rectMenu,
						 DT_CENTER | DT_VCENTER | DT_NOCLIP
				);
			}
			else if (isGameOver)
			{
				DrawText(hdc,
						 _T("GAME OVER\nPRESS ENTER TO RESTART\nOR\nPRESS Q TO QUIT"),
						 _tcslen(_T("GAME OVER\nPRESS ENTER TO RESTART\nOR\nPRESS Q TO QUIT")),
						 &rectMenu,
						 DT_CENTER | DT_VCENTER | DT_NOCLIP
				);
			}
			else
			{
				for (int i = 0; i < snake.length(); i++)
				{
					TextOut(hdc, snake[i].x, snake[i].y, _T("■"), _tcslen(_T("■")));
				}
				TextOut(hdc, itemPos.x, itemPos.y, _T("■"), _tcslen(_T("■")));
				canAcceptMoveInput = true;
			}

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_KEYDOWN:
		{
			if (wParam == VK_RETURN)
			{
				if (!isGameOver)
				{
					if (!isGameRan)
					{
						isGameRan = true;
						SetTimer(hWnd, 1, deltaTime, nullptr);
					}
					else if (!isGamePaused)
					{
						KillTimer(hWnd, 1);
						isGamePaused = true;
					}
					else
					{
						isGamePaused = false;
						SetTimer(hWnd, 1, deltaTime, nullptr);
					}
				}
				else
				{
					isGameOver = false;
					isGameRan = true;
					isGamePaused = false;

					snake.init(280, 220);
					deltaTime = 200;
					itemPos = generator.getItemPos();

					SetTimer(hWnd, 1, deltaTime, nullptr);
				}
				InvalidateRgn(hWnd, nullptr, true);
			}

			if (wParam == 'Q' && isGameOver)
			{
				PostQuitMessage(0);
			}

			if (canAcceptMoveInput)
			{
				canAcceptMoveInput = false;
				if (wParam == VK_UP)
				{
					if (snake.dir() == Direction::LEFT) snake.dir()++;
					else if (snake.dir() == Direction::RIGHT) snake.dir()--;
				}
				else if (wParam == VK_DOWN)
				{
					if (snake.dir() == Direction::LEFT) snake.dir()--;
					else if (snake.dir() == Direction::RIGHT) snake.dir()++;
				}
				else if (wParam == VK_LEFT)
				{
					if (snake.dir() == Direction::UP) snake.dir()--;
					else if (snake.dir() == Direction::DOWN) snake.dir()++;
				}
				else if (wParam == VK_RIGHT)
				{
					if (snake.dir() == Direction::UP) snake.dir()++;
					else if (snake.dir() == Direction::DOWN) snake.dir()--;
				}
			}
		}
		break;
	case WM_TIMER:
		{
			if (!snake.move())
			{
				isGameOver = true;
				KillTimer(hWnd, 1);
			}
			if (snake.hasGotItem(itemPos))
			{
				snake.addTail();
				itemPos = generator.getItemPos();
				deltaTime *= 0.95f;
				KillTimer(hWnd, 1);
				SetTimer(hWnd, 1, deltaTime, nullptr);
			}

#ifdef _DEBUG
			std::cout
				<< "LENGTH::" << (int)snake.length()
				<< " "
				<< "ITEM_POS::(" << itemPos.x << ", " << itemPos.y << ")"
				<< " "
				<< std::fixed << std::setprecision(1)
				<< "FPS::" << (1000.0f / deltaTime) << " / "
				<< std::setprecision(0)
				<< (200 / deltaTime) * 100 << "%"
				<< std::endl;
#endif
			InvalidateRgn(hWnd, nullptr, true);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
