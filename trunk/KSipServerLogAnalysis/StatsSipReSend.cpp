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

#include "StatsSipReSend.h"
#include "LogAnalysisSetup.h"
#include "Directory.h"
#include "TimeString.h"
#include "StatsSipMethod.h"

CStatsSipReSend gclsStatsSipReSend;

CStatsSipReSend::CStatsSipReSend()
{
}

CStatsSipReSend::~CStatsSipReSend()
{
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief SIP �޽��� ������ ������ �˻��ϱ� ���� SIP �޽����� �Է��Ѵ�.
 * @param pclsLogHeader SIP �α��� �߽�/���� ������ �����ϴ� ��ü
 * @param pclsMessage		SIP �޽���
 */
void CStatsSipReSend::AddSipMessage( CLogHeader * pclsLogHeader, CSipMessage * pclsMessage )
{
	SIP_SEND_MAP::iterator	itMap;
	std::string	strKey;

	GetKey( pclsMessage, strKey );

	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		CSipSendInfo clsInfo;

		clsInfo.m_sttTime = pclsLogHeader->m_sttTime;

		m_clsMap.insert( SIP_SEND_MAP::value_type( strKey, clsInfo ) );
	}
	else
	{
		CSipReSendInfo clsInfo;

		clsInfo.m_strKey = strKey;
		clsInfo.m_strMethod = pclsMessage->m_strSipMethod;
		clsInfo.m_bReq = pclsMessage->IsRequest();
		clsInfo.m_sttTime = pclsLogHeader->m_sttTime;

		m_clsReSendList.push_back( clsInfo );

		//printf( "resend(%s)\n", strKey.c_str() );
	}
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief �ڷᱸ���� �ʱ�ȭ��Ų��.
 */
void CStatsSipReSend::Clear()
{
	m_clsMap.clear();
	m_clsReSendList.clear();
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief SIP ������ ��踦 ���Ͽ� �����Ѵ�.
 * @param pszDate ��� ���� ��¥
 */
void CStatsSipReSend::SaveFile( const char * pszDate )
{
	FILE * fd;
	std::string strFileName = gclsSetup.m_strResultFolder;
	CDirectory::AppendName( strFileName, "resend_" );
	strFileName.append( pszDate );
	strFileName.append( ".csv" );

	CStatsSipMethod clsSendList, clsRecvList;
	SIP_RESEND_LIST::iterator	itList;

	for( itList = m_clsReSendList.begin(); itList != m_clsReSendList.end(); ++itList )
	{
		if( itList->m_bReq )
		{
			clsSendList.AddSipMethod( itList->m_strMethod.c_str() );
		}
		else
		{
			clsRecvList.AddSipMethod( itList->m_strMethod.c_str() );
		}
	}

	fd = fopen( strFileName.c_str(), "w" );
	if( fd == NULL ) return;

	STATS_SIP_METHOD_MAP * pclsMap;
	STATS_SIP_METHOD_MAP::iterator	itMap;

	pclsMap = clsSendList.GetMap();
	for( itMap = pclsMap->begin(); itMap != pclsMap->end(); ++itMap )
	{
		fprintf( fd, "%s,req," UNSIGNED_LONG_LONG_FORMAT "\n", itMap->first.c_str(), itMap->second );
	}

	pclsMap = clsRecvList.GetMap();
	for( itMap = pclsMap->begin(); itMap != pclsMap->end(); ++itMap )
	{
		fprintf( fd, "%s,rep," UNSIGNED_LONG_LONG_FORMAT "\n", itMap->first.c_str(), itMap->second );
	}

	fclose( fd );
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief ������ ������ ���Ͽ� �����Ѵ�.
 * @param pszLogFileName �α� ���� �̸�
 */
void CStatsSipReSend::SaveReSendInfoFile( const char * pszLogFileName )
{
	FILE * fd;
	std::string strFileName = gclsSetup.m_strResultFolder;
	CDirectory::AppendName( strFileName, "resend_" );
	strFileName.append( pszLogFileName );

	fd = fopen( strFileName.c_str(), "w" );
	if( fd == NULL ) return;

	SIP_RESEND_LIST::iterator	itList;
	char szTime[11];

	for( itList = m_clsReSendList.begin(); itList != m_clsReSendList.end(); ++itList )
	{
		GetTimeString( itList->m_sttTime.tv_sec, szTime, sizeof(szTime) );

		fprintf( fd, "[%s] [%s]\n", itList->m_strKey.c_str(), szTime );
	}

	fclose( fd );
}

/**
 * @ingroup KSipServerLogAnalysis
 * @brief ������ �ڷᱸ���� ������ Ű�� �����Ѵ�.
 * @param pclsMessage SIP �޽���
 * @param strKey			Ű ����� ����
 */
void CStatsSipReSend::GetKey( CSipMessage * pclsMessage, std::string & strKey )
{
	std::string strCallId;
	char szSeq[101];

	strKey.clear();

	pclsMessage->GetCallId( strCallId );
	pclsMessage->m_clsCSeq.ToString( szSeq, sizeof(szSeq) );

	if( pclsMessage->IsRequest() )
	{
		strKey.append( "req" );
	}
	else
	{
		strKey.append( "res" );
	}

	strKey.append( "_" );
	strKey.append( strCallId );
	strKey.append( "_" );
	strKey.append( szSeq );
}
