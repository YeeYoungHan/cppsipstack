package com.amc.sip;

import android.text.format.Formatter;
import android.util.Log;
import android.media.*;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.content.Context;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import android.os.Bundle;

import android.os.Vibrator;
import android.app.Activity;

public class CurioActivity extends Activity implements UIInterface{
	
	private static final String TAG = "CURIO";
	private static WifiManager wifiMgr = null;
	   
	static Uri muri = null;
	static Ringtone mRingtone = null;
	static Vibrator mVib;
	

	public static SipManager sipManager = null;
	
	private static Object syncUIObj = new Object();

	public static CheckBox chkCID01;
 	public static CheckBox chkCID02;
	public static Button buttonAccept;
	public static EditText edtPeerID;
	
	static boolean m_bHold = false;
	static boolean m_bTransfer = false;
	static boolean m_bMute = false;
	static boolean m_bSRTP = false;
	static boolean m_bSpkOn = false;	
	static int m_nProto = 0;	
	static int m_nMyDTMF = Global.DTMF_RFC2833;
	public static Context mContext = null;

	public String strCallID = "";
	
	int nDTMF = 0;
	
	Uri uri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
	String strUri = uri.getPath();
		
	public void onCreate(Bundle savedInstanceState)
    {     	
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

		if (sipManager == null)
		{
			sipManager = new SipManager(this, syncUIObj);
		}
		
		mContext = this;
		
		setButton();
    }
	
	public void setButton()
	{
      	final TextView myUID = (TextView) findViewById(R.id.TextViewMyID);
       	myUID.setText("My ID");
       	
      	final TextView myPwd = (TextView) findViewById(R.id.TextViewPassword);
      	myPwd.setText("Password");

      	final TextView mySIPServerIP = (TextView) findViewById(R.id.TextViewServerIP);
      	mySIPServerIP.setText("Server IP");
      	
      	final TextView mySIPServerDomain = (TextView) findViewById(R.id.TextViewServerDomain);
      	mySIPServerDomain.setText("Server Domain");
      	
      	final TextView myRcvPort = (TextView) findViewById(R.id.TextViewRcvPort);
      	myRcvPort.setText("Receive Port");
      	
      	final EditText edtUID = (EditText) findViewById(R.id.EditText01);
     	final EditText edtSvrIP = (EditText) findViewById(R.id.EditText02);
     	final EditText edtPwd = (EditText) findViewById(R.id.EditText03);
     	edtPeerID = (EditText) findViewById(R.id.EditText04);
     	final EditText edtRcvPort = (EditText) findViewById(R.id.EditText05);
     	final EditText edtDomain = (EditText) findViewById(R.id.EditText07);
     	
   
		//TEST
		edtUID.setText("");
		edtPwd.setText("");
		edtSvrIP.setText("");
		edtRcvPort.setText("");
		edtPeerID.setText("");
		edtDomain.setText("");

		final Button buttonInit = (Button) findViewById(R.id.Button13);
    	buttonInit.setText("Initialize");
    	buttonInit.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) 
    		{     			
    			Log.i(TAG, "onClick - Initialize button");   
    			
    			String myId = edtUID.getText().toString();
    			String myPwd = edtPwd.getText().toString();
    			String svrIp = edtSvrIP.getText().toString();
    			String rcvPort = edtRcvPort.getText().toString();
    			
    			String domain = edtDomain.getText().toString();
    			if (domain.length() == 0)
    				domain = svrIp;
    			
    			int nRcvPort = Integer.parseInt(rcvPort);
    			
    			
    			int nSvrPort = 0;
    			if (m_nProto == 0)
    				nSvrPort = 5060;
    			else
    				nSvrPort = 5061;
    			
    			String localAddr = getLocalIPAddress(mContext);
    			
    			String szCallID = "ABCDEFG12345";
    			
    			wifiMgr = (WifiManager) getSystemService(Context.WIFI_SERVICE);
    	    	WifiInfo wifiInfo = wifiMgr.getConnectionInfo();
    	    	String strMacAddr = wifiInfo.getMacAddress();
    	    	ShowToast("MAC Address: " + strMacAddr);
    	    	szCallID = strMacAddr;
    	    	
    	    	int nExpire = 600;
    	    	int keepCount = 2;
    	    	String strLogPath = "/sdcard/smv/";
    	 
    	    	int nRet = sipManager.Initialize(Global.STACK_PRIMARY, myId, myId, myPwd, nRcvPort, nSvrPort, svrIp, 
						domain, 10000, 15000, m_nProto, localAddr, szCallID, nExpire,
						strLogPath, keepCount); 
    	    	
    	    	if (nRet == 0)
    	    	{
    	    		sipManager.SetAudioCodecInfo("PCMU;PCMA;G.729", m_nMyDTMF);
    	    		sipManager.SetCurrentVersion("SAMSUNG-SSP-Mobile/SIP stack sample/SHW-M110S/SKTelecom");
    	    	}
    		}
    	});
       	
    	final Button buttonRegister = (Button) findViewById(R.id.Button01);
    	buttonRegister.setText("Register");
    	buttonRegister.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
     			
    			Log.i(TAG, "onClick - Register button");
    			
    			boolean ret = sipManager.Register(Global.STACK_PRIMARY, false);
    			Log.w(TAG, "SIPRegister:"+ret);
    		}
    	});
        
       	final Button buttonRequest = (Button) findViewById(R.id.Button_INVITE);
       	buttonRequest.setText("Invite");
       	buttonRequest.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Invite button");
       			
       			String pID = edtPeerID.getText().toString();
       			
       			int nRet = sipManager.RequestCall(Global.STACK_PRIMARY, pID);
       			
       			if (nRet == Global.ERR_SIP_UA_FTN_CALL_FAIL)
       			{
       				Toast toast = Toast.makeText(mContext, "ERR_SIP_UA_FTN_CALL_FAIL", Toast.LENGTH_SHORT);
       				toast.show();
       			}
    		}
    	});
       	
       	buttonAccept = (Button) findViewById(R.id.Button_ACCEPT);
       	buttonAccept.setText("Accept");
       	buttonAccept.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Accept button");
       			
       			if (chkCID01.isChecked() == true)
       				strCallID = chkCID01.getText().toString();
       			else if (chkCID02.isChecked() == true)
       				strCallID = chkCID02.getText().toString();
       			else
       				strCallID = "";
       			   
       			sipManager.AcceptCall(strCallID);

    		}
    	});
       	
       	final Button buttonReject = (Button) findViewById(R.id.Button_REJECT);
       	buttonReject.setText("Reject");
       	buttonReject.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Reject button");
       			
       			if (chkCID01.isChecked() == true)
       				strCallID = chkCID01.getText().toString();
       			else if (chkCID02.isChecked() == true)
       				strCallID = chkCID02.getText().toString();
       			else
       				strCallID = "";
        			
       			SipManager.RejectCall(strCallID, Global.SIP_CALL_RTYPE_TEMP_UNAVAILABLE, 0);
    		}
    	});
        
        
       	final Button buttonBye = (Button) findViewById(R.id.Button_BYE);
       	buttonBye.setText("Bye");
       	buttonBye.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
       			sipManager.TerminateCall(strCallID, null);
    		}
    	}); 
       	       	

       	final Button buttonHoldOn = (Button) findViewById(R.id.Button_HOLD);
       	buttonHoldOn.setText("Hold");
       	buttonHoldOn.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
       			sipManager.HoldCall(strCallID, m_bHold, 0);
    		}
    	}); 
        
       	final Button btnCancel = (Button) findViewById(R.id.Button_CANCEL);
       	btnCancel.setText("Cancel");
       	btnCancel.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
       			Log.i(TAG, "onClick - Cancel button");
       			sipManager.CancelCall(strCallID);
    		}
    	}); 
	}
    @Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		Log.d(TAG, "onPause");
	}

	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		Log.d(TAG, "onStop");
	}

	private void ShowToast(String str)
    {
    	Toast toast = null;
		toast = Toast.makeText(this, str, Toast.LENGTH_SHORT);
		toast.show();
    }

    public static void ReceivedHoldOnReq()
    {
    }
        
    static boolean m_bDestroy = false;

    @Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		
		sipManager.UnRegister(Global.STACK_PRIMARY);

		m_bDestroy = true;
	}
	
	public static void SetButtonEnable(boolean bEnable)
	{
		Log.i(TAG, "SetButtonEnable");

       	buttonAccept.setEnabled(bEnable);
	}

	public boolean addMessage(MCS_MSG msg)
	{	
    	String strMessage = "Message !!!";
    	try
    	{
        	switch (msg.nMessageID)
	    	{
		    	case Global.MCS_NONE: 
		       		strMessage = "None Message";
		    		break;
		    		
		    	case 9:
		    	{
		    		strMessage = "Register failed";
		    		break;
		    	}
		    	/* 로그인 */	
		    	case Global.MCS_SIP_REGISTER_OK:	// 10, SIP 서버 로그인 성공 
		    	{
		   			strMessage = "Register ok";
		    		break;
		    	}
		  		case  Global.MCS_SIP_REGISTER_FAIL:						// 11, SIP 서버 로그인 실패
		    	{
					strMessage = "MCS_SIP_REGISTER_FAIL" + "(error:"+msg.nValue+")";
			   		break;
			    }
		    	case Global.MCS_SIP_LOGOUT_OK:	// 20, SIP 서버 로그아웃 성공
		       	{
					strMessage = "MCS_SIP_REGISTER_OK";
					if (m_bDestroy == true)
					{
					}
			   		break;
			    }
		    	case Global.MCS_SIP_LOGOUT_FAIL:					// 21, SIP 서버 로그아웃 실패
		       	{
					strMessage = "MCS_SIP_LOGOUT_FAIL";
			   		break;
			    }
		    	case Global.MCS_CALL_RCV_RE_INVITE:
					strMessage = "MCS_CALL_RCV_RE_INVITE";
					Log.d(TAG, "MCS_CALL_RCV_RE_INVITE");
					
					break;

					
		    	case Global.MCS_CALL_CONNECT_FAIL:					// 73, 통화 연결 실패					(송신측)
		    	{   
		    		String strErr;
		    		switch (msg.nValue)
		    		{
		    		case Global.ERR_CANNOT_CALL_TO_MYSELF: 	strErr = "ERR_CANNOT_CALL_TO_MYSELF"; 	break;
		    		case Global.ERR_USER_REJECT: 			strErr = "ERR_USER_REJECT"; 			break;
		    		case Global.ERR_USER_BUSY: 				strErr = "ERR_USER_BUSY"; 				break;
		    		case Global.ERR_USER_NOT_ACCEPT: 		strErr = "ERR_USER_NOT_ACCEPT"; 		break;
		    		default: 								strErr = "Unknown Error"; 				break;
		    		}
		   
		    		strMessage = "MCS_CALL_CONNECT_FAIL ["+strErr+"]";
		    		break;
		    	}
		    	case Global.MCS_CALL_CANCEL_REQUEST:				// 75, 통화 취소 요청 수신						(수신측)
		    	{
		    		strMessage = "MCS_CALL_CANCEL_REQUEST";
		    		break;
		    	}
		    	case Global.MCS_CALL_HOLD_ON_REQUEST:				// 78, 통화 HOLD 설정됨					(요청당하는 측)
		    	{		
		    		strMessage = "MCS_CALL_HOLD_ON_REQUEST";
		    		
		    		break;
		    	}
		    	case Global.MCS_CALL_HOLD_OFF_REQUEST:				// 79, 통화 HOLD 해제됨					(요청당하는 측)
		    	{
		    		strMessage = "MCS_CALL_HOLD_OFF_REQUEST";
		    		break;
		    	}
		    	case Global.MCS_CALL_HOLD_FAIL:						// 80, 통화 HOLD 관련 Action 실패
		    	{
		    		strMessage = "MCS_CALL_HOLD_FAIL";
		    		break;
		    	}
		    	case Global.MCS_CALL_TRANSFER_READY_FAIL:			// 86, Call Transfer 준비 실패
		    	{
		    		strMessage = "MCS_CALL_TRANSFER_READY_FAIL";
		    		break;
		    	}
		    	case Global.MCS_CALL_TRANSFER_OK:					// 87, Call Transfer 성공
		    	{
		    		m_bHold = false;
		    		m_bTransfer = false;
		
		    		strMessage = "MCS_CALL_TRANSFER_OK";
		    		break;
		    	}
		    	case Global.MCS_CALL_TRANSFER_FAIL:					// 88, Call Transfer 실패
		    	{
		    		strMessage = "MCS_CALL_TRANSFER_FAIL";
		    		break;
		    	}
		    	case Global.SIP_INV_SEND_RET_FAILED:
		    	{
		    		strMessage = "SIP_INV_SEND_RET_FAILED";
		    		break;
		    	}
		    	case Global.MSG_SEND_REGISTER_ALARM:
		    	{
		    		strMessage = "SendRegisterAlarm [Interval:"+msg.nValue+"]";
		    		break;
		    	}
		    	case Global.GIPS_SET_SRTP_ENABLE:
		    	{
		    		strMessage = "GIPS_SET_SRTP_ENABLE";
		    		
//		    		if (msg.nErrorCode == 1)
//		    			GIPSVEAndroid_EnableSRTP();
//		    		else
//		    			GIPSVEAndroid_DisableSRTP();
		    		
		    		break;
		    	}
				default:
		    	{
		    		strMessage = "Unkown Message";
		    		break;
		    	}
	    	
	    	}//End switch
	    	
	       	
//			toast = Toast.makeText(this, strMessage, Toast.LENGTH_SHORT);
//			toast.show();

    	}
    	catch (Exception e)
    	{
    		e.printStackTrace();
    		return false;
    	}
 		
		Log.e(TAG, strMessage);
		
		return true;
	
	}//End GetMessage()
	
    /** LocalIPAddress 얻어오기 2 */
    public static String getLocalIPAddress(Context context)
    {
    	Log.d(TAG, "----<<<< getLocalIPAddress >>>>----");
    	String addr = "";
    	try {
    		if(context == null)throw new Exception();
    		
			WifiManager wm = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);
			WifiInfo wi = wm.getConnectionInfo();
			int ipAddress = wi.getIpAddress();
			addr = Formatter.formatIpAddress(ipAddress);
			Log.d(TAG, "getLocalIPAddress (=Initialize param) : "+addr);
		} catch (Exception e) {
			e.printStackTrace();
			Log.d(TAG, e.getMessage());
		} 
		return addr;
    }
}


