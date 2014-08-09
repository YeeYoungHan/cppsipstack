================
KSipServerLogAnalysis
================

1. 프로젝트 개요

 한국형 IP-PBX 인 KSipServer 의 어제 로그를 분석하여서 통계 데이터를 생성하는 프로그램을 개발한다.

2. 개발자 정보
  
 * 이름   : 이영한
 * 이메일 : websearch@naver.com
 * 블로그 : http://blog.naver.com/websearch

3. 라이선스

 * 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

4. 배포 버전 설치하는 방법

 * 압축 파일의 압축을 해제한다.
 * LogAnalysisSetup.xml 을 실행 환경에 적합하게 수정한다.
   - 로그 폴더
   - 데이터베이스 접속 정보 
 * 매일 오전 4시에 아래와 같이 실행하도록 설정한다.
   - KSipServerLogAnalysis.exe LogAnalysisSetup.xml
   - 실행 경로 문제가 있으면 full path 로 기술해 준다.



