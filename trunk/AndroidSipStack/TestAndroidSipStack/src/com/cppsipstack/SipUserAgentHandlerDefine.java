package com.cppsipstack;

public interface SipUserAgentHandlerDefine
{
	final String COMMAND = "command";
	
	final String EVENT_REGISTER = "EventRegister";
	final String EVENT_INCOMING_CALL = "EventIncomingCall";
	final String EVENT_CALL_RING = "EventCallRing";
	final String EVENT_CALL_START = "EventCallStart";
	final String EVENT_CALL_END = "EventCallEnd";
	final String EVENT_REINVITE = "EventReInvite";
	final String EVENT_TRANSFER = "EventTransfer";
	final String EVENT_BLIND_TRANSFER = "EventBlindTransfer";
	
	final String ARG_CALL_ID = "strCallId";
	final String ARG_INFO = "clsInfo";
	final String ARG_STATUS = "iStatus";
	final String ARG_FROM = "strFrom";
	final String ARG_TO = "strTo";
	final String ARG_RTP = "clsRtp";
	final String ARG_SIP_STATUS = "iSipStatus";
	final String ARG_REFER_TO_CALL_ID = "strReferToCallId";
	final String ARG_SCREENED_TRANSFER = "bScreenedTransfer";
	final String ARG_REFER_TO_ID = "strReferToId";
}
