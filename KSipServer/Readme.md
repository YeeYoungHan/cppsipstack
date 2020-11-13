# KSipServer

### 개요

* C++ SIP stack / SipUserAgent 를 이용한 한국형 IP-PBX 개발

### 개발자 정보
  
* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 기능 설명
KSipServer 0.06 버전의 주요 기능은 다음과 같습니다. 

* 설정 기능 
  * XML 파일로 KSipServer 의 설정 사항을 설정하는 기능 
  * 설정 정보를 XML 파일에서 읽을지 MySQL DB 에서 읽을지 선택할 수 있는 기능 

* 인증 기능 
  * 사용자 인증 기능 
  * XML 파일 또는 MySQL DB 에서 사용자 정보를 가져오는 기능 
  * SIP 요청 메시지에 대한 사용자 인증 기능 

* 통화 기능 
  * 로그인된 사용자간의 전화 통화 기능 
  * 착신 전환 기능 
  * 착신 거부 기능 
  * 통화 전달 기능 ( Screened / Unscreened / Blind Transfer ) 
  * Hold / Resume 기능 
  * RTP relay 기능 ( SBC 기능 ) 
  * CDR 저장 기능 
  * B2BUA ( back-to-back user agent )

* IP-PBX 연동 기능 
  * XML 파일 또는 MySQL DB 에서 연동할 IP-PBX 정보를 가져오는 기능 
  * IP-PBX 로그인 기능 
  * 전화번호 prefix 를 이용하여서 연동할 IP-PBX 를 선택하는 기능 
  * IP-PBX 와 연동하여서 전화 통화 연결 기능 

* 모니터링 기능 
  * 내부 자료구조에 대한 모니터링 기능 ( KSipServerMonitor.exe 로 모니터링 할 수 있음 )

### 관련 포스트

* https://blog.naver.com/websearch/70147577937 : KSipServer 설정 파일 설명
* https://blog.naver.com/websearch/70148992602 : KSipServer 사용자 XML 파일 설명
* https://blog.naver.com/websearch/70149037102 : KSipServer 외부 IP-PBX XML 파일 설명
* https://blog.naver.com/websearch/221377762416 : KSipServer 성능 측정 결과 - 다섯번째 이야기 : 3,100cps 달성
* https://blog.naver.com/websearch/221338166564 : KSipServer 가 RTP relay 하지 않도록 설정하는 방법
* https://blog.naver.com/websearch/221335748185 : 패킷 덤프에는 SIP 메시지가 수신되는데 KSipServer 로그에는 수신된 SIP 메시지가 존재하지 않는 경우 해결 방법
* https://blog.naver.com/websearch/221258225658 : VC++ 에서 KSipServer 를 디버그 모드로 실행하는 방법
* https://blog.naver.com/websearch/221227582931 : 영상 통화 가능하게 KSipServer 빌드하는 방법

