# C++ SIP stack 프로젝트
C++ STL 기반으로 SIP stack 을 개발하는 프로젝트입니다.

### 개요
본 프로젝트의 목표는 다음과 같습니다.

* C++ STL 기반 SIP stack 개발
* C++ SIP stack 기반 IP-PBX 개발
* C++ SIP stack 기반 로드밸런서 개발
* C++ SIP stack 기반 SIP 서버 성능 측정 프로그램 개발
* C++ SIP stack 기반 SIP 서버 테스트 프로그램 개발
* C++ SIP stack 기반 MFC 응용 프로그램 개발
* C++ SIP stack 기반 java SIP stack 개발

### 개발자 정보
본 프로젝트를 진행하는 개발자 정보는 다음과 같습니다.

* 이메일: websearch@naver.com
* 블로그: http://blog.naver.com/websearch

위의 블로그의 "영한 오픈소스 -> SIP stack" 카테고리에 C++ SIP stack 아키텍처, 사용법 및 Q&A 등이 기술되어 있습니다.

### 라이선스

본 프로젝트의 라이선스는 GPLv3 입니다.
본 프로젝트에 대한 상용 라이선스 발급을 원하시면 개발자 정보의 이메일 주소로 연락해 주세요.


### 폴더 설명
본 프로젝트에 포함된 폴더에 대한 설명은 다음과 같습니다.

* AndroidCppSipStack
  * 안드로이드 NDK 빌드를 위한 sip stack static library 빌드용

* AndroidSipStack
  * 안드로이드용 sip stack 개발 프로젝트

* doc
  * C++ SIP stack 사용법 및 C++ SIP stack 기반으로 개발된 프로그램 사용법을 기술한 문서가 저장되어 있다.

* FindNoLiceseFile
  * 폴더에서 라이선스 문구가 포함되지 않은 소스 코드를 검색하는 프로그램

* FindThreadUnSafe
  * thread unsafe 함수를 사용하는 소스 코드를 검색하는 프로그램

* KSipServer
  * 한국형 SIP 서버 프로그램

* openssl
  * MS 윈도우용 openssl 라이브러리

* Publish
  * MS 윈도우에서 실행 가능한 실행 파일 배포 폴더

* SdpParser
  * SDP 메시지 파서/생성 라이브러리

* ServerMonitor
  * 서버 자료구조 모니터링 프로그램

* ServerPlatform
  * 서비스 서버에 공통적으로 사용되는 라이브러리

* SimpleSipServer
  * C++ SIP stack 을 이용하여서 초간단 SIP 서버 프로그램을 개발한 것입니다.
  * C++ SIP stack 을 이용하여 간단하게 SIP 서버를 개발할 수 있다는 것을 보여주기 위한 예제 프로그램이오니 C++ SIP stack 의 사용법이 궁금하신 분은 소스 코드를 꼭 읽어 보세요.

* SipCallDump
  * 패킷 덤프하여서 통화별로 SIP 및 RTP 패킷을 하나의 파일에 저장하는 서비스 프로그램입니다.

* SipClient 
  * C++ SIP stack 을 테스트하기 위한 SIP 클라이언트 프로그램
  * C++ SIP stack 으로 SIP 클라이언트를 개발하시려면 본 소스 코드를 참고하세요.

* SipClientMFC
  * C++ SIP stack 을 테스트하기 위한 MFC 기반 SIP 클라이언트 프로그램
  * MFC 기반으로 SIP 클라이언트를 개발하시려면 본 소스 코드를 참고하세요.

* SipLoadBalancer
  * SIP 로드밸런서 프로그램

* SipParser
  * SIP 메시지 파서/생성 라이브러리

* SipPlatform
  * C++ SIP stack 에서 사용되는 OS 독립적인 유틸리티 라이브러리

* SipSpeed
  * MS 윈도우용 SIP 서버 성능 측정 프로그램

* SipSpeedLinux
  * MS 윈도우/리눅스용 SIP 서버 성능 측정 프로그램

* SipStack
  * SIP stack 라이브러리

* SipTest
  * SIP 서버 기능 테스트 프로그램

* SipUserAgent
  * SIP 로그인 / 통화 관리 라이브러리

* SipUserAgentMFC
  * SipUserAgent callback 을 MFC 용으로 사용할 수 있는 라이브러리

* XmlParser
  * XML 문자열을 파서/생성 라이브러리

* TestSipParser
  * SipParser 라이브러리 테스트 프로그램

* TestSipServer
  * SimpleSipServer 소스 코드를 가져와서 테스트용으로 사용할 IP-PBX 를 개발함.
  * 테스트용이므로 클라이언트 로그인 정보를 파일로 저장하였다가 실행될 때 읽어 오는 기능이 존재함.
  * 영상 디바이스 연동 테스트용으로 개발됨.

### 컴파일 방법
본 프로젝트의 컴파일 방법은 다음과 같습니다.

* VC++ 2008
  * SipStack.sln 더블클릭한 후, 빌드한다.
  * Visual Studio 2008 버전 9.0.30729.1 SP 에서 빌드할 것을 권장합니다.

* 리눅스 / OSX
  * make 를 실행한다.

### TLS 없이 빌드하고 싶은 경우
SipStack/SipStackDefine.h 에서 아래의 소스 코드를 주석 처리한 후, 컴파일합니다.

 #define USE_TLS

### 음성 코덱만 사용하고 싶은 경우
KSipServer 에서 지정된 음성 코덱만 사용하고 싶고 음성 통신만 할 경우에는 아래의 SipUserAgent/SipUserAgentCallBack.h 에서 아래의 소스 코드를 주석 처리한 후, 컴파일합다.

 #define USE_MEDIA_LIST