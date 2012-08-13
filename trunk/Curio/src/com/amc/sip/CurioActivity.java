package com.amc.sip;


import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;
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
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.os.Vibrator;
import android.app.Activity;


public class CurioActivity extends Activity implements UIInterface{
	
	private static final String TAG = "SIP";
		
	private static AudioManager mAudioManager = null;
	
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
		
		
		Uri uri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
		String strUri = uri.getPath();
		setUriString(strUri);

      	final TextView myUID = (TextView) findViewById(R.id.TextView01);
       	myUID.setText("My ID");
       	
      	final TextView myPwd = (TextView) findViewById(R.id.TextView03);
      	myPwd.setText("Password");

      	final TextView mySIPServerIP = (TextView) findViewById(R.id.TextView02);
      	mySIPServerIP.setText("SIP Server IP");

      	final TextView myRcvPort = (TextView) findViewById(R.id.TextView04);
      	myRcvPort.setText("SIP Receive Port");
      	
      	final EditText edtUID = (EditText) findViewById(R.id.EditText01);
     	final EditText edtSvrIP = (EditText) findViewById(R.id.EditText02);
     	final EditText edtPwd = (EditText) findViewById(R.id.EditText03);
     	edtPeerID = (EditText) findViewById(R.id.EditText04);
     	final EditText edtRcvPort = (EditText) findViewById(R.id.EditText05);
     	final EditText edtDomain = (EditText) findViewById(R.id.EditText07);
     	final EditText edtPID = (EditText) findViewById(R.id.EditText06);
		final EditText edtConference = (EditText) findViewById(R.id.EditText10);
   
		//TEST
		{
			edtUID.setText("5143");
			edtPwd.setText("0000");
			edtSvrIP.setText("192.168.200.172");
			edtRcvPort.setText("5060");
			edtPeerID.setText("5144");
			edtPID.setText("5145");
			edtDomain.setText("ug2.scm.com");
			edtConference.setText("*815145");
		}
     	

		final Button buttonTLS = (Button) findViewById(R.id.Button16);
		buttonTLS.setText("ProtoType");
		buttonTLS.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) 
    		{     			
    			Log.i(TAG, "onClick - Set ProtoType button");   
    			
    			if (m_nProto == 2)
    				m_nProto = 0;
    			else
    				m_nProto++;
    			
    			String proto = (m_nProto==0)? "UDP" : (m_nProto==1)? "TCP" : "TLS";
    	    	ShowToast("ProtoType: " + proto);
    		}
    	});
    	
    	final Button buttonSRTP = (Button) findViewById(R.id.Button10);
    	buttonSRTP.setText("SRTP Off");
    	buttonSRTP.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
    			m_bSRTP = (m_bSRTP)? false : true;
    			
       			Log.i(TAG, "onClick - SRTP:"+m_bSRTP);
       			
       			if (m_bSRTP)
       			{
       				buttonSRTP.setText("SRTP On");
       				sipManager.setSrtpType(Global.AES_128 | Global.ARIA_128 | Global.ARIA_192);
       			}
       			else
       			{
       				buttonSRTP.setText("SRTP Off");
       				sipManager.setSrtpType(Global.NONE_SRTP);
       			}
    		}
    	}); 

		final Button btnDTMF = (Button) findViewById(R.id.Button18);
		btnDTMF.setText("DTMF");
		btnDTMF.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) 
    		{     			
    			Log.i(TAG, "onClick - Set DTMF button");   
    			
    			if (m_nMyDTMF == Global.DTMF_INBAND)
    				m_nMyDTMF = Global.DTMF_RFC2833;
    			else
    				m_nMyDTMF = Global.DTMF_INBAND;
    			
    			String dtmf = (m_nMyDTMF==Global.DTMF_INBAND)? "INBAND" : "RFC2833";
    	    	ShowToast("DTMF Type: " + dtmf);
    		}
    	});

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
   	
    	    	
    	    	
    	    	
//    	    	sipManager.SetLoggingEnv(Global.STACK_PRIMARY, keepCount, null);
    		}
    	});
       	
    	final Button buttonRegister = (Button) findViewById(R.id.Button01);
    	buttonRegister.setText("Register");
    	buttonRegister.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
     			
    			Log.i(TAG, "onClick - Register button");
    			
    			boolean ret = sipManager.Register(Global.STACK_PRIMARY, false);
    			Log.w(TAG, "SIPRegister:"+ret);
    			
//    			ret = sipManager.Register(Global.STACK_SECONDARY);
//    			Log.w(TAG, "SIPRegister:"+ret);
    		}
    	});
       	
    	final Button btnUnRegister = (Button) findViewById(R.id.Button21);
//    	btnUnRegister.setWidth(100);
    	btnUnRegister.setText("UnRegister");
    	btnUnRegister.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
     			
    			Log.i(TAG, "onClick - UnRegister button");
    			
    			boolean ret = sipManager.UnRegister(Global.STACK_PRIMARY);
    			Log.w(TAG, "SIPRegister:"+ret);
    			
    		}
    	});
    	
    	final Button btnKeepAlive = (Button) findViewById(R.id.Button20);
    	btnKeepAlive.setText("KeepAlive");
    	btnKeepAlive.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
     			
    			Log.i(TAG, "onClick - KeepAlive button");
    			
    			boolean ret = sipManager.Register(Global.STACK_PRIMARY, true);
    			Log.w(TAG, "SIPRegister:"+ret);
    		}
    	});
        
       	final Button buttonRequest = (Button) findViewById(R.id.Button02);
       	buttonRequest.setText("RequestCall");
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
       	
       	
      	
     	chkCID01 = (CheckBox)findViewById(R.id.checkBox7);
     	chkCID01.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				chkCID02.setChecked(!chkCID01.isChecked());
				
				Log.d(TAG, "Selected Call ID:"+chkCID01.getText());
			}
		});
      	
     	chkCID02 = (CheckBox)findViewById(R.id.checkBox8);
     	chkCID02.setOnClickListener(new View.OnClickListener() {
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				chkCID01.setChecked(!chkCID02.isChecked());
				
				Log.d(TAG, "Selected Call ID:"+chkCID02.getText());
			}
		});
       	
		final Button btnRefresh = (Button) findViewById(R.id.button1);
		btnRefresh.setText("Refresh");
		btnRefresh.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) 
    		{     			
    			Log.i(TAG, "onClick - btnRefresh button");   
    			
    			chkCID01.setText("call id");
    			chkCID02.setText("call id");
    			
    			chkCID01.setChecked(false);
    			chkCID02.setChecked(false);
    		}
		});
    			
        
       	buttonAccept = (Button) findViewById(R.id.Button03);
       	buttonAccept.setText("AcceptCall");
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
       	
       	final Button buttonReject = (Button) findViewById(R.id.Button07);
       	buttonReject.setText("RejectCall");
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
        
        
       	final Button buttonTerminate = (Button) findViewById(R.id.Button04);
       	buttonTerminate.setText("Terminate");
       	buttonTerminate.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Terminate button");
       			
       			if (chkCID01.isChecked() == true)
       				strCallID = chkCID01.getText().toString();
       			else if (chkCID02.isChecked() == true)
       				strCallID = chkCID02.getText().toString();
       			else
       				strCallID = "";
        			
       			sipManager.TerminateCall(strCallID, null);
       			
	        	m_is183 = false;
	        	m_isCallReq = false;
	        	m_isStopGips = true;
    		}
    	}); 
       	       	
       	final Button buttonHoldOn = (Button) findViewById(R.id.Button05);
       	buttonHoldOn.setText("Hold On");
       	buttonHoldOn.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Hold On button");
       			
       			if (chkCID01.isChecked() == true)
       				strCallID = chkCID01.getText().toString();
       			else if (chkCID02.isChecked() == true)
       				strCallID = chkCID02.getText().toString();
       			else
       				strCallID = "";
        			
       			m_bHold = true;
       			sipManager.HoldCall(strCallID, m_bHold, 0);
    		}
    	}); 
       	
       	final Button buttonHoldOff = (Button) findViewById(R.id.Button17);
       	buttonHoldOff.setText("Hold Off");
       	buttonHoldOff.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Hold Off button");
       			
       			if (chkCID01.isChecked() == true)
       				strCallID = chkCID01.getText().toString();
       			else if (chkCID02.isChecked() == true)
       				strCallID = chkCID02.getText().toString();
       			else
       				strCallID = "";
        			
       			m_bHold = false;
       			sipManager.HoldCall(strCallID, m_bHold, 0);
    		}
    	}); 
        
       	final Button btnCancel = (Button) findViewById(R.id.Button19);
       	btnCancel.setText("CancelCall");
       	btnCancel.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Cancel button");
       			
       			if (chkCID01.isChecked() == true)
       				strCallID = chkCID01.getText().toString();
       			else if (chkCID02.isChecked() == true)
       				strCallID = chkCID02.getText().toString();
       			else
       				strCallID = "";
        			
       			sipManager.CancelCall(strCallID);
       			
	        	m_is183 = false;
	        	m_isCallReq = false;
	        	m_isStopGips = true;
    		}
    	}); 

     	final Button buttonTransfer = (Button) findViewById(R.id.Button06);
       	buttonTransfer.setText("Blind Transfer");
       	buttonTransfer.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Transfer button");
       			
       			if (chkCID01.isChecked() == true)
       				strCallID = chkCID01.getText().toString();
       			else if (chkCID02.isChecked() == true)
       				strCallID = chkCID02.getText().toString();
       			else
       				strCallID = "";
       			
       			String strPeerID = edtPeerID.getText().toString();
       			Log.d(TAG, "Peer ID:"+strPeerID);
       		
       			m_bTransfer = true;
    		}
    	}); 
		
		//One Step Transfer
		final Button btnOneStepTransf = (Button) findViewById(R.id.Button15);
		btnOneStepTransf.setText("One Step Transfer");
		
		btnOneStepTransf.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v)
			{
			}
		});    
     	
       	final Button buttonConferenceCall = (Button) findViewById(R.id.Button40);
       	buttonConferenceCall.setText("Conference");
       	buttonConferenceCall.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Conference Call");
       			
       			String pID = edtConference.getText().toString();
       			
       			Log.i(TAG, pID);
       			
       			int nRet = sipManager.RequestCall(Global.STACK_PRIMARY, pID);
       			
       			if (nRet == Global.ERR_SIP_UA_FTN_CALL_FAIL)
       			{
       				Toast toast = Toast.makeText(mContext, "ERR_SIP_UA_FTN_CALL_FAIL", Toast.LENGTH_SHORT);
       				toast.show();
       			}
    		}
    	}); 

		Log.d(TAG, "[InCallScreen] InCallScreen onCreate [E]");		
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
	
	
	public void initGIPSManager()
	{
		getServices();
		Initialize();
	}
	
	public void getServices()
	{
		Log.d(TAG, "[InCallScreen] getServices [S]");

		mAudioManager = (AudioManager)this.getSystemService(this.AUDIO_SERVICE);
		mVib = (Vibrator)this.getSystemService(this.VIBRATOR_SERVICE);
			
     	RegPhoneListener();
     	
		Log.d(TAG, "[InCallScreen] getServices [E]");
	}
	
	public static void SetButtonEnable(boolean bEnable)
	{
		Log.i(TAG, "SetButtonEnable");

       	buttonAccept.setEnabled(bEnable);
	}
	
	static int phoneState = TelephonyManager.CALL_STATE_IDLE;
	static TelephonyManager telephonyManager = null;
	
	public void RegPhoneListener()
	{		  
		//모니터링 할 이벤트를 리스너에 등록한다.
		telephonyManager = (TelephonyManager)this.getSystemService(this.TELEPHONY_SERVICE); 
		
		PhoneStateListener phoneStateListener = new PhoneStateListener() 
		{
			public void onCallStateChanged(int state, String incomingNumber) 
			{
				// 전화 수신 반응. 
				// 착신 전화 번호를 받는다. 
				switch (state)
				{
				case TelephonyManager.CALL_STATE_IDLE :
					if (phoneState == TelephonyManager.CALL_STATE_OFFHOOK
							&& phoneState != TelephonyManager.CALL_STATE_RINGING)
					{
						if (IsPlayingFile() == true)
						{
							stopMedia();
							playRingtoneVibrator();
						}
					}
					break; // 폰이 울리거나 통화중이 아님.
				case TelephonyManager.CALL_STATE_RINGING:
					break; // 폰이 울린다.
				case TelephonyManager.CALL_STATE_OFFHOOK:
					break; // 폰이 현재 통화 중.
				default: 
					break;
				}
				
				int nStat = telephonyManager.getCallState();
				
				
				phoneState = state;
			} 
		}; 
		
		telephonyManager.listen(phoneStateListener, PhoneStateListener.LISTEN_CALL_STATE );
	}
	
	public int m_nRcvPort = 0;
	public String m_strKey = null;
	
	static boolean m_bInit = false;
	
	public int Initialize()
	{
		int nRet = 0;

		return nRet;
	}
	
	public int getMode()
	{
		int nMode = -1;
		
		return nMode;
	}
	
	public void setMode(int nMode)
	{

	}
	
	static boolean m_bCallDupOption = false;
	public void setCallDupOption(boolean bCallDupOption)
	{
		Log.d(TAG, "[InCallScreen] setCallDupOption() [S] [bCallDupOption:"+bCallDupOption+"]");
		m_bCallDupOption = bCallDupOption;
		Log.d(TAG, "[InCallScreen] setCallDupOption() [E]");
	}
	
	public boolean SetMute(boolean bUse)
	{
		boolean bRes = false;
		
		return bRes;
	}
		
	public boolean isSpeakerOn()
	{
		Log.d(TAG, "[InCallScreen] isSpeakerOn() [S]");
		
		boolean isSpkOn = false;
		
		try
		{
			isSpkOn = mAudioManager.isSpeakerphoneOn();
		}
		catch (NullPointerException e) 
		{
			Log.d(TAG, "[InCallScreen] NullPointerException [mAudioManager]");
			e.printStackTrace();

			mAudioManager = (AudioManager)this.getSystemService(this.AUDIO_SERVICE);			
			isSpkOn = mAudioManager.isSpeakerphoneOn();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		Log.d(TAG, "[InCallScreen] isSpeakerOn() [E] SpeakerphoneOn:" + isSpkOn);
		
		return isSpkOn;
	}
	
	public boolean SetWaveOutPath(boolean bOut)
	{
		return true;
	}
	
	public boolean SetWaveOutPath2(boolean bOut)
	{
		return true;
	}
	
		
	public boolean SetSpeakerVolume(int nVal)
	{
		return true;
	}
	
	public int GetMasterVolumeLevel()
	{
		int nVolume = -1;
		return nVolume;
	}
	
	public void SetMasterVolumeLevel(int nVolLevel)
	{
		Log.d(TAG, "[InCallScreen] SetMasterVolumeLevel() [S]");
		
		Log.d(TAG, "[InCallScreen] SetMasterVolumeLevel() [E]");
	}
	
	public void setUriString(String strUri)
	{
		Log.d(TAG, "[InCallScreen] setUriString() [S] "+strUri);
		
		muri = Uri.parse(strUri);
		
		Log.d(TAG, "[InCallScreen] setUriString() [E]");
	}
	
//    Ringtone mRingtone;

	private final static Object syncObj1 = new Object();

	class RingtoneThread extends Thread 
	{
		public RingtoneThread(String name) 
		{
			super(name);
			 // TODO Auto-generated constructor stub
		}
		
		public void run()
		{
			Log.d(TAG, "[InCallScreen] RingtoneThread run() [S]");
			// Thread가 동작하는 방식을 이 메소드에서 구현을 한다.			
			
			setMode(AudioManager.MODE_RINGTONE);
			
			long StartedTime = SystemClock.elapsedRealtime();
			long CurrentTime = -1;
			boolean isResetOrgVolume = false;
			
	   		try{
				OrgVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_RING);
				Log.w(TAG, "Get volume level "+OrgVolume);
				mAudioManager.setStreamVolume(AudioManager.STREAM_RING, INIT_VOLUME, 0);
			    Log.w(TAG, "Set volume level 1");
			    Log.w(TAG, "Start Time: "+StartedTime);

			 	while (m_bStopThread == false)
			 	{					    
		 			CurrentTime = SystemClock.elapsedRealtime();
				    Log.w(TAG, "Current Time: "+CurrentTime);
			 		if ((CurrentTime-StartedTime) > 2000 && isResetOrgVolume == false)
			 		{
				 		int CurVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_RING);
				 		if (CurVolume != 1)
				 			mAudioManager.setStreamVolume(AudioManager.STREAM_RING, CurVolume, 0);
				 		else
				 			mAudioManager.setStreamVolume(AudioManager.STREAM_RING, OrgVolume, 0);
					    Log.w(TAG, "Set volume level "+OrgVolume);
			 			isResetOrgVolume = true;
			 		}
			 		
					if (mRingtone.isPlaying() == false)
					{
						Log.d(TAG, "[InCallScreen] RingtoneThread Ringtone.play()");
						if (m_bStopThread == false)
						{
							mRingtone.play();
						}
					}
					
					sleep(200);
			 	}
			 	
		 		Log.d(TAG, "[InCallScreen] ********* m_bStopThread: "+m_bStopThread);

		 		if (mRingtone.isPlaying() == true)
			 	{
					Log.d(TAG, "[InCallScreen] RingtoneThread Ringtone.stop()");
			 		mRingtone.stop();			 		
			 		
			 		int CurVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_RING);
			 		if (CurVolume != 1)
			 			mAudioManager.setStreamVolume(AudioManager.STREAM_RING, CurVolume, 0);
			 		else
			 			mAudioManager.setStreamVolume(AudioManager.STREAM_RING, OrgVolume, 0);
				    Log.w(TAG, "Set volume level "+OrgVolume);

				    setMode(AudioManager.MODE_NORMAL);			 		
			 		m_bStopRingtone = true;
			 	}
			}
	   		catch(Exception e)
	   		{
				e.printStackTrace();
	   		}
	   		
			Log.d(TAG, "[InCallScreen] RingtoneThread run() [E]");
		 }
	}
	
	static boolean m_bStopRingtone = true;
	static int OrgVolume = 0;
	static final int INIT_VOLUME = 1;
	
    public void playRingtone()
	{
		Log.d(TAG, "[InCallScreen] playRingtone() [S]");
		
		try
		{	
		    mRingtone = RingtoneManager.getRingtone(this, muri);
		}
		catch(Exception e)
		{
			Log.d(TAG, "[InCallScreen] RingtoneThread [E]");
			e.printStackTrace();
		}
		    
	    m_bStopRingtone = false;
	    m_bStopThread = false;
	    
	    try
	    {
	    	ringtonThread = new RingtoneThread("(RingtoneThread)");
	    	ringtonThread.setPriority(1);
	    	ringtonThread.start();
		}
	    catch(Exception e)
		{
			e.printStackTrace();
		}
		Log.d(TAG, "[InCallScreen] playRingtone() [E]");
	}
	
    /**
	 * @uml.property  name="ringtonThread"
	 * @uml.associationEnd  
	 */
    static RingtoneThread ringtonThread;
    static boolean m_bStopThread = false;
    
	public void stopRingtone()
	{
		Log.d(TAG, "[InCallScreen] stopRingtone() [S]");	
		
		m_bStopThread = true;
		
		if (mRingtone != null)
		{
			try
			{
//				if (ringtonThread != null)
//					ringtonThread.stop();

				Log.d(TAG, "[InCallScreen] stop ringtoneThread !!");	

				if (mRingtone.isPlaying())
				{
			 		mRingtone.stop();	
			 		
				    mAudioManager.setStreamVolume(AudioManager.STREAM_RING, OrgVolume, 0);
				    Log.w(TAG, "Set volume level "+OrgVolume);
			 		
			 		setMode(AudioManager.MODE_NORMAL);
					m_bStopRingtone = true;
				}
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}			
		}
		
// 		Log.d(TAG, "[InCallScreen] @@@@@@@@@ m_bStopThread: "+m_bStopThread);

    	Log.d(TAG, "[InCallScreen] stopRingtone() [E]");
	}
		
	public void playRingtoneVibrator()
	{
		Log.d(TAG, "[InCallScreen] playRingtoneVibrator() [S]");
		
		
		int rm = 0;
		int vs = 0;
		long[] vibratePattern = {0,1000,1000};

		try
		{
			rm = mAudioManager.getRingerMode();
			vs = mAudioManager.getVibrateSetting(AudioManager.VIBRATE_TYPE_RINGER);
		}
		catch (NullPointerException e) 
		{

		}
		
		String strMode;
		switch (rm)
		{
		case AudioManager.RINGER_MODE_VIBRATE:
			strMode = "RINGER_MODE_VIBRATE"; break;
		case AudioManager.RINGER_MODE_NORMAL:
			strMode = "RINGER_MODE_NORMAL"; break;
		default:
			strMode = "RINGER_MODE_UNKNOWN:"+rm; break;			
		}
		Log.d(TAG, "[InCallScreen] ## RingerMode:"+strMode);
		
		switch (vs)
		{
		case AudioManager.VIBRATE_SETTING_ON:
			strMode = "VIBRATE_SETTING_ON"; break;
		case AudioManager.VIBRATE_SETTING_OFF:
			strMode = "VIBRATE_SETTING_OFF"; break;
		case AudioManager.VIBRATE_SETTING_ONLY_SILENT:
			strMode = "VIBRATE_SETTING_ONLY_SILENT"; break;
		default:
			strMode = "VIBRATE_SETTING_UNKNOWN:"+vs; break;
		}
		Log.d(TAG, "[InCallScreen] ## VibrateMode:"+strMode);
		
		if ( (rm == AudioManager.RINGER_MODE_VIBRATE) 
				|| (rm == AudioManager.RINGER_MODE_NORMAL && vs == AudioManager.VIBRATE_SETTING_ON)
				|| (rm == AudioManager.RINGER_MODE_VIBRATE && vs == AudioManager.VIBRATE_SETTING_ONLY_SILENT))
		{
			if (mVib != null)
			{
				try
				{
					Log.d(TAG, "[InCallScreen] called Vibrator.vibrate()");
					mVib.vibrate(vibratePattern,1);
					m_bVib = true;
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		}		
		else if (rm == AudioManager.RINGER_MODE_NORMAL && vs != AudioManager.VIBRATE_SETTING_ON)
		{
			playRingtone();
		}
		
		Log.d(TAG, "[InCallScreen] playRingtoneVibrator() [E]");
	}
	
	static boolean m_bVib = false;
	
	public void stopVibrator()
	{
		Log.d(TAG, "[InCallScreen] stopVibrator() [S]");
		if (mVib != null)
		{
			if (m_bVib == true)
			{
				try
				{
					mVib.cancel();
					m_bVib = false;
				}
				catch (Exception e)
				{
					Log.d(TAG, "[InCallScreen] stopVibrator() ", e);
					e.printStackTrace();
				}
			}
		}
		Log.d(TAG, "[InCallScreen] stopVibrator() [E]");	
	}
	
	// stop the music and play new Music 
	public void play(Context context, int resource)
	{ 
		Log.d(TAG, "[InCallScreen] play() [S]");

		//Service에서 setMode() 하도록 수정됨.
//		setMode(4);
		
//		//재생할 음원 지정
//		MediaPlayerService.m_nResource = resource;
//		
//		//서비스 시작
//		context.startService(new Intent(context,MediaPlayerService.class));
		
//		try
//		{
////			SetWaveOutPath(false);
//			setMode(4);
//
//			if (mMediaPlay != null)
//			{
//				mMediaPlay.release();
//				mMediaPlay = null;
//			}
//			
//			mMediaPlay = MediaPlayer.create(context, resource);
//			mMediaPlay.setLooping(true);
//			mMediaPlay.start();
//		}
//		catch(Exception e)
//		{
//			Log.e(TAG, "Raised Exception in play function. [E]", e);
//			e.printStackTrace();
//		}
		Log.d(TAG, "[InCallScreen] play() [E]");
	}
	
	public void stop()
	{
		Log.d(TAG, "[InCallScreen] stop() [S]");	
		
		try
		{
			stopMedia();			
			stopRingtone();
			stopVibrator();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}		
		
		Log.d(TAG, "[InCallScreen] stop() [E]");
	}

	public void stopMedia()
	{
		Log.d(TAG, "[InCallScreen] stopMedia() [S]");
		
		//재생 중지
//		MediaPlayerService.stopMPService();
//		
//		//서비스 종료
//		this.stopService(new Intent(this,MediaPlayerService.class));
//		
		Log.d(TAG, "[InCallScreen] stopMedia() [E]");
		
//		try
//		{
//			if (mMediaPlay != null)
//			{
//				if (mMediaPlay.isPlaying())
//				{
//					mMediaPlay.stop();
//				}
//			}
//		}
//		catch(Exception e)
//		{
//			Log.e(TAG, "Raised Exception in stop function.", e);			
//			e.printStackTrace();
//		}		
	}
	
//	public boolean isOffhook()
//	{
//		Log.d(TAG, "[InCallScreen] isOffhook() [s]");
//
//		ITelephony mCDMA = ITelephony.Stub.asInterface(ServiceManager.checkService("phone"));
//		
//		boolean bOffhook = false;
//		
//		try 
//		{
//			bOffhook = mCDMA.isOffhook();
//		}
//		catch (Exception e)
//		{
//			e.printStackTrace();
//		}
//		
//		Log.d(TAG, "[InCallScreen] isOffhook() [E] [return:"+bOffhook+"]");
//		return bOffhook;
//	}
	
//	public void StartPlayFile(String szFileName)
//	{
//		Log.d(TAG, "[InCallScreen] StartPlayFile [S] ["+ szFileName + "]");
//
//		if (szFileName.equalsIgnoreCase("RingBack.wav"))
//		{
//			play(this, R.raw.ringback);
////			Log.d(TAG, "[InCallScreen] RingBack.wav");
//		}
//		else if (szFileName.equalsIgnoreCase("sound_offering.wav"))
//		{
//			//play(this, R.raw.sound_offering);
//			Log.d(TAG, "[InCallScreen] [m_bCallDupOption:"+m_bCallDupOption+"]");
//			
//			try
//			{
//				ITelephony mCDMA = ITelephony.Stub.asInterface(ServiceManager.checkService("phone"));
//				
//				if (m_bCallDupOption == true)
//				{
//					//Rnging 이거나 Dialing 이면 재생 안함.
//					if (phoneState != TelephonyManager.CALL_STATE_RINGING
//							&& (mCDMA.isAlerting() == false))
//					{
//						if (mCDMA.isOffhook() == true)
//						{
//							//beep
//							play(this, R.raw.sound_offering_pront);
//						}
//						else
//						{
//							playRingtoneVibrator();
//						}
//						Log.d(TAG, "[InCallScreen] isAlerting(): " + mCDMA.isAlerting());
//					}
//				}
//				else
//				{
//					if (phoneState == TelephonyManager.CALL_STATE_IDLE)
//					{
//						playRingtoneVibrator();
//					}
//					else
//					{
//						Log.d(TAG, "[InCallScreen] Rejected Sound Play !!!");
//					}
//				}
//			}
//			catch (Exception e)
//			{
//				e.printStackTrace();
//			}
//			
//			
//			Log.d(TAG, "[InCallScreen] sound_offering.wav");
//		}
//		else if (szFileName.equalsIgnoreCase("BusySignal.wav"))
//		{
//			play(this, R.raw.busysignal);
//			Log.d(TAG, "[InCallScreen] BusySignal.wav");
//		}
//		
//		bPlayingWav = true;
//				
//		Log.d(TAG, "[InCallScreen] StartPlayFile [E]");	
//	}
	
	public void StopPlayFile()
	{
		Log.d(TAG, "[InCallScreen] StopPlayFile() [S]");	
		
		bPlayingWav = false;
		
		stop();
		
		Log.d(TAG, "[InCallScreen] StopPlayFile() [E]");	
	}
	
	static boolean bPlayingWav = false;
	
	public boolean IsPlayingFile()
	{
		//boolean bPlayingWav = false;
		
//		if (mMediaPlay != null)
//			bPlayingWav = mMediaPlay.isPlaying();
//		else if (mRingtone != null)
//			bPlayingWav = mRingtone.isPlaying();
//		else if (mVib != null)
//			bPlayingWav = mVib.;
			
		Log.d(TAG, "[InCallScreen] IsPlayingFile ["+ bPlayingWav +"]");	
	
		return bPlayingWav;
	}
	
	public boolean SendDTMF(int nVal, boolean bOutband)
	{
		boolean bRes = false;
		
//		GIPSVEAndroid_SendDTMF(nVal, bOutband);
		
		return bRes;
	}
	
	
	public boolean StopAudioRecv()
	{
		boolean bRes = true;
//		boolean bRes = GIPSVEAndroid_StopAudioRecv();		
		return bRes;
	}
	
	public boolean StopAudioSend()
	{
		boolean bRes = true;
//		boolean bRes = GIPSVEAndroid_StopAudioSend();		
		return bRes;
	}
	
    Handler m_GipsHandler = new Handler()
	{ 
		@SuppressWarnings("unused")
		@Override
		public void handleMessage(Message msg) 
		{ 
			String strMessage = null;
			Toast toast = null;
			
			if (strMessage != null)
			{
				toast = Toast.makeText(CurioActivity.this, strMessage, Toast.LENGTH_SHORT);
				toast.show();
			}
			
			Log.d(TAG, "GetGipsMessage() [E]");
		}
	};
	
	public static boolean m_is183 = false;
	public static boolean m_isCallReq = false;
	public static boolean m_isStopGips = true;
	public static boolean m_isDupAudInfo = false;

	public boolean addMessage(MCS_MSG msg)
	{
		Log.w(TAG, "[InCallScreen_Sample] InCallScrees.AddMessage() [S]");
		
    	String strMessage = "Message !!!";
   	
		
    	try
    	{
        	Toast toast = null;

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
		    	case 100:
		    		strMessage = "Received Message: " + msg.nMessageID;
		    		setMode(4);
		    		break;
		    	case 180:
		    		strMessage = "Received Message: " + msg.nMessageID;
		    		break;
		    	case 183:
		    		strMessage = "Received Message: " + msg.nMessageID;
					Log.d(TAG, strMessage);
					
		    		m_is183 = true;
		    		break;
		    		
		    	case Global.MCS_CALL_RCV_INVITE:
		    	{	
		    		m_isCallReq = true;
		    		
					strMessage = "MCS_CALL_RCV_INVITE";
					
					//strCallID = obj.strCallID;
					
					Log.d(TAG, "Received Message: "+ strMessage + " CID: " + strCallID);
					
			   		break;
			   	}
		    	case Global.MCS_CALL_RCV_INVITE_200: 
		    	case Global.MCS_CALL_SND_INVITE_200:					// 72, 통화 연결 성공									(양쪽)
		    	{
		    		//RECEIVED_INVITE_INFO obj = (RECEIVED_INVITE_INFO)msg.param;

					strMessage = "MCS_CALL_CONNECT_OK [S]";
					Log.d(TAG, strMessage);
					
					//strCallID = obj.strCallID;
					
					if (m_is183 == true)
					{
						setMode(4);
						m_is183 = false;
						
						SetWaveOutPath2(false);
					}
					
					if (m_isCallReq == true)
					{
						setMode(4);
						m_isCallReq = false;
						
						SetWaveOutPath2(false);
					}
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
		    	case Global.MCS_CALL_BYE_REQUEST:					// 74, 통화 종료 요청 수신						(수신측)
		    	{
		    		strMessage = "MCS_CALL_BYE_REQUEST";
		    		
		    		
		    		setMode(AudioManager.MODE_NORMAL);
		    		
		    		break;
		    	}
		    	case Global.MCS_CALL_CANCEL_REQUEST:				// 75, 통화 취소 요청 수신						(수신측)
		    	{
		    		strMessage = "MCS_CALL_CANCEL_REQUEST";
		    		break;
		    	}
		    	//
		    	case Global.MCS_CALL_HOLD_ON:						// 76, 통화 HOLD 설정됨					(요청하는 측)
		    	{
		    		strMessage = "MCS_CALL_HOLD_ON";
		    		
		    		
					setMode(AudioManager.MODE_NORMAL);
		
		    		break;
		    	}
		    	case Global.MCS_CALL_HOLD_OFF:						// 77, 통화 HOLD 해제됨					(요청하는 측)
		    	{
		    		strMessage = "MCS_CALL_HOLD_OFF";
					
					setMode(4);	    						
					
		
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
		    	case Global.MCS_CALL_TRANSFER_READY_OK:			// 85, Call Transfer 준비 완료
		    	{
		    		ReceivedHoldOnReq();
		    		setMode(AudioManager.MODE_NORMAL);
						
		    		strMessage = "MCS_CALL_TRANSFER_READY_OK";
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
    		Log.w(TAG, "[InCallScreen_Sample] InCallScrees.AddMessage() rised exception [E]");
   		
    		return false;
    	}
 		
		Log.e(TAG, strMessage);
		
		Log.d(TAG, "[InCallScreen_Sample] InCallScrees.AddMessage() [E]");
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


