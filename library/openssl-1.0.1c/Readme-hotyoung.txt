�ȵ���̵�� openssl ���̺귯�� ������ ���

1. ����

 �� ������ Cygwin ���� �ȵ���̵� NDK �� �̿��� openssl ���� ���̺귯�� ������ ����� �����Ѵ�.

 �ۼ��� : �̿��� ( websearch@naver.com )
 �ۼ��� : 2013�� 5�� 24��

2. ȯ�� ����

 * NDK �� �����Ϸ� ��θ� �Ʒ��� ���� ����Ѵ�.

   # PATH=$PATH:/android-ndk-r3/build/prebuilt/windows/arm-eabi-4.4.0/bin

 * �����Ͽ� �ʿ��� �����Ϸ�/��Ŀ ���� �Ʒ��� ���� ����Ѵ�.

   # export CC=arm-eabi-gcc.exe
   # export AR=arm-eabi-ar.exe
   # export RANLIB=arm-eabi-ranlib.exe

3. ������

 * openssl �������� �Ʒ��� ���� �����Ѵ�.

   # ./Configure linux-generic32 no-shared no-dso
   # make

4. ����

 * ���̺귯�� ����
   - libcrypto.a libssl.a �� ����� ������ �����Ѵ�.
   - ��, AndroidSipStack\jni\lib ������ �����Ѵ�.

 * ��� ����
   - include ������ ����� ������ �����Ѵ�.
   - ��, AndroidSipStack\jni\include ������ �����Ѵ�.
