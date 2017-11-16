======
개요
======

* 안드로이드에서 사용할 수 있는 SIP stack NDK 라이브러리 및 java class 를 개발한다.
* 안드로이드용 C++ SIP Stack NDK 라이브러리와 java class 를 테스트할 수 있는 앱을 개발한다.

 본 프로젝트의 라이선스는 GPLv3 이다.
 본 프로젝트를 진행하는 개발자 정보는 다음과 같다. 본 프로젝트에 대한 상용 라이선스 발급을 원하시면 아래의 이메일 주소로 연락해 주세요.

 이메일: websearch@naver.com
 블로그: http://blog.naver.com/websearch

=============================
AndroidSipStack NDK 컴파일 방법
=============================

1. cygwin 의 아래의 폴더에 android-ndk-r8d 를 설치한다.
  - /android-ndk-r8d

2. jni 폴더에서 아래와 같이 컴파일한다.
  # ./make.sh

======
참고
======

* AndroidCppSipStack 프로젝트에서 C++ SIP stack 을 android 에서 사용할 수 있는 static library 로 빌드한다.
* 위에서 빌드된 static library 는 jni/lib 폴더에 복사된다.
* AndroidSipStack 는 위에서 빌드된 static library 를 이용하여서 JNI 인터페이스를 추가한 shared library 를 생성한다.
