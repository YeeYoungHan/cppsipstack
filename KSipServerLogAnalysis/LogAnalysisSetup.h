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
 * @brief KSipServerLogAnalysis 설정 파일의 내용을 저장하는 클래스
 */
class CLogAnalysisSetup
{
public:
	CLogAnalysisSetup();
	~CLogAnalysisSetup();

	/** 파일 저장 폴더 */
	std::string m_strResultFolder;

	/** 로그 폴더 */
	std::string	m_strLogFolder;

	bool Read( const char * pszFileName );
};

extern CLogAnalysisSetup gclsSetup;

#endif
