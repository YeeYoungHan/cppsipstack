# SipParser 라이브러리

### 개요

* C++ STL 라이브러리를 이용한 SIP 메시지 파싱/생성 라이브러리 개발

### 개발자 정보

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 파싱 속도 향상
SipParseDefine.h 파일에 아래의 항목이 선언된 경우와 그렇지 않은 경우의 100,000 개 SIP 메시지 파싱 속도 테스트 결과는 다음과 같다.

```
#define PARSE_FAST
```

* 선언한 경우 처리 시간 = 1,377 ms ( 4.9% 성능 향상 )
* 주석 처리한 경우 처리 시간 = 1,448 ms
