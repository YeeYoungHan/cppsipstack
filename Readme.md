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

* 본 프로젝트의 라이선스는 GPLv3 입니다.
* 본 프로젝트에 대한 상용 라이선스 발급을 원하시면 websearch@naver.com 로 연락해 주세요.

### API 문서
C++ SIP stack API 문서는 아래의 홈페이지에서 확인하실 수 있습니다.

* https://yeeyounghan.github.io/doc/CppSipStack/html/index.html

### 폴더 설명
본 프로젝트에 포함된 폴더에 대한 설명은 다음과 같습니다.

* AndroidCppSipStack
  * 안드로이드 NDK 빌드를 위한 sip stack static library 빌드용

* AndroidSipStack
  * 안드로이드용 sip stack 개발 프로젝트

* doc
  * C++ SIP stack 사용법 및 C++ SIP stack 기반으로 개발된 프로그램 사용법을 기술한 문서가 저장되어 있다.

* EchoSipServer
  * 통화 발신 및 수신을 모두 수행하는 SIP 서비스 테스트용 프로그램

* FindNoLiceseFile
  * 폴더에서 라이선스 문구가 포함되지 않은 소스 코드를 검색하는 프로그램

* FindThreadUnSafe
  * thread unsafe 함수를 사용하는 소스 코드를 검색하는 프로그램

* HttpParser
  * HTTP 프로토콜 파서/생성 라이브러리

* HttpStack
  * HTTP 클라이언트/서버 라이브러리
  * WebSocket 서버 라이브러리

* KSipServer
  * 한국형 SIP 서버 프로그램

* opensrtp
  * SRTP 라이브러리

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

* StunParser
  * STUN 패킷 파서 라이브러리

* TcpStack
  * TCP 클라이언트/서버 라이브러리
  * HttpStack 에서 HTTP 서버 개발에 사용된다.

* WdpPack
  * winpcap 개발 라이브러리

* XmlParser
  * XML 문자열을 파서/생성 라이브러리

* TestSipParser
  * SipParser 라이브러리 테스트 프로그램

* TestSipServer
  * SimpleSipServer 소스 코드를 가져와서 테스트용으로 사용할 IP-PBX 를 개발함.
  * 테스트용이므로 클라이언트 로그인 정보를 파일로 저장하였다가 실행될 때 읽어 오는 기능이 존재함.
  * 영상 디바이스 연동 테스트용으로 개발됨.

* TestWebRtc
  * WebRTC 테스트 프로그

### 컴파일 방법
본 프로젝트의 컴파일 방법은 다음과 같습니다.

* VC++ 2008
  * SipStack.sln 더블클릭한 후, 빌드한다.
  * Visual Studio 2008 버전 9.0.30729.1 SP 에서 빌드할 것을 권장합니다.

* VC++ 2017
  * SipStack-vs2017.sln 더블클릭한 후, 빌드한다.

* 리눅스 / OSX
  * make 를 실행한다.

### TLS 없이 빌드하고 싶은 경우
SipStack/SipStackDefine.h 에서 아래의 소스 코드를 주석 처리한 후, 컴파일합니다.

```
 #define USE_TLS
```

### 음성 코덱만 사용하고 싶은 경우
KSipServer 에서 지정된 음성 코덱만 사용하고 싶고 음성 통신만 할 경우에는 아래의 SipUserAgent/SipUserAgentCallBack.h 에서 아래의 소스 코드를 주석 처리한 후, 컴파일합니다.

```
 #define USE_MEDIA_LIST
```

### C++ SIP stack 관련 포스트 리스트

* https://blog.naver.com/websearch/222134325097 : C++ SIP stack 3차 성능 튜닝
* https://blog.naver.com/websearch/222133591199 : C++ SIP stack 2차 성능 튜닝 - 1차 성능 튜닝 대비 47% 성능 향상
* https://blog.naver.com/websearch/222129253727 : C++ SIP stack 1차 성능 튜닝
* https://blog.naver.com/websearch/70147577937 : KSipServer 설정 파일 설명
* https://blog.naver.com/websearch/70148992602 : KSipServer 사용자 XML 파일 설명
* https://blog.naver.com/websearch/70149037102 : KSipServer 외부 IP-PBX XML 파일 설명
* https://blog.naver.com/websearch/221687115345 : SIP REGISTER 에 대한 401 응답을 1번만 수신하는 기능 개발 완료
* https://blog.naver.com/websearch/221621020855 : CSipMutex 를 std::map 등의 자료구조의 값으로 저장하면 비정상종료될 수 있는 것 같습니다.
* https://blog.naver.com/websearch/221610999793 : SipCallDump 프로그램에 fragment 된 IP 패킷을 merge 하는 기능 추가 완료
* https://blog.naver.com/websearch/221506237184 : EchoSipServer 소스 코드 추가 완료
* https://blog.naver.com/websearch/221479988167 : CSipStackSetup.m_iTcpCallBackThreadCount 설정 소개
* https://blog.naver.com/websearch/221479972346 : CSipStackSetup.m_bUseContactListenPort 설정 소개
* https://blog.naver.com/websearch/221380224274 : SipClientMFC 프로그램에 TLS 통신 기능 추가 완료
* https://blog.naver.com/websearch/221377762416 : KSipServer 성능 측정 결과 - 다섯번째 이야기 : 3,100cps 달성
* https://blog.naver.com/websearch/221369022139 : SipClientMFC 프로그램에 로컬 IP 주소 설정하는 기능 추가 완료
* https://blog.naver.com/websearch/221348970565 : SipClientMFC 프로그램에 TCP 기반 SIP 통신 기능 추가함
* https://blog.naver.com/websearch/221338166564 : KSipServer 가 RTP relay 하지 않도록 설정하는 방법
* https://blog.naver.com/websearch/221335748185 : 패킷 덤프에는 SIP 메시지가 수신되는데 KSipServer 로그에는 수신된 SIP 메시지가 존재하지 않는 경우 해결 방법
* https://blog.naver.com/websearch/221296662438 : C++ SIP stack 으로 삼성 070 서비스에 로그인하는 방법
* https://blog.naver.com/websearch/221293416636 : C++ SIP stack 에서 SIP REGISTER 메시지의 Expires 헤더 수정하는 방법
* https://blog.naver.com/websearch/221278878737 : C++ SIP stack 으로 SIP OPTIONS 메시지 전송하는 방법
* https://blog.naver.com/websearch/221262559100 : C++ SIP stack 프로젝트의 SipClientMFC 에 음성 통신 기능 1차 추가 완료
* https://blog.naver.com/websearch/221262145495 : C++ SIP stack 에서 UDP SIP 메시지 수신 및 callback 호출 구조
* https://blog.naver.com/websearch/221258225658 : VC++ 에서 KSipServer 를 디버그 모드로 실행하는 방법
* https://blog.naver.com/websearch/221227582931 : 영상 통화 가능하게 KSipServer 빌드하는 방법
* https://blog.naver.com/websearch/221207632038 : VisualStudio 2017 에서 C++ SIP stack 빌드하는 방법
* https://blog.naver.com/websearch/221201705562 : C++ SIP stack 기반 WebRTC gateway 와 Safari 음성 통신 개발 완료
* https://blog.naver.com/websearch/221187986809 : C++ SIP stack 기반 WebRTC gateway 와 Edge, Chrome, Firefox, Opera, Andriod 와 음성 통신 개발 완료
* https://blog.naver.com/websearch/221185607478 : C++ SIP stack 기반으로 WebRTC gateway 샘플 프로그램과 LG CM 연동 및 핸드폰 연동 테스트 완료
* https://blog.naver.com/websearch/221182435607 : C++ SIP stack 과 C++ HTTP stack 프로젝트에 포함된 TestWebRtc 프로그램 비교
* https://blog.naver.com/websearch/221166795719 : SIP 통화별 패킷 덤프 파일을 생성하는 프로그램 1차 개발 완료
* https://blog.naver.com/websearch/221032868433 : 초간단 윈도우용 SIP 서버 설치 방법
* https://blog.naver.com/websearch/220977772119 : C++ SIP stack 의 장점
* https://blog.naver.com/websearch/220884720374 : C++ SIP stack 컴파일시 "미리 컴파일된 헤더 파일을 열 수 없습니다" 오류 해결 방법
* https://blog.naver.com/websearch/220474064444 : SIP 클라이언트의 REGISTER, INVITE 테스트 방법
* https://blog.naver.com/websearch/220474061522 : AndroidSipStack 빌드 오류 해결 방법
* https://blog.naver.com/websearch/220418608046 : SipUserAgentMFC 기반 프로젝트 빌드 오류 해결 방법
* https://blog.naver.com/websearch/220250563240 : C++ SIP stack 사용법
* https://blog.naver.com/websearch/220071411915 : C++ SIP stack 으로 통화 호출하는 방법
* https://blog.naver.com/websearch/220071177716 : C++ SIP stack 으로 IP-PBX 에 로그인하는 방법
* https://blog.naver.com/websearch/220063359790 : C++ SIP stack 으로 SIP 클라이언트 프로그램 개발 가이드
* https://blog.naver.com/websearch/220016067688 : C++ SIP stack 구조
* https://blog.naver.com/websearch/70178105983 : MFC 환경에서 C++ SIP stack 사용 방법
* https://blog.naver.com/websearch/70143429814 : 간단한 C++ SIP stack 사용 방법
