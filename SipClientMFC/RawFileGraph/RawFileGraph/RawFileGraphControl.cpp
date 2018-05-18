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

#include "stdafx.h"
#include "RawFileGraph.h"
#include "RawFileGraphControl.h"
#include <sys/stat.h>

IMPLEMENT_DYNAMIC(CRawFileGraphControl, CWnd)

CRawFileGraphControl::CRawFileGraphControl() : m_sMax(0), m_iStartPos(0), m_iEndPos(0)
{

}

CRawFileGraphControl::~CRawFileGraphControl()
{
	RAW_FILE_LIST::iterator itRF;

	for( itRF = m_clsList.begin(); itRF != m_clsList.end(); ++itRF )
	{
		itRF->m_iDataCount = 0;
		free( itRF->m_pData );
		itRF->m_pData = NULL;
	}
}

BOOL CRawFileGraphControl::RegisterWindowClass()
{
	WNDCLASS    sttClass;
	HINSTANCE   hInstance = AfxGetInstanceHandle();

	if( GetClassInfo( hInstance, RAW_FILE_GRAPH_CONTROL_CLASSNAME, &sttClass ) == FALSE )
	{
		sttClass.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		sttClass.lpfnWndProc      = ::DefWindowProc;
		sttClass.cbClsExtra       = 0;
		sttClass.cbWndExtra       = 0;
		sttClass.hInstance        = hInstance;
		sttClass.hIcon            = NULL;
		sttClass.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		sttClass.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		sttClass.lpszMenuName     = NULL;
		sttClass.lpszClassName    = RAW_FILE_GRAPH_CONTROL_CLASSNAME;

		if( !AfxRegisterClass( &sttClass ) )
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

bool CRawFileGraphControl::AddFile( const char * pszFileName, COLORREF dwColor )
{
	struct stat sttStat;

	if( ::stat( pszFileName, &sttStat ) == -1 )
	{
		return false;
	}

	int iFileSize = sttStat.st_size;

	if( iFileSize == 0 )
	{
		return false;
	}

	CRawFile clsRawFile;

	clsRawFile.m_dwColor = dwColor;
	clsRawFile.m_pData = (short *)malloc( iFileSize );
	if( clsRawFile.m_pData == NULL )
	{
		return false;
	}

	FILE * fd = fopen( pszFileName, "rb" );
	if( fd == NULL )
	{
		goto FUNC_ERROR;
	}

	if( fread( clsRawFile.m_pData, 1, iFileSize, fd ) != iFileSize )
	{
		goto FUNC_ERROR;
	}

	fclose( fd );

	clsRawFile.m_iDataCount = iFileSize / 2;

	for( int i = 0; i < clsRawFile.m_iDataCount; ++i )
	{
		short sValue = clsRawFile.m_pData[i];

		if( sValue < 0 )
		{
			sValue *= (-1);
		}

		if( sValue > clsRawFile.m_sMax )
		{
			clsRawFile.m_sMax = sValue;
		}
	}

	m_clsList.push_back( clsRawFile );

	return true;

FUNC_ERROR:
	free( clsRawFile.m_pData );

	return false;
}

void CRawFileGraphControl::SetMaxAndScroll()
{
	RAW_FILE_LIST::iterator itRF;
	int iCount = 0;

	m_sMax = 0;

	for( itRF = m_clsList.begin(); itRF != m_clsList.end(); ++itRF )
	{
		if( itRF->m_sMax > m_sMax )
		{
			m_sMax = itRF->m_sMax;
		}

		if( itRF->m_iDataCount > iCount )
		{
			iCount = itRF->m_iDataCount;
		}
	}

	RECT sttRect;

	GetClientRect( &sttRect );

	int iRange = iCount / sttRect.right;

	if( iCount % sttRect.right > 0 )
	{
		++iRange;
	}

	if( iRange > 0 )
	{
		ShowScrollBar( SB_HORZ, TRUE );
		SetScrollRange( SB_HORZ, 0, iRange );
	}
	else
	{
		ShowScrollBar( SB_HORZ, FALSE );
	}

	if( m_iStartPos == 0 && m_iEndPos == 0 )
	{
		m_iEndPos = sttRect.right;
	}
}

BEGIN_MESSAGE_MAP(CRawFileGraphControl, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CRawFileGraphControl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if( m_sMax == 0 )
	{
		SetMaxAndScroll();
	}

	RECT sttRect;
	int	 iCenter, iX, iY;

	GetClientRect( &sttRect );

	RAW_FILE_LIST::iterator itRF;

	iCenter = sttRect.bottom / 2;

	dc.MoveTo( 0, iCenter );
	dc.LineTo( sttRect.right, iCenter );

	short sMax = 2 * m_sMax;

	for( itRF = m_clsList.begin(); itRF != m_clsList.end(); ++itRF )
	{
		CPen clsPen( PS_SOLID, 1, itRF->m_dwColor );

		CPen * pclsPen = dc.SelectObject( &clsPen );
		dc.MoveTo( 0, sttRect.bottom / 2 );

		iX = 0;

		for( int i = m_iStartPos; i <= m_iEndPos; ++i )
		{
			if( i >= itRF->m_iDataCount ) break;

			iY = itRF->m_pData[i] * iCenter / sMax + iCenter;
			
			if( iX > 0 )
			{
				dc.LineTo( iX, iY );
			}

			dc.MoveTo( iX, iY );
			++iX;
		}

		dc.SelectObject( pclsPen );
	}
}

void CRawFileGraphControl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	RECT sttRect;

	GetClientRect( &sttRect );

	int iPos = GetScrollPos( SB_HORZ );

	if( nSBCode == SB_LINELEFT )
	{
		SetScrollPos( SB_HORZ, iPos - 1 );
	}
	else if( nSBCode == SB_LINERIGHT )
	{
		SetScrollPos( SB_HORZ, iPos + 1 );
	}
	else if( nSBCode == SB_PAGELEFT )
	{
		SetScrollPos( SB_HORZ, iPos - 1 );
	}
	else if( nSBCode == SB_PAGERIGHT )
	{
		SetScrollPos( SB_HORZ, iPos + 1 );
	}
	else if( nSBCode == SB_THUMBTRACK )
	{
		SetScrollPos( SB_HORZ, nPos );
	}

	m_iStartPos = GetScrollPos( SB_HORZ ) * sttRect.right;
	m_iEndPos = m_iStartPos + sttRect.right;

	Invalidate();

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}
