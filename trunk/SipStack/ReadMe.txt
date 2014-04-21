==============
SipStack 라이브러리
==============

1. 개요

 * C++ STL 라이브러리를 이용한 SIP stack 라이브러리 개발
 
2. 개발자 정보
  
 * 이영한
 * 이메일 : websearch@naver.com
 * 블로그 : http://blog.naver.com/websearch

3. 라이선스

 * 본 프로젝트의 라이선스는 GPLv3 이고 기업용 라이선스는 websearch@naver.com 으로 문의해 주세요.

4. SIP stack timer 정리

Timer    Value            Section               Meaning
----------------------------------------------------------------------
T1       500ms default    Section 17.1.1.1     RTT Estimate
T2       4s               Section 17.1.2.2     The maximum retransmit
                                               interval for non-INVITE
                                               requests and INVITE
                                               responses
T4       5s               Section 17.1.2.2     Maximum duration a
                                               message will
                                               remain in the network
Timer A  initially T1     Section 17.1.1.2     INVITE request retransmit
                                               interval, for UDP only
Timer B  64*T1            Section 17.1.1.2     INVITE transaction
                                               timeout timer
Timer C  > 3min           Section 16.6         proxy INVITE transaction
                           bullet 11            timeout
Timer D  > 32s for UDP    Section 17.1.1.2     Wait time for response
         0s for TCP/SCTP                       retransmits
Timer E  initially T1     Section 17.1.2.2     non-INVITE request
                                               retransmit interval,
                                               UDP only
Timer F  64*T1            Section 17.1.2.2     non-INVITE transaction
                                               timeout timer
Timer G  initially T1     Section 17.2.1       INVITE response
                                               retransmit interval
Timer H  64*T1            Section 17.2.1       Wait time for
                                               ACK receipt
Timer I  T4 for UDP       Section 17.2.1       Wait time for
         0s for TCP/SCTP                       ACK retransmits
Timer J  64*T1 for UDP    Section 17.2.2       Wait time for
         0s for TCP/SCTP                       non-INVITE request
                                               retransmits
Timer K  T4 for UDP       Section 17.1.2.2     Wait time for
         0s for TCP/SCTP                       response retransmits

                   Table 4: Summary of timers

