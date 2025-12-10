#ifndef NOMINMAX
#define NOMINMAX 
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

// librabries in order to avoid conflicts
#include <WinSock2.h>
#include <Windows.h>
#include <wingdi.h>
#include <gdiplus.h>

#include <iostream>
#include <string>

#include "SVGDocument.h"
#include "SVGParser.h"
#include "SVGRenderer.h"
#include "SVGFactoryPattern.h" 

using namespace Gdiplus;
using namespace std;

#ifdef _MSC_VER
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib,"user32.lib")
#pragma comment (lib,"gdi32.lib")
#pragma comment (lib,"kernel32.lib")
#endif

const std::string SVG_FILENAME = "D:/Download/svg-18.svg";
const LPCWSTR WINDOW_CLASS_NAME = L"SVGReaderWindow";
const LPCWSTR WINDOW_TITLE = L"SVG Reader Demo";

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// global declaration
SVGDocument* g_svgDocument = nullptr;
SVGParser g_parser;
SVGRenderer g_renderer;
SVGFactoryPattern g_factory; 
ULONG_PTR gdiplusToken;

VOID OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);	Graphics graphics(hdc);

	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.Clear(Gdiplus::Color(255, 255, 255)); 

	if (g_svgDocument && g_svgDocument->getRootGroup()) {
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		float viewportW = (float)clientRect.right;
		float viewportH = (float)clientRect.bottom;

		float viewBoxW = 1500.0f; 
		float viewBoxH = 750.0f; 

		float scaleX = viewportW / viewBoxW;
		float scaleY = viewportH / viewBoxH;
		float scale = std::min(scaleX, scaleY);

		graphics.ScaleTransform(scale, scale);

		g_svgDocument->renderSVGImage(g_renderer, graphics);
	}
	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_PAINT:
		OnPaint(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, INT iCmdShow) {

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	try {
		g_svgDocument = new SVGDocument(SVG_FILENAME);

		g_svgDocument->parseSVGImage(g_parser, g_factory);

		if (!g_svgDocument->getRootGroup()) {
			std::cerr << "Error: SVG parsing failed or root group is empty." << std::endl;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Parsing Exception: " << e.what() << std::endl;
		GdiplusShutdown(gdiplusToken);
		return 1;
	}

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = WINDOW_CLASS_NAME; 
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		GdiplusShutdown(gdiplusToken);
		return 0;
	}

	HWND hWnd = CreateWindow(
		WINDOW_CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, hInstance, NULL
	);

	if (hWnd == NULL) {
		GdiplusShutdown(gdiplusToken);
		return 0;
	}

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	delete g_svgDocument;
	GdiplusShutdown(gdiplusToken);

	return static_cast<INT>(Msg.wParam);
}