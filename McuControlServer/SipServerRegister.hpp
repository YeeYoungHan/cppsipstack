
bool CSipServer::RecvRegisterRequest( int iThreadId, CSipMessage * pclsMessage )
{
	// 모든 클라이언트의 로그인을 허용한다.
	CSipFrom clsContact;

	gclsUserMap.Insert( pclsMessage, &clsContact );

	CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_OK );
	if( pclsResponse )
	{
		pclsResponse->m_clsContactList.push_back( clsContact );

		gclsSipStack.SendSipMessage( pclsResponse );
		return true;
	}

	return false;
}
