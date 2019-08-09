# SipCallDump

### 개요

* 통화별로 SIP, RTP 패킷덤프 파일을 생성하여서 저장하는 서비스

### 개발자 정보

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 실행 방법

* MS 윈도우에서는 winpcap 이 설치되어 있어야 한다.
  * WireShark 인스톨시 winpcap 이 인스톨될 수도 있다.
  * winpcap 이 설치되저 있지 않으면 https://www.winpcap.org 에서 winpcap 을 다운로드 받아서 설치한다.

* 설정 파일을 수정한다.
  * SipCallDump.xml 파일에서 Device 를 자신에 환경에 적합하게 수정한다.
  * MS 윈도우에서 사용하는 모든 디바이스 이름을 출력하는 방법은 아래와 같다.

```
dos> SipCallDump.exe -l
```

* 도스창에서 실행하는 방법

```
dos> SipCallDump.exe SipCallDump.xml
```

* 윈도우 서비스로 인스톨하는 방법
  * 관리자 권한으로 도스창을 실행한 후, 아래와 같이 실행한다.
  * SipCallDump.xml 파일이 SipCallDump.exe 를 포함한 폴더에 같이 존재해야 한다.

```
dos> SipCallDump.exe -i
```

### 관련 포스트

* https://blog.naver.com/websearch/221166795719 : SIP 통화별 패킷 덤프 파일을 생성하는 프로그램 1차 개발 완료

