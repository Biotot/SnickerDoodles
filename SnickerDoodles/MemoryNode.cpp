#include "stdafx.h"
#include "MemoryNode.h"


namespace SnickerDoodles
{
	MemoryNode::MemoryNode(CString tNodeType)
	{
		m_Type = tNodeType;
	}

	MemoryNode::~MemoryNode()
	{
		delete m_Value;
	}

	void MemoryNode::SetOffset(DWORD tOffset)
	{
		m_Offset = tOffset;
	}

	void MemoryNode::SetBaseOffset(DWORD tOffset)
	{
		m_BaseOffset = tOffset;
	}

	void MemoryNode::SetHandle(HANDLE tProcess)
	{
		m_ProcessHandle = tProcess;
	}

	void MemoryNode::RefreshValue()
	{
		if (m_Type == TYPESTRING)
		{
			m_Value = (void*) &ReadString(m_BaseOffset + m_Offset, m_Type);
		}
		else if (m_Type == TYPEINT)
		{
			m_Value = (void*) ReadInt(m_BaseOffset + m_Offset, m_Type);
		}
		else if (m_Type == TYPEFLOAT)
		{
			float aTemp = ReadFloat(m_BaseOffset + m_Offset, m_Type);
			m_Value = (void*) &aTemp;
		}
		else if (m_Type == TYPEDWORD)
		{
			m_Value = (void*) ReadDWORD(m_BaseOffset + m_Offset, m_Type);
		}
	}



	/*
	READING 
	*/
	CString MemoryNode::ReadString(DWORD tOffset, CString tNodeType)
	{
		CString aRet = "-1";

		if (tNodeType == TYPESTRING)
		{
			ReadProcessMemory(m_ProcessHandle, (PBYTE*)(tOffset), &aRet, sizeof(20), 0);
		}

		return aRet;
	}
	int MemoryNode::ReadInt(DWORD tOffset, CString tNodeType)
	{
		int aRet = -1;

		if (tNodeType == TYPEINT)
		{
			//ReadProcessMemory(m_ProcessHandle, (PBYTE*)(tOffset), &aRet, sizeof(int), 0);
		}

		return aRet;
	}
	float MemoryNode::ReadFloat(DWORD tOffset, CString tNodeType)
	{
		float aRet = -1;

		if (tNodeType == TYPEFLOAT)
		{
			//ReadProcessMemory(m_ProcessHandle, (PBYTE*)(tOffset), &aRet, sizeof(float), 0);
		}

		return aRet;
	}
	DWORD MemoryNode::ReadDWORD(DWORD tOffset, CString tNodeType)
	{
		DWORD aRet = -1;

		if (tNodeType == TYPEDWORD)
		{
			//ReadProcessMemory(m_ProcessHandle, (PBYTE*)(tOffset), &aRet, sizeof(DWORD), 0);
		}

		return aRet;
	}

	/*
	GETTING
	*/

	CString	MemoryNode::GetStringValue()
	{
		return "DEPRICATED";

	}
	int	MemoryNode::GetIntValue()
	{
		int aRet = -1;

		if (m_Type == TYPESTRING)
		{
			/*
			CString aTemp = *(CString*)m_Value;
			aRet = atoi((string) aTemp);
			*/
		}
		else if (m_Type == TYPEFLOAT)
		{
			aRet = *(float*)m_Value;
		}
		else if (m_Type == TYPEINT)
		{
			aRet= *(int*)m_Value;
		}

		return aRet;
	}

	float	MemoryNode::GetFloatValue()
	{
		float aRet = -1;

		if (m_Type == TYPESTRING)
		{
			/*
			CString aTemp = *(CString*)m_Value;
			aRet = atof(aTemp.Tokenize);
			*/
			
		}
		else if (m_Type == TYPEFLOAT)
		{
			aRet = *(float*)m_Value;
		}
		else if (m_Type == TYPEINT)
		{
			aRet = *(int*)m_Value;
		}

		return aRet;

	}


	DWORD	MemoryNode::GetDWORDValue()
	{
		DWORD aRet = 0x0;

		if (m_Type == TYPEDWORD)
		{
			aRet = *(DWORD*)m_Value;
		}

		return aRet;
	}


}