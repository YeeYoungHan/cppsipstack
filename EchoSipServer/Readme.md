# EchoSipServer

### 개요

* C++ SIP stack / SipUserAgent 를 이용한 통화 echo 서버 개발 프로젝트

### 개발자 정보
  
* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 동작방식
EchoSipServer 의 동작방식은 다음과 같습니다.

* A 가 CallID#1 의 INVITE 를 EchoSipServer 로 전송하면 EchoSipServer 는 CallID#2 의 INVITE 를 A 에게 전송한다.
* A 가 CallID#2 의 INVITE 에 대한 200 OK 를 EchoSipServer 로 전송하면 EchoSipServer 는 CallID#1 의 INVITE 에 대한 200 OK 를 A 에게 전송한다.
* A 가 CallID#1 의 BYE 를 EchoSipServer 로 전송하면 EchoSipServer 는 CallID#2 의 BYE 를 A 에게 전송한다.

![Alt text](https://postfiles.pstatic.net/MjAxOTA0MDVfMTA2/MDAxNTU0NDQxNjkwOTM0.O96v0k7UB2hTKt5QhSDyPCjBsBHVZ5r4WEKXtehCSL0g.Q5sI7OJnWezwMPSIuIa1dD9_--KQrn_heEdVMnqX-JUg.PNG.websearch/EchoSipServer.png?type=w773)

### 관련 포스트

* https://blog.naver.com/websearch/221506237184

