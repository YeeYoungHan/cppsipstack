package com.amc.sip;

public class Global {
	//----------------------------------------------------------------------------------------
	// General Define
	//----------------------------------------------------------------------------------------
	public final static int MSG_SEND_REGISTER_ALARM		= 30010; 
	public final static int MCS_SIP_UPDATE_CLICKTODIAL 	= 30011;
	
	public final static int MILI_SECOND							= 1000;
	
	//----------------------------------------------------------------------------------------
	// call response type
	//----------------------------------------------------------------------------------------
	public final static int SIP_CALL_RTYPE_OK							= 200;	
	public final static int SIP_CALL_RTYPE_TEMP_UNAVAILABLE	= 480;	
	public final static int SIP_CALL_RTYPE_BUSY						= 486;	
	public final static int SIP_CALL_RTYPE_NOT_ACCEPT			= 488;	
	public final static int SIP_CALL_RTYPE_DECLINE					= 603;	

	
	//----------------------------------------------------------------------------------------
	// Audio or Video Mode
	//----------------------------------------------------------------------------------------
	public final static int AUDIO_MODE					= 0;
	public final static int VIDEO_MODE					= 1;

	
	//----------------------------------------------------------------------------------------
	// Primary and secondary stack index
	//----------------------------------------------------------------------------------------
	public final static int MAX_STACK_NUM			= 2;
	public final static int STACK_PRIMARY			= 0;
	public final static int STACK_SECONDARY		= 1;
	
	
	//----------------------------------------------------------------------------------------
	// Audio or Video Mode
	//----------------------------------------------------------------------------------------
	public final static int MCS_AUDIO_PCMU				=	0;
	public final static int MCS_AUDIO_PCMA				=	8;
	public final static int MCS_AUDIO_G729				=	18;
	public final static int MCS_AUDIO_iPCMWB			= 	97;
	public final static int MCS_AUDIO_iLBC				=	98;
	public final static int MCS_AUDIO_DTMF				=	101;


	//----------------------------------------------------------------------------------------
	// Define for Transfer
	//----------------------------------------------------------------------------------------
	public final static int CALL_TRANSFER_REQ			= 3000;
	public final static int CALL_TRANSFER_READY_OK 		= 3001;
	public final static int CALL_TRANSFER_READY_FAIL 	= 3002;
	public final static int CALL_TRANSFER_OK 			= 3003;
	public final static int CALL_TRANSFER_FAIL 			= 3004;	
	
	

	//----------------------------------------------------------------------------------------
	// Define for sRTP
	//----------------------------------------------------------------------------------------
	public final static int SRTP_MAX_SIZE					= 100;
	public final static int SRTP_SALT_KEY_SIZE			= 14;
	
	
	public final static int NONE_CRYPTO_SUIT 			= 0;
	public final static int AES_CRYPTO_SUIT_128			= 1;
	public final static int AES_CRYPTO_SUIT_192			= 2;
	public final static int AES_CRYPTO_SUIT_256			= 3;
	public final static int ARIA_CRYPTO_SUIT_128		= 4;
	public final static int ARIA_CRYPTO_SUIT_192		= 5;
	public final static int ARIA_CRYPTO_SUIT_256		= 6;

	public final static int NONE_SRTP 						= 0;
	public final static int AES_128 							= 0x80;
	public final static int AES_192 							= 0x40;
	public final static int AES_256 							= 0x20;
	public final static int ARIA_128 							= 0x08;
	public final static int ARIA_192 							= 0x04;
	public final static int ARIA_256 							= 0x02;

	public static String SRTP_INLINE							= "inline";
	
	public static String[] CryptoSuit = {
													"NONE_CRYPT",
													"AES_CM_128_HMAC_SHA1_80",
													"AES_CM_192_HMAC_SHA1_80",
													"AES_CM_256_HMAC_SHA1_80",
													"ARIA_CM_128_HMAC_SHA1_80",
													"ARIA_CM_192_HMAC_SHA1_80",
													"ARIA_CM_256_HMAC_SHA1_80"};
	
	
	
	public final static int DTMF_INBAND					= 0;
	public final static int DTMF_RFC2833					= 1;
	

	//----------------------------------------------------------------------------------------
	// General Define
	//----------------------------------------------------------------------------------------
	public final static int GIPS_SET_SRTP_ENABLE		= 70002;
	

	
	
	//----------------------------------------------------------------------------------------
	// Call Fail Errors
	//----------------------------------------------------------------------------------------
	public final static int ERR_NOTHING						= 0;
	public final static int ERR_FTN_CALL_FAIL				= 1;
	
	// initialize()
	//-
	//-public final static int ERR_EVENT_CREATE_FAIL			=	1005;	// �̺�Ʈ ���� ����
	//-public final static int ERR_GET_ADAPTER_INFO_CALL_FAIL	=	1006;	// GetAdaptersInfo() ȣ�� ����
	//-public final static int ERR_LOCAL_IP_NULL				=	1007;	// Local IP ����
	public final static int ERR_SIZE_NULL					=	1008;	// �Ű����� �� ���� ���� �׸��� ����
	//-public final static int ERR_SIZE_OVERFLOW				=	1009;	// �Ű����� �� ���� �ʹ� ū �׸��� ����
	//-public final static int ERR_INVALID_ARGUMENT			=	1010;	// �Ű����� �� �ùٸ��� �ʴ� ���� ����
	public final static int ERR_OTHER_ALREADY_REQUESTCALL =  1017;   // ���� �߽��Ѱͺ��� ������ ���� �߽��� ���
	
	//-public final static int ERR_SIP_SVR_NOT_LOGIN			=	2000;	// SIP ������ �α��ε��� �ʾ���
	//-public final static int ERR_SIP_SVR_INFO_NULL			=	2001;	// SIP ���� ���� ����
	//-public final static int ERR_SIP_UA_CREATE_FAIL			=	2002;	// SIP ������ UA ���� ����
	public final static int ERR_SIP_UA_INIT_FAIL			=	2003;	// SIP ������ UA �ʱ�ȭ ����
	public final static int ERR_SIP_UA_FTN_CALL_FAIL		=	2004;	// SIp ������ UA �Լ� ȣ�� ����� FALSE ��
	
	public final static int ERR_CANNOT_CALL_TO_MYSELF		=	4000;	// �ڱ� �ڽſ��� ��ȭ�ɷ��� ��				(Function Return)
	public final static int ERR_NO_CODEC_INFO				=	4001;	// �ڵ� ���� ����							(Function Return)
	//-public final static int ERR_SDP_NULL					=	4002;	// ���� SDP ����
	public final static int ERR_SDP_MAKE_FAIL				=	4003;	// SDP ���� ����
	//-public final static int ERR_SDP_PARSE_FAIL				=	4004;	// SDP �Ľ� ����
	public final static int ERR_SDP_NEGO_FAIL				=	4005;	// SDP ���� ���� (����� �ڵ� ����)
	//-public final static int ERR_ACK_SEND_FAIL				=	4006;	// ACK �޽��� ���� ����
	public final static int ERR_USER_REJECT					=	4007;	// ������ ������
	public final static int ERR_USER_BUSY					=	4008;	// ��ȭ�� ����
	public final static int ERR_USER_NOT_ACCEPT				=	4009;	// ������ ���� ����

	
	//----------------------------------------------------------------------------------------
	// Message to send UI
	//----------------------------------------------------------------------------------------
	public final static int MCS_NONE = 0;							//  0, �� ����
	public final static int SIP_INV_SEND_RET_FAILED			= 3;    //send
	/* �α��� */	
	public final static int MCS_SIP_REGISTER_OK				= 10;	// 10, SIP ���� �α��� ���� 
	public final static int MCS_SIP_REGISTER_FAIL				= 11;	// 11, SIP ���� �α��� ����
	/* �α׾ƿ� */
	public final static int MCS_SIP_LOGOUT_OK				= 20;	// 20, SIP ���� �α׾ƿ� ����
	public final static int MCS_SIP_LOGOUT_FAIL				= 21;	// 21, SIP ���� �α׾ƿ� ����
	/* ��ȭ*/
	public final static int MCS_CALL_RINGING				= 70;	// 70, ��ȭ ��û ����					(�۽���)
	public final static int MCS_CALL_REQUEST				= 71;						// 71, ��ȭ ��û�� ����						(������)
	public final static int MCS_CALL_CONNECT_OK				= 72;				// 72, ��ȭ ���� ����									(����)
	public final static int MCS_CALL_CONNECT_FAIL			= 73;					// 73, ��ȭ ���� ����					(�۽���)
	public final static int MCS_CALL_BYE_REQUEST			= 74;					// 74, ��ȭ ���� ��û ����						(������)
	public final static int MCS_CALL_CANCEL_REQUEST			= 75;				// 75, ��ȭ ��� ��û ����						(������)
	//
	public final static int MCS_CALL_HOLD_ON				= 76;				// 76, ��ȭ HOLD ������					(��û�ϴ� ��)
	public final static int MCS_CALL_HOLD_OFF				= 77;				// 77, ��ȭ HOLD ������					(��û�ϴ� ��)
	public final static int MCS_CALL_HOLD_ON_REQUEST		= 78;				// 78, ��ȭ HOLD ������					(��û���ϴ� ��)
	public final static int MCS_CALL_HOLD_OFF_REQUEST		= 79;				// 79, ��ȭ HOLD ������					(��û���ϴ� ��)
	public final static int MCS_CALL_HOLD_FAIL				= 80;						// 80, ��ȭ HOLD ���� Action ����
	//
	public final static int MCS_CALL_TRANSFER_READY_OK		= 85;				// 85, Call Transfer �غ� �Ϸ�
	public final static int MCS_CALL_TRANSFER_READY_FAIL	= 86;			// 86, Call Transfer �غ� ����
	public final static int MCS_CALL_TRANSFER_OK			= 87;					// 87, Call Transfer ����
	public final static int MCS_CALL_TRANSFER_FAIL			= 88;					// 88, Call Transfer ����
	//
	
	//----------------------------------------------------------------------------------------
	// invite sip signals for ui 
	//----------------------------------------------------------------------------------------
	public final static int MCS_CALL_RCV_UPDATE				= 1009;
	public final static int MCS_CALL_RCV_INVITE					= 1100;
	public final static int MCS_CALL_RCV_RE_INVITE			= 1101;
	public final static int MCS_CALL_RCV_INVITE_200			= 1102;
	public final static int MCS_CALL_RCV_INVITE_ACK			= 1103;
	public final static int MCS_CALL_SND_INVITE_200			= 1104;
	public final static int MCS_CALL_RCV_REINVITE_200		= 1105;
	public final static int MCS_CALL_SND_REINVITE_200		= 1106;

	
	public final static int MCS_RCV_MESSAGE						= 1107;
	public final static int MCS_RCV_MESSAGE_200				= 1108;
	public final static int MCS_RCV_MESSAGE_FAIL				= 1109;
	public final static int MCS_RCV_MESSAGE_202				= 1110;
	
	

	//--- 20110220 : han
	public final static int MCS_CALL_100TRYING				= 100;	// 100, 100 Trying
	public final static int MCS_CALL_180RINGING				= 180;	// 180 Ringing
	public final static int MCS_CALL_181CALLFORWARDING		= 181;	// 181 Call Forwarding
	public final static int MCS_CALL_183SESSION_PROGRESS	= 183;	// Session Progress

	public final static int MCS_CALL_NOTIFY					= 123;	// 123, Notify �޽���

	public final static int MCS_CALL_CANCEL_FAIL			= 124;	// 124, ���� ���ŵ�
	public final static int MCS_CALL_BYE_FAIL					= 125;	// 125, ���� ���ŵ�
	
	public final static int MCS_MESSAGE_INCOMING			= 126;	// 126, MESSAGE(SMS) ����
	public final static int MCS_MESSAGE_RET_OK					= 127;	// 127, MESSAGE�� ���� 200 OK
	public final static int MCS_MESSAGE_RET_FAIL				= 128;	// 128, MESSAGE�� ���� fail

	public final static int MCS_CALL_NOTIFY_MWI				= 129; //MWI (voice mail info)

	public final static int MCS_CALL_MISSED_CALL				= 130;		// 130, missed call
	
	//20120201 bk: added
	public final static int MCS_CALL_INFO_INCOMING				= 131; //received a INFO request
	public final static int MCS_CALL_INFO_RET_OK					= 132; //received a INFO 200 ok
	public final static int MCS_CALL_INFO_RET_FAIL				= 133; //received a INFO fail reponse 

	
	//----------------------------------------------------------------------------------------
	// 
	//----------------------------------------------------------------------------------------
	/** �� ���ǵ� Audio/Video Payload types (PT) : RFC3551 ���� (�Ľ� �� Ÿ���Ǻ���).
	 *
	 * "m=" ���� Payload Type �� ������ ID �� �ο�. �� ���� ���� major/minor mime type �� �ڵ� �Ǻ��ȴ�.
	 *
	 *  �ּ���Ģ: " number: [mimeType mimeSubType/Clock Rate/Channel Number] "
	 *
	 *  number �� �� ����� ��� payload type number �� 96 ~ 127 ���̿��� ���� �����ؾ� �ϴ� �����
	 *
	 * �߰������� application type(ê,ȭ��Ʈ����,ȭ�����,��������) Ÿ�Կ� ���� id�� �ο�
	 */
	
	public final static int SDP_CODEC_UNKNOWN			= -1;		//!< UNKNOWN TYPE (�ű������� ����)
	public final static int SDP_CODEC_PCMU				= 0;	//!< 0: G.711 mu-law (A PCMU/8000/1)
	public final static int SDP_CODEC_GSM				= 3;	//!< 3: A GSM/8000/1
	public final static int SDP_CODEC_G723				= 4;	//!< 4: A G723/8000/1
	public final static int SDP_CODEC_DVI4_8KHZ			= 5;	//!< 5: A DVI4/8000/1
	public final static int SDP_CODEC_DVI4_16KHZ		= 6;	//!< 6: A DVI4/16000/1
	public final static int SDP_CODEC_LPC				= 7;	//!< 7: A LPC/8000/1
	public final static int SDP_CODEC_PCMA				= 8;	//!< 8: G.711 a-law (A PCMA/8000/1)
	public final static int SDP_CODEC_G722				= 9;	//!< 9: A G722/8000/1
	public final static int SDP_CODEC_L16_2CH			= 10;	//!< 10: PCM 16 bit/sample 44100 samples/sec. (A L16/44100/2)
	public final static int SDP_CODEC_L16_1CH			= 11;	//!< 11: PCM 16 bit/sample 44100 samples/sec. (A L16/44100/1)
	public final static int SDP_CODEC_QCELP				= 12;	//!< 12: A QCELP/8000/1
	public final static int SDP_CODEC_CN				= 13;	//!< 13: A CN/8000/1
	public final static int SDP_CODEC_MPA				= 14;	//!< 14: A MPA/90000/??)
	public final static int SDP_CODEC_G728				= 15;	//!< 15: A G728/8000/1
	public final static int SDP_CODEC_DVI4_11KHZ		= 16;	//!< 16: A DVI4/11025/1)
	public final static int SDP_CODEC_DVI4_22KHZ		= 17;	//!< 17: A DVI4/22050/1)
	public final static int SDP_CODEC_G729				= 18;	//!< 18: G.729; with or without Annexes A or B (A G729/8000/1)
	public final static int SDP_CODEC_CeIB				= 25;	//!< 25: V CeIB/90000
	public final static int SDP_CODEC_JPEG				= 26;	//!< 26: V JPEG/90000
	public final static int SDP_CODEC_nv				= 28;	//!< 28: V nv/90000
	public final static int SDP_CODEC_H261				= 31;	//!< 31: V H261/90000
	public final static int SDP_CODEC_MPV				= 32;	//!< 32: V MPV/90000
	public final static int SDP_CODEC_MP2T				= 33;	//!< 23: V MP2T/90000
	public final static int SDP_CODEC_H263				= 34;	//!< 34: V H263/90000

	// 96 ~ 127 �� dynamic ...
	public final static int SDP_CODEC_G726_40			= 96;	//!< A G726-40/8000/1
	public final static int SDP_CODEC_G726_32			= 97;	//!< A G726-32/8000/1
	public final static int SDP_CODEC_G726_24			= 98;	//!< A G726-24/8000/1
	public final static int SDP_CODEC_G726_16			= 999;	//!< A G726-16/8000/1
	public final static int SDP_CODEC_G729D				= 100;	//!< A G729D/8000/1
	public final static int SDP_CODEC_G729E				= 101;	//!< A G729E/8000/1
	public final static int SDP_CODEC_GSM_EFR			= 102;	//!< A GSM-EFR/8000/1
	public final static int SDP_CODEC_L8				= 103;	//!< A L8/var/var
	public final static int SDP_CODEC_RED				= 104;	//!< A RED
	public final static int SDP_CODEC_VDVI				= 105;	//!< A VDVI/var/1
	public final static int SDP_CODEC_G7221				= 106;	//!< A G.722.1/16000/1
	public final static int SDP_CODEC_TEL_EVENT			= 107;	//!< A telephone-event	[RFC2833]
	public final static int SDP_CODEC_TONE				= 108;	//!< A tone				[RFC2833]
	public final static int SDP_CODEC_H263_1998			= 109;	//!< V H263-1998/90000
	/** H.264 : V H264/90000 (rfc3984)
	The OPTIONAL parameters "profile-level-id", "max-mbps", "max-fs",
	"max-cpb", "max-dpb", "max-br", "redundant-pic-cap", "sprop-
	parameter-sets", "parameter-add", "packetization-mode", "sprop-
	interleaving-depth", "deint-buf-cap", "sprop-deint-buf-req",
	"sprop-init-buf-time", "sprop-max-don-diff", and "max-rcmd-nalu-
	size", when present, MUST be included in the "a=fmtp" line of SDP.

	\code
	// example
	m=video 49170 RTP/AVP 98
	a=rtpmap:98 H264/90000
	a=fmtp:98 profile-level-id=42A01E;
	sprop-parameter-sets=Z0IACpZTBYmI,aMljiA==
	\endcode
	*/
	public final static int SDP_CODEC_H264				= 110;


}



