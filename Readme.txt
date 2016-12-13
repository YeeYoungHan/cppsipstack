================
C++ SIP stack 프로젝트
================

1. 개요

 본 프로젝트의 목표는 다음과 같다.

 * C++ STL 기반 SIP stack 개발
 * C++ SIP stack 기반 IP-PBX 개발
 * C++ SIP stack 기반 로드밸런서 개발
 * C++ SIP stack 기반 SIP 서버 성능 측정 프로그램 개발
 * C++ SIP stack 기반 SIP 서버 테스트 프로그램 개발
 * C++ SIP stack 기반 MFC 응용 프로그램 개발
 * C++ SIP stack 기반 java SIP stack 개발

 본 프로젝트의 라이선스는 GPLv3 이다.
 본 프로젝트를 진행하는 개발자 정보는 다음과 같다. 본 프로젝트에 대한 상용 라이선스 발급을 원하시면 아래의 이메일 주소로 연락해 주세요.

 이메일: websearch@naver.com
 블로그: http://blog.naver.com/websearch

2. 폴더 설명

 * ServerPlatform
   - 서비스 서버에 공통적으로 사용되는 라이브러리

 * SipClient 
   - C++ SIP stack 을 테스트하기 위한 SIP 클라이언트 프로그램
   - C++ SIP stack 으로 SIP 클라이언트를 개발하시려면 본 소스 코드를 참고하세요.

 * SipClientMFC
   - C++ SIP stack 을 테스트하기 위한 MFC 기반 SIP 클라이언트 프로그램
   - MFC 기반으로 SIP 클라이언트를 개발하시려면 본 소스 코드를 참고하세요.

 * SipParser
   - SIP 메시지 파서/생성 라이브러리

 * SipPlatform
   - C++ SIP stack 에서 사용되는 OS 독립적인 유틸리티 라이브러리

 * SipStack
   - SIP stack 라이브러리

 * SipUserAgent
   - SIP 로그인 / 통화 관리 라이브러리

 * SipUserAgentMFC
   - SipUserAgent callback 을 MFC 용으로 사용할 수 있는 라이브러리

 * SdpParser
   - SDP 메시지 파서/생성 라이브러리

 * XmlParser
   - XML 문자열을 파서/생성 라이브러리

 * TestSipParser
   - SipParser 라이브러리 테스트 프로그램

 * SimpleSipServer
   - C++ SIP stack 을 이용하여서 초간단 SIP 서버 프로그램을 개발한 것입니다.
   - C++ SIP stack 을 이용하여 간단하게 SIP 서버를 개발할 수 있다는 것을 보여주기 위한 예제 프로그램이오니 
     C++ SIP stack 의 사용법이 궁금하신 분은 소스 코드를 꼭 읽어 보세요.

 * KSipServer
   - 한국형 SIP 서버 프로그램

 * KSipServerMonitor
   - KSipServer 자료구조 모니터링 프로그램

 * KSipServerLogAnalysis
   - 어제 날짜의 로그를 분석하여서 MySQL 데이터베이스에 통계를 저장하는 프로그램

 * SipLoadBalancer
   - SIP 로드밸런서 프로그램

 * SipLoadBalancerMonitor
   - SIP 로드밸런서 자료구조 모니터링 프로그램

 * FindNoLiceseFile
   - 폴더에서 라이선스 문구가 포함되지 않은 소스 코드를 검색하는 프로그램

 * TestSipServer
   - SimpleSipServer 소스 코드를 가져와서 테스트용으로 사용할 IP-PBX 를 개발함.
   - 테스트용이므로 클라이언트 로그인 정보를 파일로 저장하였다가 실행될 때 읽어 오는 기능이 존재함.
   - 영상 디바이스 연동 테스트용으로 개발됨.

 * McuControlServer
   - TestSipServer 소스 코드를 가져와서 테스트용으로 사용할 IP-PBX 를 개발함.
   - Radvision MCU 와 연동하여서 영상 디바이스 연결 테스트용으로 개발됨.

 * AndroidCppSipStack
   - 안드로이드 NDK 빌드를 위한 sip stack static library 빌드용

 * AndroidSipStack
   - 안드로이드용 sip stack 개발 프로젝트

 * SipSpeed
   - SIP 서버 성능 측정 프로그램

 * SipTest
   - SIP 서버 기능 테스트 프로그램

 * doc
   - C++ SIP stack 사용법 및 C++ SIP stack 기반으로 개발된 프로그램 사용법을 기술한 문서가 저장되어 있다.

3. 컴파일 방법

 * VC++ 2008
   - SipStack.sln 더블클릭한 후, 빌드한다.
   - Visual Studio 2008 버전 9.0.30729.1 SP 에서 빌드할 것을 권장합니다.

 * 리눅스 / OSX
   - make 를 실행한다.

4. TLS 없이 빌드하고 싶은 경우

 SipStack/SipStackDefine.h 에서 아래의 소스 코드를 주석 처리한 후, 컴파일한다.

 #define USE_TLS

5. 음성 코덱만 사용하고 싶은 경우

 KSipServer 에서 지정된 음성 코덱만 사용하고 싶고 음성 통신만 할 경우에는 아래의 SipUserAgent/SipUserAgentCallBack.h 에서 아래의 소스 코드를 주석 처리한 후, 컴파일한다.

 #define USE_MEDIA_LIST