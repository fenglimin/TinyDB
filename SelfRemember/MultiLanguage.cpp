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
	m_mapTranslate["Exit"] = "退出";
	m_mapTranslate["Table"] = "表单";
	m_mapTranslate["OK"] = "确定";
	m_mapTranslate["Cancel"] = "取消";
	m_mapTranslate["Save"] = "保存";
	m_mapTranslate["ALL"] = "全部";
	m_mapTranslate["Notes"] = "文本";
	m_mapTranslate["Set Date Range"] = "设置日期范围";
	m_mapTranslate["Set Time Range"] = "设置时间范围";
	m_mapTranslate["From"] = "从";
	m_mapTranslate["To"] = "到";
	m_mapTranslate["Clear"] = "清除";
	m_mapTranslate["Do you want to save the changes?"] = "要保存吗？";
	m_mapTranslate["Prompt For Save"] = "提示保存";

	// About dialog
	m_mapTranslate["&About TinyDB..."] = "&关于 TinyDB...";
	m_mapTranslate["About TinyDB"] = "关于 TinyDB";
	m_mapTranslate["TinyDB Version 1.0"] = "TinyDB 版本1.0";
	m_mapTranslate["Developer : Sail Feng,  mailflm@163.com"] = "作者 :	冯利民, mailflm@163.com";

	// Menu
	m_mapTranslate["Copy"] = "拷贝";
	m_mapTranslate["Delete All Selected Items"] = "删除所有选中的行";
	m_mapTranslate["Insert Item Before This Row"] = "插入一行数据";
	m_mapTranslate["Append Item To Bottom"] = "增加一行数据（在最后面）";
	m_mapTranslate["Move Top"] = "移到顶端";
	m_mapTranslate["Move Up"] = "上移";
	m_mapTranslate["Move Down"] = "下移";
	m_mapTranslate["Move Bottom"] = "移到末端";
	m_mapTranslate["Select All"] = "全选";
	m_mapTranslate["Select None"] = "全不选";
	m_mapTranslate["Delete All Selected Users"] = "删除用户";
	m_mapTranslate["New User"] = "增加用户";
	m_mapTranslate["Delete All Selected Columns"] = "删除所有选中的列";
	m_mapTranslate["Insert Column"] = "插入一个列";
	m_mapTranslate["Append Column"] = "添加一个列（在最后面）";
	m_mapTranslate["Save"] = "保存";


	// Main Dialog
	m_mapTranslate["TinyDB - Welcome, %s"] = "TinyDB - 欢迎，%s";
	m_mapTranslate["Create Table"] = "新建表单";
	m_mapTranslate["Edit Table"] = "编辑表单";
	m_mapTranslate["Delete Table"] = "删除表单";
	m_mapTranslate["Export"] = "导出";
	m_mapTranslate["Import"] = "导入";
	m_mapTranslate["Change Password"] = "更改密码";
	m_mapTranslate["Options"] = "选项";
	m_mapTranslate["Are you sure to delete this table?"] = "你确定要删除这个表单吗？";
	m_mapTranslate["Change password successfully!"] = "密码更改成功！";
	m_mapTranslate["Change password failed!"] = "密码更改失败！";

	// Login
	m_mapTranslate["Username"] = "用户名";
	m_mapTranslate["Password"] = "密码";
	m_mapTranslate["Login"] = "登录";


	// Create/Edit table
	m_mapTranslate["Table Property"] = "表单属性";
	m_mapTranslate["Table Name"] = "表单名";
	m_mapTranslate["Header Height"] = "表头高度";
	m_mapTranslate["Row Height"] = "行高";
	m_mapTranslate["Enable Filter"] = "启用查询";
	m_mapTranslate["Select Then Edit"] = "先选择再编辑";
	m_mapTranslate["Highlight Changed Cell"] = "高亮更改的单元格";
	m_mapTranslate["Allow Multi Selection"] = "允许选择多行";
	m_mapTranslate["Edit First Cell After New Row Added"] = "增加新行后自动编辑第一格";
	m_mapTranslate["Edit Next Cell After Hit Return"] = "回车后编辑下一格";
	m_mapTranslate["Column Definition"] = "定义列";
	m_mapTranslate["Name"] = "名字";
	m_mapTranslate["Column Name"] = "列的名字";
	m_mapTranslate["Width"] = "宽度";
	m_mapTranslate["Data Type"] = "数据类型";
	m_mapTranslate["Edit Type"] = "编辑类型";
	m_mapTranslate["Align"] = "对齐";
	m_mapTranslate["Default Value"] = "缺省值";
	m_mapTranslate["Select List"] = "选择列表（以;隔开）";
	m_mapTranslate["Create Table"] = "新建表单";
	m_mapTranslate["Edit Table"] = "编辑表单";
	m_mapTranslate["You must set at least one column!"] = "必须设置至少一个列";
	m_mapTranslate["Table name can NOT be empty!"] = "表单名字不能为空！";
	m_mapTranslate["Table name already exist!"] = "表单已存在！";

	// Change password
	m_mapTranslate["Change Password"] = "更改密码";
	m_mapTranslate["Old Password"] = "旧密码";
	m_mapTranslate["New Password"] = "新密码";
	m_mapTranslate["Confirm Password"] = "确认密码";
	m_mapTranslate["New password mismatch!"] = "新密码不匹配！";

	// Export / import
	m_mapTranslate["Table Name In The File"] = "文件中的表单名";
	m_mapTranslate["Table Name Imported(Editable)"] = "导入后的表单名（可改名）";
	m_mapTranslate["Table Name In The System"] = "系统中的表单名";
	m_mapTranslate["Table Name Exported(Editable)"] = "导出后的表单名(可改名）";
	m_mapTranslate["Please set the export file!"] = "请设置导出的文件！";
	m_mapTranslate["Password mismatch!"] = "密码不匹配！";
	m_mapTranslate["Table name %s is duplicated!"] = "表单名%s重复！";
	m_mapTranslate["Table name %s is already existed!"] = "表单名%s已存在！";
	m_mapTranslate["Please select at least one table!"] = "请至少选择一张表单！";
	m_mapTranslate["This file contains entire database information, you can NOT import it!"] = "这个文件包含整个数据库信息，您无权导入！";
	m_mapTranslate["Import All"] = "导入全部";
	m_mapTranslate["Export All"] = "导出全部";
	m_mapTranslate["Password Protect"] = "密码保护";
	m_mapTranslate["Reload"] = "重新加载";
	m_mapTranslate["Encrypt"] = "加密";
	m_mapTranslate["File"] = "文件";
	m_mapTranslate["Export Tables"] = "导出选定表单";
	m_mapTranslate["Import Tables"] = "导入选定表单";
	m_mapTranslate["Export failed!"] = "导出失败！";
	m_mapTranslate["Export to file %s successfully!"] = "导出到文件%s成功！";

	// Options
	m_mapTranslate["Property Name"] = "属性";
	m_mapTranslate["Property Value"] = "值";
	m_mapTranslate["Language"] = "语言";
	m_mapTranslate["Query"] = "查询";
	m_mapTranslate["Case Sensitive"] = "区分大小写";
	m_mapTranslate["Query while editing"] = "编辑时立即查询";
	m_mapTranslate["Auto Save"] = "自动保存";
	m_mapTranslate["Enable"] = "启用";
	m_mapTranslate["Time Interval(minutes)"] = "保存时间间隔（分）";
	m_mapTranslate["Auto Save File"] = "自动保存文件";
	m_mapTranslate["Chinese"] = "中文";
	

	// CIniDB
	m_mapTranslate["Error loading database file!"] = "装载数据库文件错误！";
	m_mapTranslate["Error loading table information!"] = "无法从数据库中获取表单信息！";
	m_mapTranslate["Can NOT find table %s!"] = "表单%s不存在!";
	m_mapTranslate["Update table access information failed!"] = "更新用户访问表失败！";
	m_mapTranslate["Error loading user information!"] = "无法从数据库中后去用户信息！";
	m_mapTranslate["Error loading user table access information!"] = "无法从数据库中获取用户权限！";
	m_mapTranslate["Wrong password!"] = "密码错误！";
	m_mapTranslate["User does NOT exist!"] = "用户不存在！";
	m_mapTranslate["The user name %s is reserved!"] = "用户名%s被保留！";
	m_mapTranslate["The user name %s is already exist!"] = "用户名%s已存在！";
	m_mapTranslate["Can NOT find table %s!"] = "无法找到表单%s！";
	m_mapTranslate["Remove table access information failed!"] = "删除用户权限信息失败！";


	m_mapTranslate["Align"] = "对齐";

}

void CMultiLanguage::Clear()
{
	m_mapTranslate.clear();
}
