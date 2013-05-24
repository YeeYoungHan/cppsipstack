안드로이드용 openssl 라이브러리 컴파일 방법

1. 개요

 본 문서는 Cygwin 에서 안드로이드 NDK 를 이용한 openssl 정적 라이브러리 컴파일 방법을 설명한다.

 작성자 : 이영한 ( websearch@naver.com )
 작성일 : 2013년 5월 24일

2. 환경 설정

 * NDK 의 컴파일러 경로를 아래와 같이 등록한다.

   # PATH=$PATH:/android-ndk-r3/build/prebuilt/windows/arm-eabi-4.4.0/bin

 * 컴파일에 필요한 컴파일러/링커 등을 아래와 같이 등록한다.

   # export CC=arm-eabi-gcc.exe
   # export AR=arm-eabi-ar.exe
   # export RANLIB=arm-eabi-ranlib.exe

3. 컴파일

 * openssl 폴더에서 아래와 같이 실행한다.

   # ./Configure linux-generic32 no-shared no-dso
   # make

4. 복사

 * 라이브러리 복사
   - libcrypto.a libssl.a 를 사용할 폴더에 복사한다.
   - 즉, AndroidSipStack\jni\lib 폴더에 복사한다.

 * 헤더 복사
   - include 폴더를 사용할 폴더에 복사한다.
   - 즉, AndroidSipStack\jni\include 폴더에 복사한다.
