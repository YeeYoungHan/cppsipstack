# C++ SIP stack 2차 튜닝 프로젝트
C++ STL 기반으로 SIP stack 을 튜닝하는 프로젝트입니다.

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

### 수정 내용

* ICT, IST 에서 ACK 검색 속도를 향상시킴
* std::list 에 포인터를 저장하여서 복사 연산을 줄임
* CSipMessage 객체를 재활용하는 기능을 추가함
* UDP 수신/전송 버퍼 크기 설정 기능을 추가

### 테스트 결과

* 50% 정도 성능 향상

### 관련 포스트

* https://blog.naver.com/websearch/222134325097 : C++ SIP stack 3차 성능 튜닝

