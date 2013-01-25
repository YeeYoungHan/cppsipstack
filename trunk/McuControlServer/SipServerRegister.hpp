
bool CSipServer::RecvRegisterRequest( int iThreadId, CSipMessage * pclsMessage )
{
	// ��� Ŭ���̾�Ʈ�� �α����� ����Ѵ�.
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
