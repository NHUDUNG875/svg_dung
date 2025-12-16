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

const std::string SVG_FILENAME = "D:/Download/svg-06.svg";
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
	HDC hdc = BeginPaint(hWnd, &ps);
	Graphics graphics(hdc);

	graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	graphics.Clear(Gdiplus::Color(255, 255, 255));

	if (g_svgDocument && g_svgDocument->getRootGroup()) {
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		float viewportW = (float)clientRect.right;
		float viewportH = (float)clientRect.bottom;

		// 1. Lấy thông tin kích thước và ViewBox
		float docW = g_svgDocument->getWidth();
		float docH = g_svgDocument->getHeight();
		SVGViewBox vb = g_svgDocument->getViewBox();

		// Nếu không có viewBox hợp lệ, giả lập viewBox từ width/height
		if (!vb.isValid()) {
			vb.x = 0; vb.y = 0;
			vb.width = docW > 0 ? docW : 800.0f;
			vb.height = docH > 0 ? docH : 600.0f;
		}

		// 2. Tính tỷ lệ Scale (Fit to screen)
		float scaleX = viewportW / vb.width;
		float scaleY = viewportH / vb.height;
		float scale = std::min(scaleX, scaleY); // Giữ tỷ lệ khung hình

		// 3. Tính toán vị trí để căn giữa (Centering)
		// Kích thước thực tế sau khi zoom
		float finalW = vb.width * scale;
		float finalH = vb.height * scale;

		float offsetX = (viewportW - finalW) / 2.0f;
		float offsetY = (viewportH - finalH) / 2.0f;

		// 4. Áp dụng Transform (Thứ tự rất quan trọng!)

		// B1: Dịch chuyển vùng vẽ ra giữa màn hình
		graphics.TranslateTransform(offsetX, offsetY);

		// B2: Phóng to/Thu nhỏ
		graphics.ScaleTransform(scale, scale);

		// B3: Dịch chuyển ngược lại theo min-x, min-y của viewBox
		// (Để điểm (vb.x, vb.y) trùng với gốc tọa độ 0,0 mới)
		graphics.TranslateTransform(-vb.x, -vb.y);

		// Render
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