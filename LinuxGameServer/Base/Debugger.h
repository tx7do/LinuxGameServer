/**********************************************************************
* Copyright (C) 2008 - ʢ������ - All Rights Reserved
*
* �ļ�����:		Debugger.h
* ժ    Ҫ:		������
*     
* ��    ��:		yanglinbo, 
* ��    ��:		�鿴�ļ����·�.
* 
***********************************************************************/

#pragma once


#include <string>


//////////////////////////////////////////////////////////////////////////
/// ������
//////////////////////////////////////////////////////////////////////////
class Debugger
{
public:
	/// ���������Ƿ����
	static bool isAvailable();

	/// ������������Ϣ
	static void message(const std::string& msg);
	static void message(const std::string& msg, const char* file, int line);

	/// ��ͣ����ִ�У��򿪵��������������ģʽ��
	static void enter();
	static void enter(const std::string& msg);
	static void enter(const std::string& msg, const char* file, int line);
	static void enter(const char* file, int line);
};
