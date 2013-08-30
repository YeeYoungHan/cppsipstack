#pragma once

#include "SipUserAgentCallBack.h"

class CSipUserAgentMFC : public ISipUserAgentCallBack
{
public:
	CSipUserAgentMFC();
	virtual ~CSipUserAgentMFC();

	virtual void EventRegister( CSipServerInfo * pclsInfo, int iStatus );
	virtual bool EventIncomingRequestAuth( CSipMessage * pclsMessage );
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp );
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus );
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual bool EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer );
	virtual bool EventBlindTransfer( const char * pszCallId, const char * pszReferToId );
	virtual bool EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage );
	virtual void EventCallBackThreadEnd( int iThreadId );
};
