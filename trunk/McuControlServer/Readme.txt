============
McuControlServer
============

1. 개요

 * C++ SIP stack 을 이용한 영상/음성 디바이스 테스트용 SIP 서버 개발
 
 * 목적
   - C++ SIP stack 을 이용하여서 얼마나 쉽게 SIP 서버를 개발할 수 있는지 보여주기 위한 예제 프로그램
   - C++ SIP stack 을 사용하는 방법을 알려주기 위한 예제 프로그램
   - 영상/음성 디바이스 연동 테스트
   - Radvision MCU 와 연동하여서 1:1 통화를 회의방 연동으로 처리하는 기능 테스트

 * 주요 기능
   - SIP REGISTER 수신시 사용자 자료구조에 저장하는 기능
   - SIP INVITE / CANCEL / ACK / BYE 전달 기능
   - 71 로 시작되는 전화번호는 MCU 의 회의방으로 연결하는 기능
   - 1:1 통화 요청이 수신되면 MCU 에 회의방을 생성한 후, 1:1 통화가 MCU 연동으로 동작하는 기능

2. 개발자 정보
  
 * 이영한
 * 이메일 : websearch@naver.com
 * 블로그 : http://blog.naver.com/websearch

3. 라이선스

 * 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.
