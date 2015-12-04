#include "stdafx.h"
#include "MultiLanguage.h"

BOOL CMultiLanguage::m_bEnglish = FALSE;
map<CString,CString>	CMultiLanguage::m_mapTranslate;

void CMultiLanguage::SetLanguage(BOOL bEnglish)
{
	m_bEnglish = bEnglish;
}

CString CMultiLanguage::Translate(const CString& strKey)
{
	if ( m_bEnglish )
		return strKey;

	CString	strRet = m_mapTranslate[strKey];
	if ( strRet.IsEmpty())
		strRet = strKey;

	return strRet;
}

void CMultiLanguage::Init()
{
	// Common
	m_mapTranslate["Exit"] = "�˳�";
	m_mapTranslate["Table"] = "��";
	m_mapTranslate["OK"] = "ȷ��";
	m_mapTranslate["Cancel"] = "ȡ��";
	m_mapTranslate["Save"] = "����";
	m_mapTranslate["ALL"] = "ȫ��";
	m_mapTranslate["Notes"] = "�ı�";
	m_mapTranslate["Set Date Range"] = "�������ڷ�Χ";
	m_mapTranslate["Set Time Range"] = "����ʱ�䷶Χ";
	m_mapTranslate["From"] = "��";
	m_mapTranslate["To"] = "��";
	m_mapTranslate["Clear"] = "���";
	m_mapTranslate["Do you want to save the changes?"] = "Ҫ������";
	m_mapTranslate["Prompt For Save"] = "��ʾ����";

	// About dialog
	m_mapTranslate["&About TinyDB..."] = "&���� TinyDB...";
	m_mapTranslate["About TinyDB"] = "���� TinyDB";
	m_mapTranslate["TinyDB Version 1.0"] = "TinyDB �汾1.0";
	m_mapTranslate["Developer : Sail Feng,  mailflm@163.com"] = "���� :	������, mailflm@163.com";

	// Menu
	m_mapTranslate["Copy"] = "����";
	m_mapTranslate["Delete All Selected Items"] = "ɾ������ѡ�е���";
	m_mapTranslate["Insert Item Before This Row"] = "����һ������";
	m_mapTranslate["Append Item To Bottom"] = "����һ�����ݣ�������棩";
	m_mapTranslate["Move Top"] = "�Ƶ�����";
	m_mapTranslate["Move Up"] = "����";
	m_mapTranslate["Move Down"] = "����";
	m_mapTranslate["Move Bottom"] = "�Ƶ�ĩ��";
	m_mapTranslate["Select All"] = "ȫѡ";
	m_mapTranslate["Select None"] = "ȫ��ѡ";
	m_mapTranslate["Delete All Selected Users"] = "ɾ���û�";
	m_mapTranslate["New User"] = "�����û�";
	m_mapTranslate["Delete All Selected Columns"] = "ɾ������ѡ�е���";
	m_mapTranslate["Insert Column"] = "����һ����";
	m_mapTranslate["Append Column"] = "���һ���У�������棩";
	m_mapTranslate["Save"] = "����";


	// Main Dialog
	m_mapTranslate["TinyDB - Welcome, %s"] = "TinyDB - ��ӭ��%s";
	m_mapTranslate["Create Table"] = "�½���";
	m_mapTranslate["Edit Table"] = "�༭��";
	m_mapTranslate["Delete Table"] = "ɾ����";
	m_mapTranslate["Export"] = "����";
	m_mapTranslate["Import"] = "����";
	m_mapTranslate["Change Password"] = "��������";
	m_mapTranslate["Options"] = "ѡ��";
	m_mapTranslate["Are you sure to delete this table?"] = "��ȷ��Ҫɾ���������";
	m_mapTranslate["Change password successfully!"] = "������ĳɹ���";
	m_mapTranslate["Change password failed!"] = "�������ʧ�ܣ�";

	// Login
	m_mapTranslate["Username"] = "�û���";
	m_mapTranslate["Password"] = "����";
	m_mapTranslate["Login"] = "��¼";


	// Create/Edit table
	m_mapTranslate["Table Property"] = "������";
	m_mapTranslate["Table Name"] = "����";
	m_mapTranslate["Header Height"] = "��ͷ�߶�";
	m_mapTranslate["Row Height"] = "�и�";
	m_mapTranslate["Enable Filter"] = "���ò�ѯ";
	m_mapTranslate["Select Then Edit"] = "��ѡ���ٱ༭";
	m_mapTranslate["Highlight Changed Cell"] = "�������ĵĵ�Ԫ��";
	m_mapTranslate["Allow Multi Selection"] = "����ѡ�����";
	m_mapTranslate["Edit First Cell After New Row Added"] = "�������к��Զ��༭��һ��";
	m_mapTranslate["Edit Next Cell After Hit Return"] = "�س���༭��һ��";
	m_mapTranslate["Column Definition"] = "������";
	m_mapTranslate["Name"] = "����";
	m_mapTranslate["Column Name"] = "�е�����";
	m_mapTranslate["Width"] = "���";
	m_mapTranslate["Data Type"] = "��������";
	m_mapTranslate["Edit Type"] = "�༭����";
	m_mapTranslate["Align"] = "����";
	m_mapTranslate["Default Value"] = "ȱʡֵ";
	m_mapTranslate["Select List"] = "ѡ���б���;������";
	m_mapTranslate["Create Table"] = "�½���";
	m_mapTranslate["Edit Table"] = "�༭��";
	m_mapTranslate["You must set at least one column!"] = "������������һ����";
	m_mapTranslate["Table name can NOT be empty!"] = "�����ֲ���Ϊ�գ�";
	m_mapTranslate["Table name already exist!"] = "���Ѵ��ڣ�";

	// Change password
	m_mapTranslate["Change Password"] = "��������";
	m_mapTranslate["Old Password"] = "������";
	m_mapTranslate["New Password"] = "������";
	m_mapTranslate["Confirm Password"] = "ȷ������";
	m_mapTranslate["New password mismatch!"] = "�����벻ƥ�䣡";

	// Export / import
	m_mapTranslate["Table Name In The File"] = "�ļ��еı���";
	m_mapTranslate["Table Name Imported(Editable)"] = "�����ı������ɸ�����";
	m_mapTranslate["Table Name In The System"] = "ϵͳ�еı���";
	m_mapTranslate["Table Name Exported(Editable)"] = "������ı���(�ɸ�����";
	m_mapTranslate["Please set the export file!"] = "�����õ������ļ���";
	m_mapTranslate["Password mismatch!"] = "���벻ƥ�䣡";
	m_mapTranslate["Table name %s is duplicated!"] = "����%s�ظ���";
	m_mapTranslate["Table name %s is already existed!"] = "����%s�Ѵ��ڣ�";
	m_mapTranslate["Please select at least one table!"] = "������ѡ��һ�ű���";
	m_mapTranslate["This file contains entire database information, you can NOT import it!"] = "����ļ������������ݿ���Ϣ������Ȩ���룡";
	m_mapTranslate["Import All"] = "����ȫ��";
	m_mapTranslate["Export All"] = "����ȫ��";
	m_mapTranslate["Password Protect"] = "���뱣��";
	m_mapTranslate["Reload"] = "���¼���";
	m_mapTranslate["Encrypt"] = "����";
	m_mapTranslate["File"] = "�ļ�";
	m_mapTranslate["Export Tables"] = "����ѡ����";
	m_mapTranslate["Import Tables"] = "����ѡ����";
	m_mapTranslate["Export failed!"] = "����ʧ�ܣ�";
	m_mapTranslate["Export to file %s successfully!"] = "�������ļ�%s�ɹ���";

	// Options
	m_mapTranslate["Property Name"] = "����";
	m_mapTranslate["Property Value"] = "ֵ";
	m_mapTranslate["Language"] = "����";
	m_mapTranslate["Query"] = "��ѯ";
	m_mapTranslate["Case Sensitive"] = "���ִ�Сд";
	m_mapTranslate["Query while editing"] = "�༭ʱ������ѯ";
	m_mapTranslate["Auto Save"] = "�Զ�����";
	m_mapTranslate["Enable"] = "����";
	m_mapTranslate["Time Interval(minutes)"] = "����ʱ�������֣�";
	m_mapTranslate["Auto Save File"] = "�Զ������ļ�";
	m_mapTranslate["Chinese"] = "����";
	

	// CIniDB
	m_mapTranslate["Error loading database file!"] = "װ�����ݿ��ļ�����";
	m_mapTranslate["Error loading table information!"] = "�޷������ݿ��л�ȡ����Ϣ��";
	m_mapTranslate["Can NOT find table %s!"] = "��%s������!";
	m_mapTranslate["Update table access information failed!"] = "�����û����ʱ�ʧ�ܣ�";
	m_mapTranslate["Error loading user information!"] = "�޷������ݿ��к�ȥ�û���Ϣ��";
	m_mapTranslate["Error loading user table access information!"] = "�޷������ݿ��л�ȡ�û�Ȩ�ޣ�";
	m_mapTranslate["Wrong password!"] = "�������";
	m_mapTranslate["User does NOT exist!"] = "�û������ڣ�";
	m_mapTranslate["The user name %s is reserved!"] = "�û���%s��������";
	m_mapTranslate["The user name %s is already exist!"] = "�û���%s�Ѵ��ڣ�";
	m_mapTranslate["Can NOT find table %s!"] = "�޷��ҵ���%s��";
	m_mapTranslate["Remove table access information failed!"] = "ɾ���û�Ȩ����Ϣʧ�ܣ�";


	m_mapTranslate["Align"] = "����";

}

void CMultiLanguage::Clear()
{
	m_mapTranslate.clear();
}
