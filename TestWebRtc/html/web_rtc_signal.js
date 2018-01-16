var gstrUserId = "";
var gstrUserPw = "";
var gstrSipServerIp = "";
var gstrToId = "";
var gstrSdp = "";
var ws;

btnInvite.disabled = true;
btnAccept.disabled = true;
btnDecline.disabled = true;
btnBye.disabled = true;

function StartSession()
{
  var txtUserId = document.getElementById('user_id');
  var txtUserPw = document.getElementById('user_pw');
  var txtSipServerIp = document.getElementById('sip_server_ip');
  
  gstrUserId = txtUserId.value;
  gstrUserPw = txtUserPw.value;
  gstrSipServerIp = txtSipServerIp.value;

  if( gstrUserId.length == 0 )
  {
    alert( "user id is not defined" );
    return;
  }
  
  if( gstrUserPw.length == 0 )
  {
  	alert( "user pw is not defined" );
    return;
  }
  
  if( gstrSipServerIp.length == 0 )
  {
  	alert( "sip server ip is not defined" );
    return;
  }
  
  if( ws == null )
  {
  	ws = new WebSocket("ws://" + window.location.hostname + ":8080");
  	
  	// websocket 서버에 연결되면 연결 메시지를 화면에 출력한다.
		ws.onopen = function(e){
			Send( "req|register|" + gstrUserId + "|" + gstrUserPw + "|" + gstrSipServerIp );
		};

		// websocket 에서 수신한 메시지를 화면에 출력한다.
		ws.onmessage = function(e){
			
			Log("Recv[" + e.data + "]");
			
			var arrData = e.data.split("|");
			
			switch( arrData[0] )
			{
			case "res":
				switch( arrData[1] )
				{
				case "register":
					if( arrData[2] == '200' )
					{
						if( btnRegister.disabled == false )
						{
							btnRegister.disabled = true;
							btnInvite.disabled = false;
						}
					}
					else
					{
						
					}
					break;
				case "invite":
					if( arrData[2] == '200' )
					{
	          btnInvite.disabled = true;
	          btnAccept.disabled = true;
	          btnDecline.disabled = true;
	          btnBye.disabled = false;
          }
          else
          {
          	var iStatusCode = parseInt( arrData[2] );
          	
          	if( iStatusCode > 200 )
          	{
          		stopPeer();
          		btnInvite.disabled = false;
          	}
          	else if( iStatusCode == 180 )
          	{
          		setAnswer( arrData[3] );
          	}
          }
					break;
				}
				break;
			case "req":
				switch( arrData[1] )
				{
				case "invite":
					gstrToId = arrData[2];
					gstrSdp = arrData[3];
          var txtPeerId = document.getElementById('peer_id');
          txtPeerId.value = gstrToId;

          Log("Invite event peer(" + gstrToId + ") sdp(" + arrData[3]+ ")" );
          //createAnswer( clsData.sdp );

          btnInvite.disabled = true;
          btnAccept.disabled = false;
          btnDecline.disabled = false;
          btnBye.disabled = true;
					break;
				case "bye":
					gstrToId = "";
          stopPeer();
          btnInvite.disabled = false;
  				btnBye.disabled = true;
          break;
				}
			}
		};

		// websocket 세션이 종료되면 화면에 출력한다.
		ws.onclose = function(e){
			ws = null;
			InitButton();
			Log("WebSocket is closed");
		}
  }
}

function Send( data )
{
	ws.send(data);
	Log( "Send[" + data + "]" );
}

function Invite( strSdp )
{
	Send( "req|invite|" + gstrToId + "|" + strSdp );
}

/** 전화 수신한다. */
function Accept( strSdp )
{
	Send( "res|invite|200|" + strSdp );
}

function SendInvite()
{
  var txtPeerId = document.getElementById('peer_id');
  var strPeerId = txtPeerId.value;

  if( strPeerId.length == 0 )
  {
    alert( 'peer id is not defined' );
    return;
  }

  btnInvite.disabled = true;
  btnAccept.disabled = true;
  btnDecline.disabled = true;
  btnBye.disabled = true;

  gstrToId = strPeerId;
  createOffer();

  //Invite( "o=" + strPeerId );
}

function SendAccept()
{
  btnInvite.disabled = true;
  btnAccept.disabled = true;
  btnDecline.disabled = true;
  btnBye.disabled = false;

  createAnswer(gstrSdp);
  //Accept( "o=accept" );
}

/** INVITE 거절 응답을 전송한다. */
function SendDecline()
{
	Send( "res|invite|603" );
	
	btnInvite.disabled = false;
  btnAccept.disabled = true;
  btnDecline.disabled = true;
}

/** BYE 를 전송한다. */
function SendBye()
{
  stopPeer();
  Send( "req|bye" );
  gstrToId = "";
  btnInvite.disabled = false;
  btnBye.disabled = true;
}

