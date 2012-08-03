================
C++ SIP stack 프로젝트
================

1. 개요

 본 프로젝트의 목표는 다음과 같다.

 * C++ STL 기반 SIP stack 개발
 * C++ STL 기반 SIP stack 기반 IP-PBX 개발

 본 프로젝트의 라이선스는 GPLv3 이다.
 본 프로젝트를 진행하는 개발자 정보는 다음과 같다.

 이메일: websearch@naver.com
 블로그: http://blog.naver.com/websearch

2. 폴더 설명

 * SipClient 
   - C++ SIP stack 을 테스트하기 위한 SIP 클라이언트 프로그램

 * SipParser
   - SIP 메시지 파서/생성 라이브러리

 * SipStack
   - SIP stack 라이브러리

 * SipUserAgent
   - SIP 로그인 / 통화 관리 라이브러리

 * SdpParser
   - SDP 메시지 파서/생성 라이브러리

 * TestSipParser
   - SipParser 라이브러리 테스트 프로그램

 * SimpleSipServer
   - C++ SIP stack 을 이용하여서 초간단 SIP 서버 프로그램을 개발한 것입니다.
   - C++ SIP stack 을 이용하여 간단하게 SIP 서버를 개발할 수 있다는 것을 보여주기 위한 예제 프로그램이오니 
     C++ SIP stack 의 사용법이 궁금하신 분은 소스 코드를 꼭 읽어 보세요.

3. 컴파일 방법

 * VC++ 2008
   - SipStack.sln 더블클릭한 후, 빌드한다.

 * 리눅스 / OSX
   - make 를 실행한다.
