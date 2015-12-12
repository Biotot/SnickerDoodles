#pragma once
#include "stdafx.h"
#include "LeagueMiner.h"
//#include <thread>


using namespace System::Threading;
using namespace std;


#define HeatMapNodeCount	5
#define TYPE_STRING "string"
#define TYPE_INT "int"
#define TYPE_FLOAT  "float"
#define MAX_IMAGES  50

namespace SnickerDoodles
{
	struct DrawnImage
	{
		CString m_Type;
		CString m_DrawType;

		int m_X; int m_Y; int m_Width; int m_Height;
		CString m_Text;
		int m_Color;

	};

	class PainterBase
	{
	public:
		PainterBase();
		~PainterBase(); 

		bool StartThread();
		void SetUp();
		void StartBuffer();
		void LoadBuffer();
		void LoadReadyArray();
		void LoadDrawingArray();

		HDC HDC_Desktop, Memhdc, Loadedhdc;
		HWND TargetWnd;
		HWND TransWnd;
		HWND Handle;
		HBITMAP Membitmap;

		HBRUSH box[10];
		HFONT Font;
		
		//Text colors
		COLORREF textColor[4];
		float cameraPosition[3];
		int screenWidth, screenHeight;
		int miniScreenSize, mapLocationX;
		int m_NewImageSize = 0;
		bool m_ImagesReady = false;
		bool m_ImagesTransfering = false;


		DrawnImage m_NewImages[MAX_IMAGES];
		DrawnImage m_ReadyImages[MAX_IMAGES];
		DrawnImage m_DrawingImages[MAX_IMAGES];

		void AddImage(DrawnImage tImage);
	private:


		void DrawMiniMapBox(int x, int y, int w, int h, int thickness, int color);
		void DrawMiniMapString(int x, int y, int color, CString text);
		void DrawDataString(int x, int y, int color, CString text);

		void DrawFilledRect(int x, int y, int w, int h, int color);
		void DrawBorderBox(int x, int y, int w, int h, int thickness, int color);

		void DrawString(int x, int y, int color, CString text);
		void DrawStringToDC(HDC paintTarget, int x, int y, int color, const char* text);

	};

	
}