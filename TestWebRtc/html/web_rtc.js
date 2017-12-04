var localStream, remoteStream;
var pc;

// 아래와 같이 설정하면 stun 서버와 연동할 수 있도록 ice candidate 가 생성된다.
//var pc_config = {'iceServers': [{'url': 'stun:stun.l.google.com:19302'}]};

// 아래와 같이 설정하면 P2P 통신을 위한 ice candidate 가 생성된다.
var pc_config = null;

//
var pc_constraints = { 'optional': [{'DtlsSrtpKeyAgreement': true} ]};
var sdpConstraints = {};

var localVideo = document.getElementById("localVideo");
var remoteVideo = document.getElementById("remoteVideo");

var btnRegister = document.getElementById("btnRegister");
var btnInvite = document.getElementById("btnInvite");
var btnAccept = document.getElementById("btnAccept");
var btnDecline = document.getElementById("btnDecline");
var btnBye = document.getElementById("btnBye");
var lyLog = document.getElementById('log');

var iLogMaxRowCount = 3000;
var iLogRowCount = 0;

InitButton();
startLocal();

function Log(strLog)
{
  var clsDate = new Date();
  var strTime = "[" + clsDate.getHours() + ":" + clsDate.getMinutes() + ":" + clsDate.getSeconds() + "] ";

  if( iLogRowCount == iLogMaxRowCount )
  {
    lyLog.innerHTML = strTime + strLog + "<br>";
    iLogRowCount = 0;
  }
  else
  {
    lyLog.innerHTML += strTime + strLog + "<br>";
  }

  ++iLogRowCount;

  //console.log(strLog);
}

function InitButton()
{
	btnRegister.disabled = false;
	btnInvite.disabled = true;
	btnAccept.disabled = true;
	btnDecline.disabled = true;
	btnBye.disabled = true;
}

function handleLocalMedia(stream)
{
  if (window.URL)
  {
    localVideo.src = URL.createObjectURL(stream);
  }
  else
  {
    localVideo.src = stream;
  }
  localStream = stream;

  // 로컬 마이크에서 입력된 음원을 출력하지 않기 위한 기능
  localVideo.muted = true;
}

function handleUserMediaError(error)
{
  Log("#### handleUserMediaError ####");
  Log("error(" + error.name + ")");
}

function startLocal()
{
  navigator.getUserMedia = navigator.getUserMedia || navigator.webkitGetUserMedia || navigator.mozGetUserMedia;
  navigator.getUserMedia( { audio:true, video:false }, handleLocalMedia, handleUserMediaError );
}

function stopPeer()
{
  if( pc ) pc.close();
  pc = null;
  remoteVideo.src = null;
}

function createOffer()
{
  Log( "#### createOffer ####")

  if( navigator.webkitGetUserMedia )
  {
    // Chrome
    RTCPeerConnection = webkitRTCPeerConnection;
  }
  else if( navigator.mozGetUserMedia )
  {
    // Firefox
    RTCPeerConnection = mozRTCPeerConnection;
    RTCSessionDescription = mozRTCSessionDescription;
    RTCIceCandidate = mozRTCIceCandidate;
  }

  pc = new RTCPeerConnection(pc_config, pc_constraints);
  pc.addStream(localStream);
  pc.onicecandidate = setIceCandidateOffer;
  pc.onaddstream = handleRemoteStreamAdded;
  pc.onremovestream = handleRemoteStreamRemoved;

  pc.createOffer( setLocalOffer, onSignalingError, sdpConstraints );
}

function createAnswer(strSdp)
{
  Log("#### createAnswer sdp(" + strSdp + ")");

  if( navigator.webkitGetUserMedia )
  {
    // Chrome
    RTCPeerConnection = webkitRTCPeerConnection;
  }
  else if( navigator.mozGetUserMedia )
  {
    // Firefox
    RTCPeerConnection = mozRTCPeerConnection;
    RTCSessionDescription = mozRTCSessionDescription;
    RTCIceCandidate = mozRTCIceCandidate;
  }

  pc = new RTCPeerConnection(pc_config, pc_constraints);
  pc.addStream(localStream);
  pc.onicecandidate = setIceCandidateAnswer;
  pc.onaddstream = handleRemoteStreamAdded;
  pc.onremovestream = handleRemoteStreamRemoved;

  var sd = new RTCSessionDescription();
  sd.sdp = strSdp;
  sd.type = "offer";

  pc.setRemoteDescription(sd);
  pc.createAnswer(setLocalAnswer, onSignalingError, sdpConstraints);
}

function handleRemoteStreamAdded(event)
{
  Log("#### handleRemoteStreamAdded ####" );

  if (window.URL)
  {
    remoteVideo.src = URL.createObjectURL(event.stream);
  }
  else
  {
    remoteVideo.src = event.stream;
  }

  remoteStream = event.stream;
}

function handleRemoteStreamRemoved(event)
{
  Log( "#### handleRemoteStreamRemoved ####" );
}

function setIceCandidateOffer(event)
{
  if( event.candidate == null )
  {
    Log( "setIceCandidateOffer(null)" );
    //Log( "local sdp(" + pc.localDescription.sdp + ")" );

    Invite( pc.localDescription.sdp );
  }
  else
  {
    //console.log( "handleIceCandidate(" + event.candidate.candidate + ") sdpMid(" + event.candidate.sdpMid + ")" );
  }
}

function setIceCandidateAnswer(event)
{
  if( event.candidate == null )
  {
    Log( "setIceCandidateAnswer(null)" );
    //Log( "local sdp(" + pc.localDescription.sdp + ")" );

    Accept(pc.localDescription.sdp);
  }
  else
  {
    //console.log( "handleIceCandidate(" + event.candidate.candidate + ") sdpMid(" + event.candidate.sdpMid + ")" );
  }
}

function setLocalOffer(sessionDescription)
{
  pc.setLocalDescription(sessionDescription);

  Log("createOffer result sdp(" + sessionDescription.sdp + ") type(" + sessionDescription.type + ")" );

  // IceCandidate 를 가져오기 전에 호출된다. IceCandidate callback 호출에서 Invite 메소드를 호출한다.
}

function setLocalAnswer(sessionDescription) {
  pc.setLocalDescription(sessionDescription);

  Log("createAnswer result sdp(" + sessionDescription.sdp + ") type(" + sessionDescription.type + ")" );

  // IceCandidate 를 가져오기 전에 호출된다. IceCandidate callback 호출에서 Accept 메소드를 호출한다.
}

function onSignalingError(error)
{
  Log('Failed to create signaling message : ' + error.name);
}

function setAnswer(strSdp)
{
  var sd = new RTCSessionDescription();
  sd.sdp = strSdp;
  sd.type = "answer";

  pc.setRemoteDescription(sd);

  Log('setAnswer remote sdp(' + strSdp + ")" );
}
