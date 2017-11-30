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

#ifndef _HTTP_STATUS_CODE_H_
#define _HTTP_STATUS_CODE_H_

#define HTTP_TRYING                        100
#define HTTP_SWITCHING_PROTOCOLS           101
#define HTTP_RINGING                       180
#define HTTP_CALL_IS_BEING_FORWARDED       181
#define HTTP_QUEUED                        182
#define HTTP_SESSION_PROGRESS              183
#define HTTP_OK                            200
#define HTTP_ACCEPTED                      202
#define HTTP_MULTIPLE_CHOICES              300
#define HTTP_MOVED_PERMANENTLY             301
#define HTTP_MOVED_TEMPORARILY             302
#define HTTP_USE_PROXY                     305
#define HTTP_ALTERNATIVE_SERVICE           380
#define HTTP_BAD_REQUEST                   400
#define HTTP_UNAUTHORIZED                  401
#define HTTP_PAYMENT_REQUIRED              402
#define HTTP_FORBIDDEN                     403
#define HTTP_NOT_FOUND                     404
#define HTTP_METHOD_NOT_ALLOWED            405
#define HTTP_NOT_ACCEPTABLE								406
#define HTTP_PROXY_AUTHENTICATION_REQUIRED 407
#define HTTP_REQUEST_TIME_OUT              408
#define HTTP_CONFLICT											409
#define HTTP_GONE                          410
#define HTTP_LENGTH_REQUIRED								411
#define HTTP_CONDITIONAL_REQUEST_FAILED		412
#define HTTP_REQUEST_ENTITY_TOO_LARGE      413
#define HTTP_REQUEST_URI_TOO_LARGE         414
#define HTTP_UNSUPPORTED_MEDIA_TYPE        415
#define HTTP_UNSUPPORTED_URI_SCHEME        416
#define HTTP_BAD_EXTENSION                 420
#define HTTP_EXTENSION_REQUIRED            421
#define HTTP_SESSION_INTERVAL_TOO_SMALL		422
#define HTTP_INTERVAL_TOO_BRIEF            423
#define HTTP_TEMPORARILY_UNAVAILABLE       480
#define HTTP_CALL_TRANSACTION_DOES_NOT_EXIST 481
#define HTTP_LOOP_DETECTED                 482
#define HTTP_TOO_MANY_HOPS                 483
#define HTTP_ADDRESS_INCOMPLETE            484
#define HTTP_AMBIGUOUS                     485
#define HTTP_BUSY_HERE                     486
#define HTTP_REQUEST_TERMINATED            487
#define HTTP_NOT_ACCEPTABLE_HERE           488
#define HTTP_BAD_EVENT                     489
#define HTTP_REQUEST_PENDING               491
#define HTTP_UNDECIPHERABLE                493
#define HTTP_INTERNAL_SERVER_ERROR         500
#define HTTP_NOT_IMPLEMENTED               501
#define HTTP_BAD_GATEWAY                   502
#define HTTP_SERVICE_UNAVAILABLE           503
#define HTTP_SERVER_TIME_OUT               504
#define HTTP_VERSION_NOT_SUPPORTED         505
#define HTTP_MESSAGE_TOO_LARGE             513
#define HTTP_BUSY_EVRYWHERE                600
#define HTTP_DECLINE                       603
#define HTTP_DOES_NOT_EXIST_ANYWHERE       604
#define HTTP_CONNECT_ERROR									660
#define HTTP_RING_TIMEOUT_ERROR						661

const char * GetReasonPhrase( int iSipCode );

#endif
