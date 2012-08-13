package com.amc.sip;

import java.util.Random;
import java.util.StringTokenizer;
import android.util.Log;

public class SipManager {	
	public static String TAG = "SMV";
	
	public static int m_nLogLevel 				= 1;
	public static int m_nAuthRegSendCount[] 	= new int[Global.MAX_STACK_NUM];
	public static int m_nAuthInvSendCount[] 	= new int[Global.MAX_STACK_NUM];
	public static int m_nAuthRefSendCount[] 	= new int[Global.MAX_STACK_NUM];
	public static int m_nExpires 						= 0;
	public static int m_AudioCodecCount 		= 0;
	public static int m_nMediaSPort				= 0;
	public static int m_nMediaEPort				= 0;
	public static int m_nLastPortNum			= 0;
	public static int m_nSndSRTP				= 0;
	public static int m_nRcvSRTP				= 0;
	public static String m_strInvProxyCredentials 	= "";
	public static String m_strInvCredentials 		= "";
	public static String strRegID[] 			= new String[Global.MAX_STACK_NUM];
	public static String strRegPW[] 			= new String[Global.MAX_STACK_NUM];
	public static String strRegIP[] 			= new String[Global.MAX_STACK_NUM];
	public static String m_strVersion 			= "SMV SIP Test";
	public static int m_nMyDTMF					= Global.DTMF_RFC2833;
	
	
    static {
		//-----------------------------------------------------------
    	// Load SipUA Library
		//-----------------------------------------------------------
    	System.loadLibrary("SipUA_so");
    }

	public SipManager(UIInterface interf, Object syncObj)
	{
	}
	
	public void close()
	{
		return;
	}
	
	public int Initialize(int nStack, 
									String id, 
									String authId, 
									String authPwd, 
									int nRecvPort,
									int nSendPort,
									String svrIp, 
									String domain, 
									int nMediaSPort, 
									int nMediaEPort,
									int nProto,
									String localIp,
									String callId,
									int nExpire,
									String strLogPath,
									int keepCount)
	{
		m_nMediaSPort = nMediaSPort;
		m_nMediaEPort = nMediaEPort;
		m_nExpires = nExpire;

		
		Log.d(TAG, "[SIP] Initialize(..) Stack: " + nStack + ", id: " + id + ", authId: " + authId + ", authPwd: " + authPwd + ", domain: " + domain);

		
		m_nLogLevel = 1;
		
		if (domain.length() > 0)
		{


		}
		else
		{
			Log.e(TAG, "A domain info does not exist.");
		}
		
		return Global.ERR_NOTHING;
	}
	

	public boolean Register(int nStack, boolean bKeepAlive)
	{	
		boolean bRet = true;
		return bRet;
	}


	public boolean UnRegister(int nStack)
	{
		boolean bRet = true;
		return bRet;
	}
	
	public static int GetPortNumber()
	{
		int nRandomNum;
		
		if (m_nMediaSPort<=0 || m_nMediaEPort<=0)
			return 0;

		Random rand = new Random();
		while (true)
		{
			nRandomNum = rand.nextInt() + m_nMediaSPort;
			nRandomNum %= m_nMediaEPort;
			
			if (nRandomNum >= m_nMediaSPort
					&& nRandomNum <= m_nMediaEPort)
			{
				break;
			}
		}
		
		rand = null;

		return nRandomNum;
	}
	

	public String ExtractSubString(String strS, int nPos, String cTok)
	{
		int nCount = 0;
		
//		Log.d(TAG, "ExtractSubString(..) codecList: " + strS);
		
		String strT = "";
		String strToken = "";	
		StringTokenizer st = new StringTokenizer(strS, cTok);
		while (st.hasMoreTokens())
		{
			strToken = st.nextToken();
			
			if (nCount == nPos)
			{
				strT = strToken;
				st = null;
				return strT;
			}
			nCount++;
		}
		
		st = null;
		
		return strT;
	}
	
	public void setSrtpType(int nSrtpType)
	{
		m_nSndSRTP = nSrtpType;
		m_nRcvSRTP = 0x00;	
	}
	
	/*
	//20110701 bk
	public static void createSndSRTPKey(CallInfo callInfo)
	{
		for (int i=0; i<8; i++)
		{
			int nSrtpType = m_nSndSRTP & (0x01 << i);
			if (nSrtpType == 0)
				continue;

			int nKeySize = GetMasterKeySize(nSrtpType);

			byte[] btTmpMaster = GetMasterKey(nKeySize);
			byte[] btTmpSalt = GetSaltKey();
			
			int nTotalSize = nKeySize+Global.SRTP_SALT_KEY_SIZE;
			byte[] concatenatedKey = new byte[nTotalSize];

			for (int k=0, nCntMast=0, nCntSalt=0; k<nTotalSize; k++)
				concatenatedKey[k] = (k<nKeySize)? btTmpMaster[nCntMast++] : btTmpSalt[nCntSalt++];
			
			if ((nSrtpType & 0xF0) != 0)
				callInfo.btMyAESKey = concatenatedKey;
			else if ((nSrtpType & 0x0F) != 0)
				callInfo.btMyARIAKey = concatenatedKey;
			
			btTmpMaster = null;
			btTmpSalt = null;
		}
	}
	
		
	public static boolean getRcvSRTPKey(String srtpKey, int nCryptType, SdpNegoResult negoResult)
	{
		boolean bRet = true;
		
		Log.d(TAG, "[SIP] SRTP Type:"+nCryptType+", Key(Base64):"+srtpKey);
		
		byte[] DecryptedKey = decodeBase64(srtpKey);
		
		if (DecryptedKey == null)
			return false;
		
		if ((nCryptType & 0xF0) != 0)
		{
			negoResult.SrtpKeyAES = new byte[DecryptedKey.length];
			System.arraycopy(DecryptedKey, 0, negoResult.SrtpKeyAES, 0, DecryptedKey.length);
		}
		else if ((nCryptType & 0x0F) != 0)
		{
			negoResult.SrtpKeyARIA = new byte[DecryptedKey.length];
			System.arraycopy(DecryptedKey, 0, negoResult.SrtpKeyARIA, 0, DecryptedKey.length);
		}
		
		String strLog = "";
		for (int m=0; m<DecryptedKey.length; m++)
			strLog += DecryptedKey[m] + " ";
		Log.e(TAG, "[SIP] SRTP DecryptedKey("+DecryptedKey.length+" size):" + strLog);
		
		DecryptedKey = null;
		
		return bRet;
	}
	
	
	public static String encodeBase64(byte[] source)
	{
		String result = Base64Util.base64Encode(source);
		return result;
	}
	
	public static byte[] decodeBase64(String source)
	{
		byte[] result = Base64Util.base64Decode(source);
		return result;
	}

	public boolean MakeSDP(CallInfo callInfo)
	{
		int nCount = 0;		
		callInfo.sndSdpBody = new CSdpBodyWrapper();
		
		setSdpBodyCommon(callInfo.nStack, callInfo.sndSdpBody);
		
		// 포트번호 생성
		callInfo.nAPort = (audCodecList.size() > 0)? GetPortNumber() : 0;
		if (callInfo.nAPort <= 0)
			return false;

		String strTempPayloadType = "";
		String strTempSpace = "";
		for (int i=0; i<audCodecList.size(); i++)
		{
			strTempSpace = (i==0)? "" : " ";
			strTempPayloadType += strTempSpace + audCodecList.get(i).nPayloadType;
		}
		
		//--------------------------------------------------------------
		// SRTP media
		//--------------------------------------------------------------
		if (m_nSndSRTP > 0) 
		{
			String KeyParam = "";
			
			createSndSRTPKey(callInfo);
			
			if (callInfo.sndSdpBody.addMedia(Global.SDP_CODEC_UNKNOWN, 
					callInfo.nAPort, 
					"RTP/SAVP", 
					"audio", 
					strTempPayloadType) == false)
			{
				Log.d(TAG, "[SIP] Fail to add a SdpMedia..");
				return false;
			}
			
			for (int t = 0; t < audCodecList.size(); t++)
			{
				//Frequency
				int nFreq = 0;
				if (audCodecList.get(t).nPayloadType == 97)
					nFreq = 16000;
				else
					nFreq = 8000;
				
				String strPayLoadType = "" + audCodecList.get(t).nPayloadType;
				callInfo.sndSdpBody.addMediaFormat(nCount, strPayLoadType, audCodecList.get(t).strCodecName, nFreq);
			}

			int nCryptoCnt = 0;
			for (int i=0; i<8; i++)
			{
				int nSrtpType = m_nSndSRTP & (0x01 << i);
				if (nSrtpType == 0)
					continue;

				nCryptoCnt++;

				if (nSrtpType != 0)
				{
					//20110407 bk: crytosuit
					String encryptName = getCryptoSuitString(nSrtpType);
					
					// create crypto attribute
					if ((nSrtpType & 0x0F) != 0)
					{
						KeyParam = Global.SRTP_INLINE + ":" + encodeBase64(callInfo.btMyARIAKey);
						callInfo.sndSdpBody.addMediaFormat(nCount, null, null, -1, -1, null, nCryptoCnt, encryptName, KeyParam);
					}
					else if ((nSrtpType & 0xF0) != 0)
					{
						KeyParam = Global.SRTP_INLINE + ":" + encodeBase64(callInfo.btMyAESKey);
						callInfo.sndSdpBody.addMediaFormat(nCount, null, null, -1, -1, null, nCryptoCnt, encryptName, KeyParam);
					}
				}
			}
			
			//---------------------------------------------------------
			//20120405 bk: for ptime testing
			callInfo.sndSdpBody.addMediaFormat(nCount, "ptime", "20", "");
			//---------------------------------------------------------
			
			
			callInfo.sndSdpBody.addMediaFormat(nCount, "sendrecv", "", "");
			
			nCount++;
		}
				
		//--------------------------------------------------------------
		// RTP media
		//--------------------------------------------------------------
		{
			if (callInfo.sndSdpBody.addMedia(Global.SDP_CODEC_UNKNOWN, 
												callInfo.nAPort, 
												"RTP/AVP", 
												"audio", 
												strTempPayloadType) == false)
			{
				Log.d(TAG, "[SIP] Fail to add a SdpMedia..");
				return false;
			}
			
			for (int t = 0; t < audCodecList.size(); t++)
			{
				//Frequency
				int nFreq = 0;
				if (audCodecList.get(t).nPayloadType == 97)
					nFreq = 16000;
				else
					nFreq = 8000;
				
				String strPayLoadType = "" + audCodecList.get(t).nPayloadType;
				callInfo.sndSdpBody.addMediaFormat(nCount, strPayLoadType, audCodecList.get(t).strCodecName, nFreq);
			}
			
			//---------------------------------------------------------
			//20120405 bk: for ptime testing
			callInfo.sndSdpBody.addMediaFormat(nCount, "ptime", "20", "");
			//---------------------------------------------------------
			
			callInfo.sndSdpBody.addMediaFormat(nCount, "sendrecv", "", "");
		}
		
		Log.i(TAG, callInfo.sndSdpBody.getSdpBodyRawMessage());
		
		return true;
	}
*/
	//20110523 bk
	//aria's priority is higher than aes.
	//aes is default
	public static int getNegoSRTPType(int nSndType, int nRcvType)
	{
		int nNegoType = 0;

		if ((nSndType & nRcvType) != 0)
		{
			for (int m=0; m<8; m++)
			{
				int nMask = 0x01 << m;
				if (((nSndType & nMask) != 0) && ((nRcvType & nMask) != 0))
				{
					nNegoType = nMask;
					break;
				}
			}
		}

		return nNegoType;
	}
	
	public void SetCurrentVersion(String strVer)
	{
		Log.d(TAG, "[SIP] Version: "+strVer);
		m_strVersion = strVer;
	}
	
	/********************************************************************************************************
	 * MCS_SetAudioCodecInfo
	 * Description: 음성 코덱 정보 설정
	 * Params:		dsAudioCodec			[IN]	음성 코덱 정보 구조체
	 * return:		ERR_NOTHING				[OUT]	설정 성공
	 *				그 외					[OUT]	실패
	 */
	public int SetAudioCodecInfo(String strCodecList, int dtmf) 
	{
		int nRetVal = Global.ERR_NOTHING;
		
		String strTok = "";
		int nCnt = 0;
		
		//clear codec list
		//audCodecList.clear();
		
		//to parse codec list
		do
		{
			strTok = ExtractSubString(strCodecList, nCnt, ";");
			if (strTok.length() == 0)
				break;
			
			nCnt++;
		} while (true);
		
		m_nMyDTMF = dtmf;		

		return nRetVal;
	}

	


	//----------------------------------------------------------
	// Create Master Key by rand()
	//----------------------------------------------------------
	public static byte[] GetMasterKey(int nSize)
	{
		if (nSize > Global.SRTP_MAX_SIZE)
			return null;

		byte[] btMasterKey = new byte[Global.SRTP_MAX_SIZE];

		Random rand = new Random();
		for (int i=0; i<nSize; i++)
		{
			btMasterKey[i] = (byte)(rand.nextInt() % 255);
		}
		
		rand = null;
		
		return btMasterKey;
	}

	//----------------------------------------------------------
	// Create Salt Key by rand()
	//----------------------------------------------------------
	public static byte[] GetSaltKey()
	{
		byte[] btSaltKey = new byte[Global.SRTP_MAX_SIZE];

		Random rand = new Random();
		for (int i=0; i<Global.SRTP_SALT_KEY_SIZE; i++)
		{
			btSaltKey[i] = (byte)(rand.nextInt() % 255);
		}
		
		rand = null;
		
		return btSaltKey;
	}

	public static int GetMasterKeySize(int nCrypt)
	{
		int nKeySize = 0;

		switch (nCrypt)
		{
		case Global.AES_128: nKeySize = 16; break;
		case Global.AES_192: nKeySize = 24; break;
		case Global.AES_256: nKeySize = 32; break;
		case Global.ARIA_128: nKeySize = 16; break;
		case Global.ARIA_192: nKeySize = 24; break;
		case Global.ARIA_256: nKeySize = 32; break;
		default: nKeySize = 16; break;
		}

		Log.d(TAG, "[SIP] Master key size: "+nKeySize);

		return nKeySize;
	}

	/*
	 * 20110407 bk
	 */
	public static int getCryptoSuitType(String strCrypt)
	{
		int nIndex = 0;
		for (int i=0; i<7; i++)
		{
			if (strCrypt.equalsIgnoreCase(Global.CryptoSuit[i]) == true)
			{
				nIndex = i;
				break;
			}
		}

		int nRet = 0;
		switch (nIndex)
		{
		case 0: nRet = 0x00; break;
		case 1: nRet = 0x80; break;
		case 2: nRet = 0x40; break;
		case 3: nRet = 0x20; break;
		case 4: nRet = 0x08; break;
		case 5: nRet = 0x04; break;
		case 6: nRet = 0x02; break;
		default: nRet = 0x00; break;
		}

		Log.d(TAG, "[SIP] GetCryptoSuitType("+strCrypt + ") return: 0x"+ Integer.toHexString(nRet));

		return nRet;
	}

	public static String getCryptoSuitString(int nCrypto)
	{
		int nIndex = 0;

		switch (nCrypto)
		{
		case Global.NONE_SRTP:	nIndex = Global.NONE_CRYPTO_SUIT;		break;
		case Global.AES_128:		nIndex = Global.AES_CRYPTO_SUIT_128;	break;
		case Global.AES_192:		nIndex = Global.AES_CRYPTO_SUIT_192;	break;
		case Global.AES_256:		nIndex = Global.AES_CRYPTO_SUIT_256;	break;
		case Global.ARIA_128:		nIndex = Global.ARIA_CRYPTO_SUIT_128;	break;
		case Global.ARIA_192:		nIndex = Global.ARIA_CRYPTO_SUIT_192;	break;
		case Global.ARIA_256:		nIndex = Global.ARIA_CRYPTO_SUIT_256;	break;
		default: 							nIndex = Global.NONE_CRYPTO_SUIT; break;
		}

		String strRet = Global.CryptoSuit[nIndex];

		return strRet;
	}


	public int RequestCall(int nStack, String peerId)
	{
		int nRet = Global.ERR_NOTHING;
		return nRet;
	}
	
	public int AcceptCall(String cid)
	{
		int nRet = Global.ERR_NOTHING;
		return nRet;
	}
	

	public static int RejectCall(String cid, int reason, int cause)
	{
		int nRet = Global.ERR_NOTHING;
		return nRet;
	}

	public int CancelCall(String cid)
	{
		int nRet = Global.ERR_NOTHING;
		return nRet;
	}

	public int SendUpdate(String cid)
	{	
		int nRet = Global.ERR_NOTHING;
		return nRet;
	}
	
	public int TerminateCall(String cid, String strReasonText)
	{
		int nRetVal = Global.ERR_NOTHING;
		return nRetVal;
	}
	
	public int HoldCall(String cid, boolean bHold, int cause)
	{
		int nRetVal = Global.ERR_NOTHING;
		return nRetVal;
	}
	
	public boolean SendMessage(int nStack, String tid, String text)
	{
		boolean bRet = true;
		return bRet;
	}
	
	public boolean SendInfo(String cid, String text)
	{
		boolean bRet = true;
		return bRet;
	}
	
	public void SetLoggingEnv(int nStack, int keepCount, String strLogPath) 
	{
		return;
	}
}
