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

 * SipPlatform
   - C++ SIP stack 에서 사용되는 OS 독립적인 유틸리티 라이브러리

 * SipStack
   - SIP stack 라이브러리

 * SipUserAgent
   - SIP 로그인 / 통화 관리 라이브러리

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

 * KSipServerAdmin
   - KSipServer 가 사용하는 MySQL 데이터베이스와 연동하는 웹 프로그램
   - KSipServer 관리자 웹 프로그램

 * KSipServerMonitor
   - KSipServer 자료구조 모니터링 프로그램

 * KSipServerLogAnalysis
   - 어제 날짜의 로그를 분석하여서 MySQL 데이터베이스에 통계를 저장하는 프로그램

 * MySQL Server 5.0
   - MS 윈도우용 MySQL Server 5.0 에서 가져온 개발용 헤더 파일과 라이브러리
   - 32bit 버전

 * MySQL Server 5.5
   - MS 윈도우용 MySQL Server 5.5 에서 가져온 개발용 헤더 파일과 라이브러리
   - 64bit 버전

 * FindNoLiceseFile
   - 폴더에서 라이선스 문구가 포함되지 않은 소스 코드를 검색하는 프로그램

 * Curio
   - C++ SIP stack 을 이용한 안드로이드 SIP 클라이언트 ( 개발 계획중 )
   - 정종환 [sosolili@naver.com] 님께서 개발하실 계획입니다.

3. 컴파일 방법

 * VC++ 2008
   - SipStack.sln 더블클릭한 후, 빌드한다.

 * 리눅스 / OSX
   - make 를 실행한다.

4. MySQL 사용 방법

 MySQL 에 사용자 정보 및 IP-PBX 정보 등을 저장한 후, KSipServer 에서 이를 사용하는 방법은 다음과 같다.

 * MySQL 에 ksipserver 데이터베이스를 생성한다.
 * KSipServer / Sql 폴더의 SQL 문을 MySQL 의 ksipserver 데이터베이스에서 실행한다.
 * KSipServer.h 파일에 아래의 소스 코드의 주석을 해제한다.
   - 이미 해제되어 있으면 컴파일만 하면 된다.

   #define USE_MYSQL
