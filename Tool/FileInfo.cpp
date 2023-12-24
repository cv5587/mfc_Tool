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
	// CFileFind : mfc���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
	CFileFind		Find;

	// *.*�� �����Ͽ� ��λ��� ��� ������ ã�� ��
	wstring		wstrFilePath = wstrPath + L"\\*.*";

	//FindFile : �Ű� ������ ���� ��ο� ������ �ִ��� ������ �˻��ϴ� �Լ�

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());
	int fileother = 8;
	while (bContinue)
	{
		//FindNextFile : ���� ��� �ȿ��� ���� ������ �ִ��� Ž��
		// ���̻� ã�� ������ ���ų� �Ǵ� ������ ������ ��� 0�� ��ȯ

		// ���� ������ Ȯ���ϰ� ���� �� ���Ͽ� �����Ϸ� �� ���, FindNextFile �Լ��� ������ ȣ���ؾ� �� 

		bContinue = Find.FindNextFile();

		// ã�� ���� �̸��� .�̳� ..�̸� �ٽ� Ž���Ѵ�.
		// ���丮 �ȿ��� .�̳� ..���� ��Ŀ�� ����

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath : ���� Find ��ü�� ����Ű�� ������ ��θ� ������ �Լ�
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}
		else
		{
			if (Find.IsSystem())
				continue;

			IMGPATH* pImgPath = new IMGPATH;
			TCHAR		szPathBuf[MAX_PATH] = L"";
			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			// L"D:\\����ȯ\\142��\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"

			// ���� �̸��� Ȯ���ڸ� �߶�
			PathRemoveFileSpec(szPathBuf);
			//szPathBuf = 0x00e9d9c4 L"D:\\����ȯ\\142��\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack"

			// GetFileTitle : ���� �̸��� ������ �Լ�
			wstring		wstrTextureName = Find.GetFileTitle().GetString();	
			// L"BossMultiAttack0"


			if (CheckNum(wstrTextureName))
			{	//��Ƽ �϶� ���� 
				
				pImgPath->iCount = DirInfoCount(szPathBuf);

				// substr(����, ��) : ���ۿ��� ���� �ش��ϴ� ���ڿ��� �߶�
				wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
				// L"BossMultiAttack%d.png"

				TCHAR	szBuf[MAX_PATH] = L"";
				lstrcpy(szBuf, szPathBuf);

				// PathCombine(dst, src1, src2)
				// : src1�� src2�� �̾� �ٿ� �ϼ��� ��θ� dst�� ����
				// src1�� src2 ���̿� '\\'�� �ڵ����� ���ԵǾ� dst�� ����

				PathCombine(szBuf, szBuf, wstrTextureName.c_str());
				// L"D:\\����ȯ\\142��\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"
				pImgPath->wstrPath = ConvertRelativePath(szBuf);
				// L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

				PathRemoveFileSpec(szBuf);
				// L"D:\\����ȯ\\142��\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack"

				// PathFindFileName : ���� �̸��� ã�ų� ���� ������ ���� �̸��� ã�Ƴ�
				pImgPath->wstrStateKey = PathFindFileName(szBuf);
				// L"BossMultiAttack"

				PathRemoveFileSpec(szBuf);
				// L"D:\\����ȯ\\142��\\Frame142\\Texture\\Stage\\Effect"
				pImgPath->wstrObjKey = PathFindFileName(szBuf);
				// L"Effect"

				rPathList.push_back(pImgPath);

				bContinue = 0;
			}
			else
			{		//�̱��϶� ����

				pImgPath->iCount = 0;

				// substr(����, ��) : ���ۿ��� ���� �ش��ϴ� ���ڿ��� �߶�
				wstrTextureName +=  L".png";
				// L"BossMultiAttack%d.png"

				TCHAR	szBuf[MAX_PATH] = L"";	
				lstrcpy(szBuf, szPathBuf);	

				// PathCombine(dst, src1, src2)
				// : src1�� src2�� �̾� �ٿ� �ϼ��� ��θ� dst�� ����
				// src1�� src2 ���̿� '\\'�� �ڵ����� ���ԵǾ� dst�� ����

				PathCombine(szBuf, szBuf, wstrTextureName.c_str());	
				// L"D:\\����ȯ\\142��\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"
				pImgPath->wstrPath = ConvertRelativePath(szBuf);	
				// L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

				PathRemoveFileSpec(szBuf);	
				// L"D:\\����ȯ\\142��\\Frame142\\Texture\\Stage\\Effect\\BossMultiAttack"

				// PathFindFileName : ���� �̸��� ã�ų� ���� ������ ���� �̸��� ã�Ƴ�
				pImgPath->wstrStateKey = PathFindFileName(szBuf);
				// L"BossMultiAttack"

				PathRemoveFileSpec(szBuf);
				// L"D:\\����ȯ\\142��\\Frame142\\Texture\\Stage\\Effect"
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

//TOTO:2019 ���� üũ�� TRUE ��ȯ
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
