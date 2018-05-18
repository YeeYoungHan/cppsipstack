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

#pragma once

#include <list>

#define RAW_FILE_GRAPH_CONTROL_CLASSNAME _T("RawFileGraphControl")

class CRawFile
{
public:
	CRawFile() : m_pData(NULL), m_iDataCount(0), m_sMax(0)
	{}

	COLORREF		m_dwColor;
	short				* m_pData;
	int					m_iDataCount;
	short				m_sMax;
};

typedef std::list< CRawFile > RAW_FILE_LIST;

class CRawFileGraphControl : public CWnd
{
	DECLARE_DYNAMIC(CRawFileGraphControl)

public:
	CRawFileGraphControl();
	virtual ~CRawFileGraphControl();

	static BOOL RegisterWindowClass();

	bool AddFile( const char * pszFileName, COLORREF dwColor );
	void SetMaxAndScroll();

private:
	RAW_FILE_LIST	m_clsList;
	short					m_sMax;
	int						m_iStartPos, m_iEndPos;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


