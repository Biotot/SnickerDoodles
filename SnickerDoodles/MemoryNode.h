#pragma once
namespace SnickerDoodles
{
#define TYPESTRING "STRING"
#define TYPEFLOAT "FLOAT"
#define TYPEINT "INT"
#define TYPEDWORD "DWORD"



	class MemoryNode
	{
	public:
		MemoryNode(CString tNodeType);
		~MemoryNode();
		CString GetStringValue();
		int GetIntValue();
		float GetFloatValue();
		DWORD GetDWORDValue();
		void SetOffset(DWORD tOffset);
		void SetBaseOffset(DWORD tOffset);
		void RefreshValue();



		CString	ReadString(DWORD tOffset, CString tNodeType);
		int		ReadInt(DWORD tOffset, CString tNodeType);
		float	ReadFloat(DWORD tOffset, CString tNodeType);
		DWORD	ReadDWORD(DWORD tOffset, CString tNodeType);
		void	SetHandle(HANDLE tProcess);

	private:

		void *m_Value;
		CString m_Type;
		int m_Depth;
		DWORD m_Offset;
		DWORD m_BaseOffset;
		HANDLE m_ProcessHandle;

	};

}
