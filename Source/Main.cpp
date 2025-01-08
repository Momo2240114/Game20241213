#include <windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>

#include "Framework.h"

const LONG SCREEN_WIDTH = 1920;
const LONG SCREEN_HEIGHT = 1080;

LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Framework *f = reinterpret_cast<Framework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return f ? f->HandleMessage(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
}

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(237);
#endif
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = fnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("Game");
	wcex.hIconSm = 0;
	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hWnd = CreateWindow(_T("Game"), _T(""), WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
	ShowWindow(hWnd, cmd_show);

	Framework f(hWnd);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&f));
	return f.Run();
}

//
//LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//{
//    Framework* f = reinterpret_cast<Framework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
//    return f ? f->HandleMessage(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
//}
//
//INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
//{
//#if defined(DEBUG) | defined(_DEBUG)
//    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LzEAK_CHECK_DF);
//    //_CrtSetBreakAlloc(237);
//#endif
//
//    WNDCLASSEX wcex;
//    wcex.cbSize = sizeof(WNDCLASSEX);
//    wcex.style = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc = fnWndProc;
//    wcex.cbClsExtra = 0;
//    wcex.cbWndExtra = 0;
//    wcex.hInstance = instance;
//    wcex.hIcon = 0;
//    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    wcex.lpszMenuName = NULL;
//    wcex.lpszClassName = _T("Game");
//    wcex.hIconSm = 0;
//    RegisterClassEx(&wcex);
//
//    // �t���X�N���[���p�ɉ�ʉ𑜓x��ݒ�
//    DEVMODE devmode;
//    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
//    devmode.dmPelsWidth = SCREEN_WIDTH;
//    devmode.dmPelsHeight = SCREEN_HEIGHT;
//    devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
//    ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
//
//    // �E�B���h�E�T�C�Y�ƈʒu�̐ݒ�
//    RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);  // �^�C�g���o�[���c�����߂̒���
//
//    // �E�B���h�E����ʑS�̂ɍL���A�^�X�N�o�[���B��        
//    HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, _T("Game"), _T("Game Title"), WS_OVERLAPPEDWINDOW,
//        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, instance, NULL);
//
//    // �^�X�N�o�[���B�����߂ɃE�B���h�E���ő剻����
//    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
//    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_SHOWWINDOW);
//
//    // �X�V
//    UpdateWindow(hWnd);
//
//    Framework f(hWnd);
//    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&f));
//
//    // �^�C�g�����Œ肵�ĕύX���Ȃ�
//    SetWindowText(hWnd, _T("�Q�[���^�C�g��")); // �^�C�g���o�[�ɃQ�[������
//
//
//    return f.Run();
//}