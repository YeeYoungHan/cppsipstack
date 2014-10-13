/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef _SIP_STATUS_CODE_H_
#define _SIP_STATUS_CODE_H_

#define SIP_TRYING                        100
#define SIP_RINGING                       180
#define SIP_CALL_IS_BEING_FORWARDED       181
#define SIP_QUEUED                        182
#define SIP_SESSION_PROGRESS              183
#define SIP_OK                            200
#define SIP_ACCEPTED                      202
#define SIP_MULTIPLE_CHOICES              300
#define SIP_MOVED_PERMANENTLY             301
#define SIP_MOVED_TEMPORARILY             302
#define SIP_USE_PROXY                     305
#define SIP_ALTERNATIVE_SERVICE           380
#define SIP_BAD_REQUEST                   400
#define SIP_UNAUTHORIZED                  401
#define SIP_PAYMENT_REQUIRED              402
#define SIP_FORBIDDEN                     403
#define SIP_NOT_FOUND                     404
#define SIP_METHOD_NOT_ALLOWED            405
#define SIP_NOT_ACCEPTABLE								406
#define SIP_PROXY_AUTHENTICATION_REQUIRED 407
#define SIP_REQUEST_TIME_OUT              408
#define SIP_CONFLICT											409
#define SIP_GONE                          410
#define SIP_LENGTH_REQUIRED								411
#define SIP_CONDITIONAL_REQUEST_FAILED		412
#define SIP_REQUEST_ENTITY_TOO_LARGE      413
#define SIP_REQUEST_URI_TOO_LARGE         414
#define SIP_UNSUPPORTED_MEDIA_TYPE        415
#define SIP_UNSUPPORTED_URI_SCHEME        416
#define SIP_BAD_EXTENSION                 420
#define SIP_EXTENSION_REQUIRED            421
#define SIP_SESSION_INTERVAL_TOO_SMALL		422
#define SIP_INTERVAL_TOO_BRIEF            423
#define SIP_TEMPORARILY_UNAVAILABLE       480
#define SIP_CALL_TRANSACTION_DOES_NOT_EXIST 481
#define SIP_LOOP_DETECTED                 482
#define SIP_TOO_MANY_HOPS                 483
#define SIP_ADDRESS_INCOMPLETE            484
#define SIP_AMBIGUOUS                     485
#define SIP_BUSY_HERE                     486
#define SIP_REQUEST_TERMINATED            487
#define SIP_NOT_ACCEPTABLE_HERE           488
#define SIP_BAD_EVENT                     489
#define SIP_REQUEST_PENDING               491
#define SIP_UNDECIPHERABLE                493
#define SIP_INTERNAL_SERVER_ERROR         500
#define SIP_NOT_IMPLEMENTED               501
#define SIP_BAD_GATEWAY                   502
#define SIP_SERVICE_UNAVAILABLE           503
#define SIP_SERVER_TIME_OUT               504
#define SIP_VERSION_NOT_SUPPORTED         505
#define SIP_MESSAGE_TOO_LARGE             513
#define SIP_BUSY_EVRYWHERE                600
#define SIP_DECLINE                       603
#define SIP_DOES_NOT_EXIST_ANYWHERE       604
#define SIP_CONNECT_ERROR									660
#define SIP_RING_TIMEOUT_ERROR						661

const char * GetReasonPhrase( int iSipCode );

#endif
