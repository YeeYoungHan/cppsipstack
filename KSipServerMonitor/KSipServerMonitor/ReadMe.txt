================================================================================
    MFC 라이브러리 : KSipServerMonitor 프로젝트 개요
===============================================================================

응용 프로그램 마법사에서 이 KSipServerMonitor 응용 프로그램을 
만들었습니다.  이 응용 프로그램은 MFC(Microsoft Foundation Classes)의
기본적인 사용법을 보여 주며 응용 프로그램 작성을 위한 출발점을 제공합니다.

이 파일에는 KSipServerMonitor 응용 프로그램을 구성하는 각 파일에 대한
요약 설명이 포함되어 있습니다.

KSipServerMonitor.vcproj
    응용 프로그램 마법사를 사용하여 생성된 VC++ 프로젝트의 주 프로젝트 파일입니다.
    파일을 생성한 Visual C++ 버전에 대한 정보와
    응용 프로그램 마법사를 사용하여 선택한 플랫폼, 구성 및 프로젝트
    기능에 대한 정보가 들어 있습니다.

KSipServerMonitor.h
    응용 프로그램의 기본 헤더 파일입니다.  여기에는 다른 프로젝트 관련
    헤더(Resource.h 포함)가 들어 있고 CKSipServerMonitorApp 응용 프로그램
    클래스를 선언합니다.

KSipServerMonitor.cpp
    응용 프로그램 클래스 CKSipServerMonitorApp이(가) 들어 있는 기본 응용 프로그램
    소스 파일입니다.

KSipServerMonitor.rc
    프로그램에서 사용하는 모든 Microsoft Windows 리소스의 목록입니다.
  여기에는 RES 하위 디렉터리에 저장된 아이콘, 비트맵 및 커서가
    포함됩니다.  이 파일은 Microsoft Visual C++에서
    직접 편집할 수 있습니다. 프로젝트 리소스는 1042에 있습니다.

res\KSipServerMonitor.ico
    응용 프로그램의 아이콘으로 사용되는 아이콘 파일입니다.  이 아이콘은
    주 리소스 파일인 KSipServerMonitor.rc에 의해 포함됩니다.

res\KSipServerMonitor.rc2
    이 파일에는 Microsoft Visual C++ 이외의 다른 도구에서 편집한 리소스가
    포함되어 있습니다. 리소스 편집기로 편집할 수 없는 모든 리소스는
    이 파일에 넣어야 합니다.

/////////////////////////////////////////////////////////////////////////////

주 프레임 창의 경우:
    프로젝트에는 표준 MFC 인터페이스가 포함됩니다.

MainFrm.h, MainFrm.cpp
    이 파일에는 프레임 클래스 CMainFrame이(가) 들어 있습니다.
    이 클래스는 CMDIFrameWnd에서 파생되며 모든 MDI 프레임 기능을 제어합니다.

res\Toolbar.bmp
    이 비트맵 파일은 도구 모음을 위한 바둑판식 이미지를 만드는 데 사용됩니다.
    초기 도구 모음과 상태 표시줄은 CMainFrame
    클래스에서 생성됩니다. 리소스 편집기를 사용하여 이 도구 모음 비트맵을 편집하고
    KSipServerMonitor.rc의 IDR_MAINFRAME TOOLBAR 배열을 업데이트하여 도구 모음
    단추를 추가합니다.
/////////////////////////////////////////////////////////////////////////////

자식 프레임 창의 경우:

ChildFrm.h, ChildFrm.cpp
    이 파일은 MDI 응용 프로그램에서 자식 창을 지원하는 CChildFrame
    클래스를 정의하고 구현합니다.

/////////////////////////////////////////////////////////////////////////////

응용 프로그램 마법사에서 하나의 문서 형식과 하나의 뷰가 만들어집니다.

KSipServerMonitorDoc.h, KSipServerMonitorDoc.cpp - 문서
    이 파일에는 CKSipServerMonitorDoc 클래스가 들어 있습니다.  이 파일을 편집하여
    특수한 문서 데이터를 추가하고 CKSipServerMonitorDoc::Serialize를 통해 파일
    저장 및 로드를 구현합니다.

KSipServerMonitorView.h, KSipServerMonitorView.cpp - 문서 뷰
    이 파일에는 CKSipServerMonitorView 클래스가 들어 있습니다.
    CKSipServerMonitorView 개체는 CKSipServerMonitorDoc 개체를 보는 데 사용됩니다.

res\KSipServerMonitorDoc.ico
    CKSipServerMonitorDoc 클래스의 MDI 자식 창에 대한 아이콘으로 사용되는
    아이콘 파일입니다.  이 아이콘은 주 리소스 파일인 KSipServerMonitor.rc에
    의해 포함됩니다.




/////////////////////////////////////////////////////////////////////////////

기타 기능:

ActiveX 컨트롤
    응용 프로그램에서 ActiveX 컨트롤을 사용할 수 있도록 지원합니다.

Windows 소켓
    응용 프로그램에서 TCP/IP 네트워크를 사용한 통신을 지원합니다.

/////////////////////////////////////////////////////////////////////////////

기타 표준 파일:

StdAfx.h, StdAfx.cpp
    이 파일은 미리 컴파일된 헤더(PCH) 파일인 KSipServerMonitor.pch와
    미리 컴파일된 형식 파일인 StdAfx.obj를 빌드하는 데 사용됩니다.

Resource.h
    새 리소스 ID를 정의하는 표준 헤더 파일입니다.
    Microsoft Visual C++에서 이 파일을 읽고 업데이트합니다.

KSipServerMonitor.manifest
	응용 프로그램 매니페스트 파일은 Windows XP에서 특정 버전의 Side-by-Side
	어셈블리에 대한 응용 프로그램 종속성을 설명하는 데 사용됩니다. 로더는 이 정보를
	사용하여 어셈블리 캐시에서 적절한 어셈블리를 로드하거나 응용 프로그램에서 전용
	어셈블리를 로드합니다. 응용 프로그램 매니페스트는 응용 프로그램 실행 파일과 같은
	폴더에 설치된 외부 .manifest 파일로 재배포를 위해 포함되거나 리소스의 형태로
	실행 파일에 포함될 수 있습니다.
/////////////////////////////////////////////////////////////////////////////

기타 참고:

응용 프로그램 마법사는 "TODO:"를 사용하여 추가하거나 사용자 지정해야 하는
소스 코드 부분을 나타냅니다.

응용 프로그램에서 공유 DLL의 MFC를 사용하는 경우
해당 MFC DLL을 다시 배포해야 합니다. 응용 프로그램의 언어가
운영 체제의 로캘과 다른 경우
지역화된 해당 리소스 MFC90XXX.DLL도 다시 배포해야 합니다.
이 두 가지 항목에 대한 자세한 내용은 MSDN 설명서에서
Visual C++ 응용 프로그램 다시 배포에 대한 단원을 참조하십시오.

/////////////////////////////////////////////////////////////////////////////
