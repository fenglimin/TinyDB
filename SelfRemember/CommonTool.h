#pragma once

class CCommonTool
{
public:
	CCommonTool(void);
	virtual ~CCommonTool(void);

public:
	BOOL	IsCtrlAltPressed();
	BOOL	OutFile(int nResID, char* strResName, char* FileName );
	BOOL	RunProgram ( char* FileName, char* strWorkPath, int nShowFlag );
};
