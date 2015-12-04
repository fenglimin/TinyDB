// Copyright 1997-98 (c) Iuri Apollonio
// given freely to www.codeguru.com

// Ini.cpp: implementation of the CIni class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Ini.h"
#include <stdlib.h>
#include <fstream>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIni::CIni()
{
	csLineEnd = "\r\n";
}

CIni::~CIni()
{
	Clear();
}

bool CIni::Read(const char * cFileName, BOOL bEncrypt)
{
	if ( bEncrypt )
		return ReadEncryptText(cFileName);
	else
		return ReadPlainText(cFileName);
}

bool CIni::Write(const char * cFileName, BOOL bEncrypt)
{
	if ( bEncrypt )
		return WriteEncryptText(cFileName);
	else
		return WritePlainText(cFileName);
}

bool CIni::ReadPlainText(const char * cFileName)
{
	Clear();
	char buf[10240];
	locale loc = locale::global(locale(""));
	ifstream ifs(cFileName,ios_base::in);
	locale::global(loc);
	while (ifs.good())
	{
		ifs.getline(buf, 10239);
		CString cs(buf);
		csList.Add(cs);
	}
	return true;
}

bool CIni::WritePlainText(const char * cFileName)
{
	ASSERT(cFileName);

	locale loc = locale::global(locale(""));
	ofstream ofs(cFileName,ios_base::out);
	locale::global(loc);

	int t, max = csList.GetSize();
	for (t = 0; t < max; t++)
	{
		ofs << csList.GetAt(t) << "\n";
	}

	return true;
}

void CIni::LoadBinaryFile(const char* cFileName, char* pBuf, int& nLen)
{

}

bool CIni::ReadEncryptText(const char * cFileName)
{
	Clear();

	locale loc = locale::global(locale(""));
	ifstream ifs(cFileName,ios_base::in | ios::binary);
	locale::global(loc);

	streampos pos = ifs.tellg();
	ifs.seekg(0, ios::end);
	int nLen = ifs.tellg();
	ifs.seekg(pos);

	char* pBuf = new char[nLen+1];
	memset(pBuf, 0, nLen+1);
	ifs.read(pBuf, nLen);

	for ( int i = 0; i < nLen; i ++ )
	{
		pBuf[i] = (pBuf[i]-i)%256;
	}

	CString strContent(pBuf);
	delete[] pBuf;

	//AfxMessageBox(strContent);

	int nPos = strContent.Find("\r\n");
	CString strLine;
	while ( nPos != -1 )
	{
		strLine = strContent.Left(nPos);
		csList.Add(strLine);
		strContent = strContent.Right(strContent.GetLength()-nPos-2);
		nPos = strContent.Find("\r\n");
	}

	return true;
}

bool CIni::WriteEncryptText(const char * cFileName)
{
	ASSERT(cFileName);

	locale loc = locale::global(locale(""));
	ofstream ofs(cFileName,ios_base::out | ios::binary);
	locale::global(loc);

	int t, max = csList.GetSize();
	int nTotalWrite = 0;
	CString strLine;
	int nLen;
	BYTE buf[10240];
	memset(buf, 0, 10240);
	for (t = 0; t <= max-1; t++)
	{
		strLine = csList.GetAt(t);
		strLine += "\r\n";
		nLen = strLine.GetLength();
		for ( int i = 0; i < nLen; i ++ )
		{
			buf[i] = (strLine.GetAt(i)+nTotalWrite)%256;
			nTotalWrite++;
		}
		ofs.write((char*)buf, nLen);
	}

	return true;
}

// **********************************************************************************

void CIni::Clear()
{
	csList.RemoveAll();
}

// **********************************************************************************

int CIni::FindSection(const char * cSection)
{
	int t, max = csList.GetSize();
	CString csSection;
	csSection.Format("[%s]", cSection);

	for (t = 0; t < max; t++)
		if (csList.GetAt(t) == csSection) return t;

	return -1;
}

int CIni::InsertSection(const char * cSection)
{
	ASSERT(cSection);
	if (!cSection) return -1;

	int idx = FindSection(cSection);
	if (idx < 0)
	{
		CString csSection;
		csSection.Format("[%s]", cSection);
		idx = csList.Add(csSection);
	}
	return idx;
}

int CIni::FindItem(const int iSection, const char * cItem, CString &csVal)
{
	ASSERT(iSection >= 0);
	ASSERT(cItem);

	int max = csList.GetSize(), t;
	CString csItem(cItem), csLook;
//	csItem += " = ";
	int iLen = csItem.GetLength();

	for (t = iSection; t < max; t++)
	{
		if (!IsSection(t))
		{
			csLook = csList.GetAt(t);
			if (csLook.GetLength() >= iLen)
			{
				if (csLook.Left(iLen) == csItem) 
				{
					if (csLook.GetLength() == iLen) 
						csVal = "";
					else 
					{
						int nLen = csLook.GetLength();
						while ( ( iLen < nLen ) && ( csLook.GetAt(iLen++) == ' ' ) );
						iLen--;
						while ( ( iLen < nLen ) && ( csLook.GetAt(iLen++) == '=' ) );
						iLen--;
						while ( ( iLen < nLen ) && ( csLook.GetAt(iLen++) == ' ' ) );
						iLen--;
						csVal = csLook.Right(csLook.GetLength() - iLen);
					}
						
					return t;
				}
			}
		}
		else return -1;
	}
	return -1;
}

int CIni::FindMultiItem(const int iSection, const char * cItem, CString &csVal)
{
	ASSERT(iSection >= 0);
	ASSERT(cItem);

	int max = csList.GetSize(), t, i;
	CString csItem(cItem), csLook;
	csItem += " = \"";
	int iLen = csItem.GetLength();

	for (t = iSection; t < max; t++)
	{
		if (!IsSection(t))
		{
			csLook = csList.GetAt(t);
			if (csLook == csItem)
			{
				csVal = "";
				for (i = t + 1; i < max; i++)
				{
					csLook = csList.GetAt(i);
					if (csLook == '\"' || IsSection(i)) 
					{
						i = max; 
					}
					else 
					{
						if (csVal != "") csVal += csLineEnd;
						csVal += csLook;
					}
				}
				return t;
			}
		}
		else return -1;
	}
	return -1;
}

bool CIni::IsSection(const int iSection)
{
	ASSERT(iSection >= 0 && iSection < csList.GetSize());
	if (iSection >= 0 && iSection < csList.GetSize())
	{
		CString csItem = csList.GetAt(iSection);
		if (csItem.GetLength() > 2 && csItem.Left(1) == '[' && csItem.Right(1) == ']') return true;
	}
	return false;
}

bool CIni::RemoveSection(const char * cSection)
{
	int idx = FindSection(cSection);
	if (idx >= 0)
	{
		for (;;)
		{
			csList.RemoveAt(idx);
			if (idx >= csList.GetSize()) return true;
			if (IsSection(idx)) return true;
		}
	}
	return true;
}

void CIni::RemoveMultiLineItem(const int idx)
{
	int max = csList.GetSize(), t;
	CString csLook;

	for (t = idx; t < max; t++)
	{
		if (!IsSection(t))
		{
			csLook = csList.GetAt(t);
			if (csLook == '\"')
			{
				csList.RemoveAt(t);
				return;
			}
			csList.RemoveAt(t);
		}
		else return;
	}
}

// **********************************************************************************

bool CIni::SetValue(const char * cSection, const char * cItem, const bool bVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = %s", cItem, bVal ? "true" : "false");
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}

bool CIni::SetValue(const char * cSection, const char * cItem, const COLORREF crVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = %d", cItem, (DWORD) crVal);
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}

bool CIni::SetValue(const char * cSection, const char * cItem, const char * cVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = %s", cItem, cVal);
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}

bool CIni::SetValue(const char * cSection, const char * cItem, const double dbVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = %f", cItem, dbVal);
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}

bool CIni::SetValue(const char * cSection, const char * cItem, const float fVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = %f", cItem, fVal);
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}

bool CIni::SetValue(const char * cSection, const char * cItem, const long lVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = %d", cItem, lVal);
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}

bool CIni::SetValue(const char * cSection, const char * cItem, const int iVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = %d", cItem, iVal);
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}

bool CIni::SetMultiValue(const char * cSection, const char * cItem, const char * cVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = %s", cItem, cVal);
		char * c = csVal.LockBuffer();

		int i = csVal.Find('\r');
		while (i >= 0)
		{
			c[i] = ' ';
			i = csVal.Find('\r');
		}
		i = csVal.Find('\n');
		while (i >= 0)
		{
			c[i] = '|';
			i = csVal.Find('\n');
		}

		csVal.UnlockBuffer();
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}

bool CIni::SetValue(const char * cSection, const char * cItem, const CRect rcVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = RECT(%d,%d,%d,%d)", cItem, rcVal.left, rcVal.top, rcVal.right, rcVal.bottom);
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}

bool CIni::SetValue(const char * cSection, const char * cItem, const CPoint ptVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		int iIdx = FindItem(idx+1, cItem, csVal);
		csVal.Format("%s = POINT(%d,%d)", cItem, ptVal.x, ptVal.y);
		if (iIdx >= 0) csList.SetAt(iIdx, csVal);
		else csList.InsertAt(idx+1, csVal);
		return true;
	}
	return false;
}


// **********************************************************************************

bool CIni::GetValue(const char * cSection, const char * cItem, COLORREF &crVal)
{
	int idx = FindSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		if (FindItem(idx+1, cItem, csVal) > 0)
		{
			crVal = (COLORREF) (DWORD) atol(csVal);
			return true;
		}
	}
	return false;
}

bool CIni::GetValue(const char * cSection, const char * cItem, bool &bVal)
{
	int idx = FindSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		if (FindItem(idx+1, cItem, csVal) > 0)
		{
			if (csVal.Find("true") >= 0) bVal = true; else bVal = false;
			return true;
		}
	}
	return false;
}

bool CIni::GetValue(const char * cSection, const char * cItem, CString &cVal)
{
	int idx = FindSection(cSection);
	if (idx >= 0)
	{
		if (FindItem(idx+1, cItem, cVal) > 0)
			return true;
	}
	return false;
}

bool CIni::GetValue(const char * cSection, const char * cItem, double &dbVal)
{
	int idx = FindSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		if (FindItem(idx+1, cItem, csVal) > 0)
		{
			dbVal = (double) atof(csVal);
			return true;
		}
	}
	return false;
}

bool CIni::GetValue(const char * cSection, const char * cItem, float &fVal)
{
	int idx = FindSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		if (FindItem(idx+1, cItem, csVal) > 0)
		{
			fVal = (float) atof(csVal);
			return true;
		}
	}
	return false;
}

bool CIni::GetValue(const char * cSection, const char * cItem, long &lVal)
{
	int idx = FindSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		if (FindItem(idx+1, cItem, csVal) > 0)
		{
			lVal = (long) atol(csVal);
			return true;
		}
	}
	return false;
}

bool CIni::GetValue(const char * cSection, const char * cItem, int &iVal)
{
	int idx = FindSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		if (FindItem(idx+1, cItem, csVal) > 0)
		{
			iVal = (int) atoi(csVal);
			return true;
		}
	}
	return false;
}

bool CIni::GetMultiValue(const char * cSection, const char * cItem, CString &cVal)
{
	int idx = FindSection(cSection);
	if (idx >= 0)
	{
		if (FindItem(idx+1, cItem, cVal) > 0)
		{
			char * ch = cVal.LockBuffer();
			int i = cVal.Find(' ');
			while (i >= 0)
			{
				ch[i] = '\r';
				i = cVal.Find(' ');
			}
			i = cVal.Find('|');
			while (i >= 0)
			{
				ch[i] = '\n';
				i = cVal.Find('|');
			}
			cVal.UnlockBuffer();
			return true;
		}
	}
	return false;
}

bool CIni::GetValue(const char * cSection, const char * cItem, CRect &rcVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		if (FindItem(idx+1, cItem, csVal) > 0)
		{
			char * pt = csVal.LockBuffer();
			int pf, t = 0, l = 0, r = 0, b = 0;
			pf = sscanf_s(csVal, "RECT(%d,%d,%d,%d)", &l, &t, &r, &b);
			ASSERT(pf == 4);
			csVal.UnlockBuffer();
			rcVal.SetRect(l, t, r, b);
			return true;
		}
	}
	return false;
}

bool CIni::GetValue(const char * cSection, const char * cItem, CPoint &ptVal)
{
	int idx = InsertSection(cSection);
	if (idx >= 0)
	{
		CString csVal;
		if (FindItem(idx+1, cItem, csVal) > 0)
		{
			char * pt = csVal.LockBuffer();
			int pf, x = 0, y = 0;
			pf = sscanf_s(csVal, "POINT(%d,%d)", &x, &y);
			ASSERT(pf == 2);
			csVal.UnlockBuffer();
			ptVal.x = x;
			ptVal.y = y;
			return true;
		}
	}
	return false;
}


BOOL CIni::DeleteSection(const char *cSection)
{
	int idx = FindSection ( cSection );
	if ( idx < 0 )
		return FALSE;

	csList.RemoveAt ( idx );
	while ( idx < csList.GetSize() )
		if ( !IsSection ( idx ) )
			csList.RemoveAt ( idx );
		else
			break;

	return TRUE;
}

bool CIni::RenameSecton(const char* cOldSection, const char* cNewSection)
{
	int idx = FindSection(cOldSection);
	if ( idx < 0 )
		return false;

	CString strNew;
	strNew.Format(_T("[%s]"), cNewSection);
	csList.SetAt(idx, strNew);

	return true;
}