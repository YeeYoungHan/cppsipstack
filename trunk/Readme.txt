================
C++ SIP stack ������Ʈ
================

1. ����

 �� ������Ʈ�� ��ǥ�� ������ ����.

 * C++ STL ��� SIP stack ����
 * C++ STL ��� SIP stack ��� IP-PBX ����

 �� ������Ʈ�� ���̼����� GPLv3 �̴�.
 �� ������Ʈ�� �����ϴ� ������ ������ ������ ����.

 �̸���: websearch@naver.com
 ��α�: http://blog.naver.com/websearch

2. ���� ����

 * SipClient 
   - C++ SIP stack �� �׽�Ʈ�ϱ� ���� SIP Ŭ���̾�Ʈ ���α׷�

 * SipParser
   - SIP �޽��� �ļ�/���� ���̺귯��

 * SipPlatform
   - C++ SIP stack ���� ���Ǵ� OS �������� ��ƿ��Ƽ ���̺귯��

 * SipStack
   - SIP stack ���̺귯��

 * SipUserAgent
   - SIP �α��� / ��ȭ ���� ���̺귯��

 * SdpParser
   - SDP �޽��� �ļ�/���� ���̺귯��

 * XmlParser
   - XML ���ڿ��� �ļ�/���� ���̺귯��

 * TestSipParser
   - SipParser ���̺귯�� �׽�Ʈ ���α׷�

 * SimpleSipServer
   - C++ SIP stack �� �̿��Ͽ��� �ʰ��� SIP ���� ���α׷��� ������ ���Դϴ�.
   - C++ SIP stack �� �̿��Ͽ� �����ϰ� SIP ������ ������ �� �ִٴ� ���� �����ֱ� ���� ���� ���α׷��̿��� 
     C++ SIP stack �� ������ �ñ��Ͻ� ���� �ҽ� �ڵ带 �� �о� ������.

 * KSipServer
   - �ѱ��� SIP ���� ���α׷�

 * KSipServerAdmin
   - KSipServer �� ����ϴ� MySQL �����ͺ��̽��� �����ϴ� �� ���α׷�
   - KSipServer ������ �� ���α׷�

 * KSipServerMonitor
   - KSipServer �ڷᱸ�� ����͸� ���α׷�

 * KSipServerLogAnalysis
   - ���� ��¥�� �α׸� �м��Ͽ��� MySQL �����ͺ��̽��� ��踦 �����ϴ� ���α׷�

 * MySQL Server 5.0
   - MS ������� MySQL Server 5.0 ���� ������ ���߿� ��� ���ϰ� ���̺귯��
   - 32bit ����

 * MySQL Server 5.5
   - MS ������� MySQL Server 5.5 ���� ������ ���߿� ��� ���ϰ� ���̺귯��
   - 64bit ����

 * FindNoLiceseFile
   - �������� ���̼��� ������ ���Ե��� ���� �ҽ� �ڵ带 �˻��ϴ� ���α׷�

 * Curio
   - C++ SIP stack �� �̿��� �ȵ���̵� SIP Ŭ���̾�Ʈ ( ���� ��ȹ�� )
   - ����ȯ [sosolili@naver.com] �Բ��� �����Ͻ� ��ȹ�Դϴ�.

3. ������ ���

 * VC++ 2008
   - SipStack.sln ����Ŭ���� ��, �����Ѵ�.

 * ������ / OSX
   - make �� �����Ѵ�.

4. MySQL ��� ���

 MySQL �� ����� ���� �� IP-PBX ���� ���� ������ ��, KSipServer ���� �̸� ����ϴ� ����� ������ ����.

 * MySQL �� ksipserver �����ͺ��̽��� �����Ѵ�.
 * KSipServer / Sql ������ SQL ���� MySQL �� ksipserver �����ͺ��̽����� �����Ѵ�.
 * KSipServer.h ���Ͽ� �Ʒ��� �ҽ� �ڵ��� �ּ��� �����Ѵ�.
   - �̹� �����Ǿ� ������ �����ϸ� �ϸ� �ȴ�.

   #define USE_MYSQL
