package com.curio.sip;

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
	private static Object syncUIObj = new Object();
	
	public static SipManager sipManager = null;
	public static EditText edtPeerID;
	public static int m_nProto = 0;	
	public static Context mContext = null;

	public String strCallID = "";
	
	int nDTMF = 0;
		
	
	
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
    	 
    	    	int nRet = sipManager.Initialize(myId, myId, myPwd, nRcvPort, nSvrPort, svrIp, 
						domain, 10000, 15000, m_nProto, localAddr, szCallID, nExpire,
						strLogPath, keepCount);
    		}
    	});
       	
    	final Button buttonRegister = (Button) findViewById(R.id.Button01);
    	buttonRegister.setText("Register");
    	buttonRegister.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
     			
    			Log.i(TAG, "onClick - Register button");
    			
    			boolean ret = sipManager.Register();
    			Log.w(TAG, "SIPRegister:"+ret);
    		}
    	});
        
       	final Button buttonRequest = (Button) findViewById(R.id.Button_INVITE);
       	buttonRequest.setText("Invite");
       	buttonRequest.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Invite button");
       			
       			String pID = edtPeerID.getText().toString();
       			
       			int nRet = sipManager.Invite(pID);
    		}
    	});
       	
       	final Button buttonAccept = (Button) findViewById(R.id.Button_ACCEPT);
       	buttonAccept.setText("Accept");
       	buttonAccept.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Accept button");
       			sipManager.Accept(strCallID);
    		}
    	});
       	
       	final Button buttonReject = (Button) findViewById(R.id.Button_REJECT);
       	buttonReject.setText("Reject");
       	buttonReject.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
    			
       			Log.i(TAG, "onClick - Reject button");
       			SipManager.Reject(strCallID);
    		}
    	});
        
       	final Button buttonBye = (Button) findViewById(R.id.Button_BYE);
       	buttonBye.setText("Bye");
       	buttonBye.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
       			sipManager.Bye(strCallID);
    		}
    	}); 
       	       	
       	final Button buttonHoldOn = (Button) findViewById(R.id.Button_HOLD);
       	buttonHoldOn.setText("Hold");
       	buttonHoldOn.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
       			sipManager.Hold(strCallID);
    		}
    	}); 
        
       	final Button btnCancel = (Button) findViewById(R.id.Button_CANCEL);
       	btnCancel.setText("Cancel");
       	btnCancel.setOnClickListener(new View.OnClickListener() {
    		public void onClick(View v) {
       			Log.i(TAG, "onClick - Cancel button");
       			sipManager.Cancel(strCallID);
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
        
    @Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}

	public boolean addMessage(MCS_MSG msg)
	{	
    	String strMessage = "Message !!!";
 		
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


