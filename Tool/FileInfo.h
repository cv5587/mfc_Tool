#pragma once

#include "Include.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString	ConvertRelativePath(CString strFullPath);

	static void		DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathList);
	static int		DirInfoCount(const wstring& wstrPath);
	static bool		CheckNum(const wstring& wstrTextureName);
};

