#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	PathRelativePathTo(szRelativePath, 
						szCurDirPath, 
						FILE_ATTRIBUTE_DIRECTORY, 
						strFullPath.GetString(), 
						FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring& wstrPath, list<IMGPATH*>& rPathList)
{
	// CFileFind : mfc에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind		Find;

	// *.*을 결합하여 경로상의 모든 파일을 찾을 것
	wstring		wstrFilePath = wstrPath + L"\\*.*";

	//FindFile : 매개 변수로 들어온 경로에 파일이 있는지 없는지 검사하는 함수

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());
	int fileother = 8;
	while (bContinue)
	{
		//FindNextFile : 동일 경로 안에서 다음 파일이 있는지 탐색
		// 더이상 찾을 파일이 없거나 또는 마지막 파일인 경우 0을 반환

		// 파일 유무를 확인하고 실제 그 파일에 접근하려 할 경우, FindNextFile 함수를 무조건 호출해야 함 

		bContinue = Find.FindNextFile();

		// 찾은 파일 이름이 .이나 ..이면 다시 탐색한다.
		// 디렉토리 안에는 .이나 ..같은 마커가 존재

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath : 현재 Find 객체가 가리키는 파일의 경로를 얻어오는 함수
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}
		else
		{
			if (Find.IsSystem())
				continue;

			IMGPATH* pImgPath = new IMGPATH;
			TCHAR		szPathBuf[MAX_PATH] = L"";
			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			// L"D:\\유준환\\142기\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"

			// 파일 이름과 확장자를 잘라냄
			PathRemoveFileSpec(szPathBuf);
			//szPathBuf = 0x00e9d9c4 L"D:\\유준환\\142기\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack"

			// GetFileTitle : 파일 이름만 얻어오는 함수
			wstring		wstrTextureName = Find.GetFileTitle().GetString();	
			// L"BossMultiAttack0"


			if (CheckNum(wstrTextureName))
			{	//멀티 일때 기준 
				
				pImgPath->iCount = DirInfoCount(szPathBuf);

				// substr(시작, 끝) : 시작에서 끝에 해당하는 문자열을 잘라냄
				wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
				// L"BossMultiAttack%d.png"

				TCHAR	szBuf[MAX_PATH] = L"";
				lstrcpy(szBuf, szPathBuf);

				// PathCombine(dst, src1, src2)
				// : src1과 src2를 이어 붙여 완성된 경로를 dst에 저장
				// src1과 src2 사이에 '\\'가 자동으로 삽입되어 dst에 저장

				PathCombine(szBuf, szBuf, wstrTextureName.c_str());
				// L"D:\\유준환\\142기\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"
				pImgPath->wstrPath = ConvertRelativePath(szBuf);
				// L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

				PathRemoveFileSpec(szBuf);
				// L"D:\\유준환\\142기\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack"

				// PathFindFileName : 파일 이름을 찾거나 가장 마지막 폴더 이름을 찾아냄
				pImgPath->wstrStateKey = PathFindFileName(szBuf);
				// L"BossMultiAttack"

				PathRemoveFileSpec(szBuf);
				// L"D:\\유준환\\142기\\Frame142\\Texture\\Stage\\Effect"
				pImgPath->wstrObjKey = PathFindFileName(szBuf);
				// L"Effect"

				rPathList.push_back(pImgPath);

				bContinue = 0;
			}
			else
			{		//싱글일때 기준

				pImgPath->iCount = 0;

				// substr(시작, 끝) : 시작에서 끝에 해당하는 문자열을 잘라냄
				wstrTextureName +=  L".png";
				// L"BossMultiAttack%d.png"

				TCHAR	szBuf[MAX_PATH] = L"";	
				lstrcpy(szBuf, szPathBuf);	

				// PathCombine(dst, src1, src2)
				// : src1과 src2를 이어 붙여 완성된 경로를 dst에 저장
				// src1과 src2 사이에 '\\'가 자동으로 삽입되어 dst에 저장

				PathCombine(szBuf, szBuf, wstrTextureName.c_str());	
				// L"D:\\유준환\\142기\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"
				pImgPath->wstrPath = ConvertRelativePath(szBuf);	
				// L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

				PathRemoveFileSpec(szBuf);	
				// L"D:\\유준환\\142기\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack"

				// PathFindFileName : 파일 이름을 찾거나 가장 마지막 폴더 이름을 찾아냄
				pImgPath->wstrStateKey = PathFindFileName(szBuf);
				// L"BossMultiAttack"

				PathRemoveFileSpec(szBuf);
				// L"D:\\유준환\\142기\\Frame142\\Texture\\Stage\\Effect"
				pImgPath->wstrObjKey = PathFindFileName(szBuf);
				// L"Effect"

				rPathList.push_back(pImgPath);

				bContinue = fileother--;
			}

	
		}
	}

}

int CFileInfo::DirInfoCount(const wstring& wstrPath)
{

	CFileFind		Find;
	wstring		wstrFilePath = wstrPath + L"\\*.*";
	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	int	iCount(0);

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iCount;
	}

	return iCount;
}

//TOTO:2019 숫자 체크시 TRUE 반환
bool CFileInfo::CheckNum(const wstring& wstrTextureName)
{
	int length = wstrTextureName.length();

	for (int i = 0; i < length; i++)
	{
		if (0!=isdigit(wstrTextureName[i]))
		{
			return true;
		}
	}

	return false;
}
