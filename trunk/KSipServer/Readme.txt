===========
KSipServer
===========

1. 개요

 * C++ SIP stack / SipUserAgent 를 이용한 한국형 IP-PBX 개발
 
 * 목적
   - C++ SIP stack / User Agent 를 이용한 한국형 IP-PBX 개발

2. 개발자 정보
  
 * 이름   : 이영한
 * 이메일 : websearch@naver.com
 * 블로그 : http://blog.naver.com/websearch

3. 라이선스

 * 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

4. 기능 설명

 KSipServer 0.06 버전의 주요 기능은 다음과 같습니다. 

 * 설정 기능 
   - XML 파일로 KSipServer 의 설정 사항을 설정하는 기능 
   - 설정 정보를 XML 파일에서 읽을지 MySQL DB 에서 읽을지 선택할 수 있는 기능 

 * 인증 기능 
   - 사용자 인증 기능 
   - XML 파일 또는 MySQL DB 에서 사용자 정보를 가져오는 기능 
   - SIP 요청 메시지에 대한 사용자 인증 기능 

 * 통화 기능 
   - 로그인된 사용자간의 전화 통화 기능 
   - 착신 전환 기능 
   - 착신 거부 기능 
   - 통화 전달 기능 ( Screened / Unscreened / Blind Transfer ) 
   - Hold / Resume 기능 
   - RTP relay 기능 ( SBC 기능 ) 
   - CDR 저장 기능 
   - B2BUA ( back-to-back user agent )

 * IP-PBX 연동 기능 
   - XML 파일 또는 MySQL DB 에서 연동할 IP-PBX 정보를 가져오는 기능 
   - IP-PBX 로그인 기능 
   - 전화번호 prefix 를 이용하여서 연동할 IP-PBX 를 선택하는 기능 
   - IP-PBX 와 연동하여서 전화 통화 연결 기능 

 * 모니터링 기능 
   - 내부 자료구조에 대한 모니터링 기능 ( KSipServerMonitor.exe 로 모니터링 할 수 있음 ) 
