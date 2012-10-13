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


