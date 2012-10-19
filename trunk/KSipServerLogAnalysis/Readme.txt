================
KSipServerAnalysis 설명
================

1. 프로젝트 개요

 한국형 IP-PBX 인 KSipServer 의 어제 로그를 분석하여서 통계 데이터를 생성하는 프로그램을 개발한다.

2. 데이터베이스 스키마

 KSipServer 의 데이터베이스 스키마를 생성하려면 아래와 같이 진행하면 된다.

 * MySQL 에 ksipserver 이름의 데이터베이스를 생성한다.
 * MySQL 에 ksipserver 이름의 사용자를 등록하고 ksipserver 데이터베이스에 SELECT, INSERT, UPDATE, DELETE 할 수 있는 권한을 부여한다.
 * KSipServer / Sql 폴더의 sql 문을 ksipserver 데이터베이스에서 실행하여서 테이블을 생성한다.

3. 배포 버전 설치하는 방법

 * 압축 파일의 압축을 해제한다.
 * 2번과 같이 데이터베이스를 추가한다.
 * 압축 파일에 포함된 *.sql 문을 실행하여서 통계 관련 테이블을 생성한다.
 * LogAnalysisSetup.xml 을 실행 환경에 적합하게 수정한다.
   - 로그 폴더
   - 데이터베이스 접속 정보 
 * 매일 오전 4시에 아래와 같이 실행하도록 설정한다.
   - KSipServerLogAnalysis.exe LogAnalysisSetup.xml
   - 실행 경로 문제가 있으면 full path 로 기술해 준다.



