/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef _LOG_ANALYSIS_SETUP_H_
#define _LOG_ANALYSIS_SETUP_H_

#include <string>

/**
 * @ingroup KSipServerLogAnalysis
 * @brief KSipServerLogAnalysis ���� ������ ������ �����ϴ� Ŭ����
 */
class CLogAnalysisSetup
{
public:
	CLogAnalysisSetup();
	~CLogAnalysisSetup();

	/** DB ���� IP �ּ� */
	std::string	m_strDbHost;

	/** DB ���� ���� ����� ���̵� */
	std::string m_strDbUserId;

	/** DB ���� ���� ��й�ȣ */
	std::string m_strDbPassWord;

	/** DB ���� �����ͺ��̽� �̸� */
	std::string m_strDbName;

	/** DB ���� ��Ʈ ��ȣ */
	int					m_iDbPort;

	/** �α� ���� */
	std::string	m_strLogFolder;

	bool Read( const char * pszFileName );
};

extern CLogAnalysisSetup gclsSetup;

#endif
