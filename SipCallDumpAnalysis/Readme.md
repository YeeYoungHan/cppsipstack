# SipCallDump

### 개요

* 패킷덤프 파일을 읽어서 SIP 기반 통화에 대한 RTP 세션 정보를 출력하는 프로그램

### 개발자 정보

* 이메일 : websearch@naver.com
* 블로그 : http://blog.naver.com/websearch

### 라이선스

* 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

### 실행 방법

* MS 윈도우에서는 winpcap 이 설치되어 있어야 한다.
  * WireShark 인스톨시 winpcap 이 인스톨될 수도 있다.
  * winpcap 이 설치되저 있지 않으면 https://www.winpcap.org 에서 winpcap 을 다운로드 받아서 설치한다.

* 명령 프롬프트를 실행한 후, 아래와 같이 실행하면 dump.pcap 파일을 읽어서 전화번호가 1234 인 통화에 대한 RTP 세션 정보를 출력한다.
  * ReINVITE 로 RTP 세션 정보가 수정된 경우, log 에 관련 정보를 출력해 준다.

```
dos> SipCallDumpAnalysis.exe dump.pcap 1234
callid[0620e8e7-a3b9-4ae1-8720@test.kr] from[1234] to[0000]
 - rtp(10.25.100.41:10410)=>(10.25.220.152:24758) count(9563)
 - rtp(10.25.220.152:24758)=>(10.25.100.41:10410) count(9545)
 - log[caller rtp change (10.25.220.152:24752) => (10.25.220.152:24758)
]
```

### 관련 포스트

* https://blog.naver.com/websearch/221885139592 : 패킷 덤프 파일을 읽어서 SIP 통화에 대한 RTP 세션 정보를 출력하는 프로그램 1차 개발 완료

