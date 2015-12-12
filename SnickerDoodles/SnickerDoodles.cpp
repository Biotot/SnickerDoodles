// SnickerDoodles.cpp : main project file.
#include "stdafx.h"
#include "Host.h"

using namespace System;
using namespace SnickerDoodles;

int main()
{

	Host m_Host;

	m_Host.StartPainter();
	m_Host.StartMiner();

    Console::WriteLine(L"Hello World");
	Console::ReadLine();
    return 0;
}
