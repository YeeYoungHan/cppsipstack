# AndroidSipStack

### ����

* �ȵ���̵忡�� ����� �� �ִ� SIP stack NDK ���̺귯�� �� java class �� �����Ѵ�.
* �ȵ���̵�� C++ SIP Stack NDK ���̺귯���� java class �� �׽�Ʈ�� �� �ִ� ���� �����Ѵ�.

### ���̼���

* �� ������Ʈ�� ���̼����� GPLv3 �̴�.

### ������ ���� 
�� ������Ʈ�� �����ϴ� ������ ������ ������ ����. �� ������Ʈ�� ���� ��� ���̼��� �߱��� ���Ͻø� �Ʒ��� �̸��� �ּҷ� ������ �ּ���.

* �̸���: websearch@naver.com
* ��α�: http://blog.naver.com/websearch

### ���� ����

* TestAndroidSipStack : ��Ŭ���� ������Ʈ
* TestAndroidSipStack2 : �ȵ���̵� ��Ʃ��� ������Ʈ

### AndroidSipStack NDK ������ ���

* cygwin �� �Ʒ��� ������ android-ndk-r8d �� ��ġ�Ѵ�.

```
/android-ndk-r12d
```

* jni �������� �Ʒ��� ���� �������Ѵ�.

```
# ./make.sh
```

### ����

* AndroidCppSipStack ������Ʈ���� C++ SIP stack �� android ���� ����� �� �ִ� static library �� �����Ѵ�.
* ������ ����� static library �� jni/lib ������ ����ȴ�.
* AndroidSipStack �� ������ ����� static library �� �̿��Ͽ��� JNI �������̽��� �߰��� shared library �� �����Ѵ�.
