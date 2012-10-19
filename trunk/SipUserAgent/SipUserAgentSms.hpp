

bool CSipUserAgent::SendSms( const char * pszFrom, const char * pszTo, const char * pszText, CSipCallRoute * pclsRoute )
{
	CSipMessage * pclsRequest = new CSipMessage();
	if( pclsRequest == NULL ) return false;

	pclsRequest->m_strSipMethod = "MESSAGE";
	pclsRequest->m_clsReqUri.Set( "sip", pszTo, gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort );

	pclsRequest->m_clsFrom.m_clsUri.Set( "sip", pszFrom, gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort );
	pclsRequest->m_clsFrom.InsertTag();

	pclsRequest->m_clsTo.m_clsUri.Set( "sip", pszTo, gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort );

	pclsRequest->m_clsCSeq.m_iDigit = 1;
	pclsRequest->m_clsCSeq.m_strMethod = pclsRequest->m_strSipMethod;

	return true;
}
