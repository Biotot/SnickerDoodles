#include "stdafx.h"
#include "PainterBase.h"

#include <Windows.h>
#include <windows.h>
#include "windows.h"
#include "Windows.h"
#include <sstream>
#include <iostream> 
#include <math.h>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>  
#include "Wingdi.h"

#pragma comment( lib, "Msimg32" )
using namespace std;
namespace SnickerDoodles
{
#define MiniMapScreenStartX 1650
#define MiniMapScreenStartY 1070
#define MiniMapScreenHeight 260
#define MiniMapScreenWidth 260

#define MapWidth 15000
#define MapHeight 15000

	PainterBase::PainterBase()
	{
	}


	PainterBase::~PainterBase()
	{
	}



	bool PainterBase::StartThread()
	{
		bool aRet = true;
		m_ImagesReady = false;
		SetUp();
		while (true)
		{

			//printf("Painted\n");
			StartBuffer();
			LoadBuffer();
			if (m_ImagesReady)
			{
				LoadDrawingArray();
			}

		}

		return aRet;
	}


	void PainterBase::SetUp() {

		m_NewImageSize = 0;
		//creating all of the colors
		
		box[3] = CreateSolidBrush(RGB(0, 250, 0));//Red
		box[2] = CreateSolidBrush(RGB(250, 250, 0));//Orange
		box[1] = CreateSolidBrush(RGB(250, 100, 0));//Yellow
		box[0] = CreateSolidBrush(RGB(250, 0, 0));//Green

		textColor[3] = RGB(0, 250, 0);
		textColor[2] = RGB(250, 250, 0);
		textColor[1] = RGB(250, 100, 0);
		textColor[0] = RGB(250, 0, 0);

		TargetWnd = FindWindow(0, "League of Legends (TM) Client");
		RECT tSize;
		GetWindowRect(TargetWnd, &tSize);
		screenWidth = tSize.right - tSize.left;
		screenHeight = tSize.bottom - tSize.top;

		miniScreenSize = screenHeight / 4;
		mapLocationX = screenWidth - miniScreenSize;

		HDC_Desktop = GetDC(TargetWnd);
		Memhdc = CreateCompatibleDC(HDC_Desktop);
		Loadedhdc = CreateCompatibleDC(HDC_Desktop);

	}


	void PainterBase::AddImage(DrawnImage tImage)
	{
		//adding a null check because it is stupid
		try
		{
			if ((m_NewImages != NULL))
			{
				m_NewImages[m_NewImageSize] = tImage;
				m_NewImageSize++;
			}
		}
		catch (int e)
		{
			cout << "An exception occurred. Exception Nr. " << e << '\n';
		}
	}

	void PainterBase::LoadReadyArray()
	{
		m_ImagesReady = false;
		while (m_ImagesTransfering);
		//printf("Ready<<<<<<<\n");
		for (int x = 0; x < MAX_IMAGES; x++ )
		{
			m_ReadyImages[x] = m_NewImages[x];
			m_NewImages[x].m_DrawType = "";
		}
		m_NewImageSize = 0;
		m_ImagesReady = true;
	}


	void PainterBase::LoadDrawingArray()
	{
		m_ImagesTransfering = true;
		//printf("Drawn\n");
		for (int x = 0; x < MAX_IMAGES; x++)
		{
			m_DrawingImages[x] = m_ReadyImages[x];
		}
		m_ImagesTransfering = false;
	}

	
	/*
	
	ASSISTED SCREEN DRAWING
	
	*/
	void PainterBase::DrawMiniMapBox(int x, int y, int w, int h, int thickness, int color)
	{

		int aDrawnX = (((float)x / (MapWidth))*MiniMapScreenWidth) + MiniMapScreenStartX;

		int aDrawnY = MiniMapScreenStartY - ((float)y / MapWidth)*MiniMapScreenWidth;

		DrawBorderBox(aDrawnX, aDrawnY, w, h, thickness, color);
	}
	
	void PainterBase::DrawMiniMapString(int x, int y, int color, CString text)
	{
		int aDrawnX = (((float)x / (MapWidth))*MiniMapScreenWidth) + MiniMapScreenStartX;

		int aDrawnY = MiniMapScreenStartY - ((float)y / MapWidth)*MiniMapScreenWidth;

		DrawString(aDrawnX+3, aDrawnY-15, color, text);
	}

	void PainterBase::DrawDataString(int x, int y, int color, CString text)
	{
		DrawString(x, y, color, text);
	}


	/*
	
	BASE SCREEN DRAWING 
	
	*/

	void PainterBase::DrawStringToDC(HDC paintTarget, int x, int y, int color, const char* text)
	{
		SetTextAlign(paintTarget, TA_CENTER | TA_NOUPDATECP);
		SetBkColor(paintTarget, RGB(0, 0, 0));
		SetBkMode(paintTarget, TRANSPARENT);
		if (color < 0)return;
		if (color > 4) {
			color = 0;
		}
		SetTextColor(paintTarget, textColor[color]);
		SelectObject(paintTarget, Font);
		TextOutA(paintTarget, x, y, text, strlen(text));
		DeleteObject(Font);
	}
	void PainterBase::DrawString(int x, int y, int color, CString text)
	{
		DrawStringToDC(Memhdc, x, y, color, text);
		//DrawStringToDC(HDC_Desktop, x, y, color, text);

	}
	void PainterBase::DrawFilledRect(int x, int y, int w, int h, int color)
	{
		//We create our rectangle to draw on screen
		RECT rect = { x, y, x + w, y + h };
		if ((color < 0) || (color >= 10)) {
			color = 0;
		}
		FillRect(Memhdc, &rect, box[color]);
		//FillRect(HDC_Desktop, &rect, box[color]);
	}

	void PainterBase::DrawBorderBox(int x, int y, int w, int h, int thickness, int color)
	{
		//Top horiz line
		DrawFilledRect(x, y, w, thickness, color);
		//Left vertical line
		DrawFilledRect(x, y, thickness, h, color);
		//right vertical line
		DrawFilledRect((x + w), y, thickness, h, color);
		//bottom horiz line
		DrawFilledRect(x, y + h, w + thickness, thickness, color);
	}

	/*

	BUFFER MANAGEMENT

	*/

	void PainterBase::StartBuffer() {
		DeleteObject(Membitmap);
		DeleteDC(Memhdc);
		DeleteDC(HDC_Desktop);

		HDC_Desktop = GetDC(TargetWnd);
		Memhdc = CreateCompatibleDC(HDC_Desktop);

		Membitmap = CreateCompatibleBitmap(HDC_Desktop, screenWidth, screenHeight);

		SelectObject(Memhdc, Membitmap);


	}

	void PainterBase::LoadBuffer() {

		for (int x = 0; x < MAX_IMAGES; x++)
		{
			if (m_DrawingImages[x].m_DrawType == "MiniMap")
			{
				if (m_DrawingImages[x].m_Type == "Box")
				{
					DrawMiniMapBox(m_DrawingImages[x].m_X, m_DrawingImages[x].m_Y, m_DrawingImages[x].m_Width, m_DrawingImages[x].m_Height, 2, m_DrawingImages[x].m_Color);
				}
				else if (m_DrawingImages[x].m_Type == "ChampInitial")
				{
					DrawMiniMapString(m_DrawingImages[x].m_X, m_DrawingImages[x].m_Y,  m_DrawingImages[x].m_Color, m_DrawingImages[x].m_Text);
				}
			}
			else if (m_DrawingImages[x].m_DrawType == "TabData")
			{
				boolean tabbed = false;
				if ((1 << 16) & (SHORT)(GetAsyncKeyState(VK_TAB)))
				{
					tabbed = true;

					if (m_DrawingImages[x].m_Type == "PlayerData")
					{
						DrawDataString(screenWidth-200, m_DrawingImages[x].m_Y, m_DrawingImages[x].m_Color, m_DrawingImages[x].m_Text);
					}
				}
			}
			else if (m_DrawingImages[x].m_DrawType == "")
			{
				//Stop looking for more images. This one is empty
				break;
			}
		}
		GdiTransparentBlt(HDC_Desktop, 0, 0, screenWidth, screenHeight, Memhdc, 0, 0, screenWidth, screenHeight, RGB(0, 0, 0));

	}
}