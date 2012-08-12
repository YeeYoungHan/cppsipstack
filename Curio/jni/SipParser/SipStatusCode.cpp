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

#include "SipStatusCode.h"

const char * GetReasonPhrase( int iSipCode )
{
	switch( iSipCode )
	{
	case SIP_TRYING:											return "Trying";
	case SIP_RINGING:                     return "Ringing";
	case SIP_CALL_IS_BEING_FORWARDED:     return "Call Is Being Forwarded";
	case SIP_QUEUED:                      return "Queued";
	case SIP_SESSION_PROGRESS:            return "Session Progress";
	case SIP_OK:                          return "OK";
	case SIP_ACCEPTED:                    return "Accepted";
	case SIP_MULTIPLE_CHOICES:            return "Multiple Choices";
	case SIP_MOVED_PERMANENTLY:           return "Moved Permanently";
	case SIP_MOVED_TEMPORARILY:           return "Moved Temporarily";
	case SIP_USE_PROXY:                   return "Use Proxy";
	case SIP_ALTERNATIVE_SERVICE:         return "Alternative Service";
	case SIP_BAD_REQUEST:                 return "Bad Request";
	case SIP_UNAUTHORIZED:                return "Unauthorized";
	case SIP_PAYMENT_REQUIRED:            return "Payment Required";
	case SIP_FORBIDDEN:                   return "Forbidden";
	case SIP_NOT_FOUND:                   return "Not Found";
	case SIP_METHOD_NOT_ALLOWED:          return "Method Not Allowed";
	case SIP_NOT_ACCEPTABLE:							return "Not Acceptable";
	case SIP_PROXY_AUTHENTICATION_REQUIRED:	return "Proxy Authentication Required";
	case SIP_REQUEST_TIME_OUT:            return "Request Timeout";
	case SIP_CONFLICT:										return "Conflict";
	case SIP_GONE:                        return "Gone";
	case SIP_LENGTH_REQUIRED:							return "Length Required";
	case SIP_CONDITIONAL_REQUEST_FAILED:	return "Conditional Request Failed";
	case SIP_REQUEST_ENTITY_TOO_LARGE:    return "Request Entity Too Large";
	case SIP_REQUEST_URI_TOO_LARGE:       return "Request-URI Too Large";
	case SIP_UNSUPPORTED_MEDIA_TYPE:      return "Unsupported Media Type";
	case SIP_UNSUPPORTED_URI_SCHEME:      return "Unsupported Uri Scheme";
	case SIP_BAD_EXTENSION:               return "Bad Extension";
	case SIP_EXTENSION_REQUIRED:          return "Extension Required";
	case SIP_SESSION_INTERVAL_TOO_SMALL:	return "Session Interval Too Small";
	case SIP_INTERVAL_TOO_BRIEF:          return "Interval Too Brief";
	case SIP_TEMPORARILY_UNAVAILABLE:     return "Temporarily not available";
	case SIP_CALL_TRANSACTION_DOES_NOT_EXIST: return "Call Leg/Transaction Does Not Exist";
	case SIP_LOOP_DETECTED:               return "Loop Detected";
	case SIP_TOO_MANY_HOPS:               return "Too Many Hops";
	case SIP_ADDRESS_INCOMPLETE:          return "Address Incomplete";
	case SIP_AMBIGUOUS:                   return "Ambiguous";
	case SIP_BUSY_HERE:                   return "Busy Here";
	case SIP_REQUEST_TERMINATED:          return "Request Cancelled";
	case SIP_NOT_ACCEPTABLE_HERE:         return "Not Acceptable Here";
	case SIP_BAD_EVENT:                   return "Bad Event";
	case SIP_REQUEST_PENDING:             return "Request Pending";
	case SIP_UNDECIPHERABLE:              return "Undecipherable";
	case SIP_INTERNAL_SERVER_ERROR:       return "Internal Server Error";
	case SIP_NOT_IMPLEMENTED:             return "Not Implemented";
	case SIP_BAD_GATEWAY:                 return "Bad Gateway";
	case SIP_SERVICE_UNAVAILABLE:         return "Service Unavailable";
	case SIP_SERVER_TIME_OUT:             return "Server Timeout";
	case SIP_VERSION_NOT_SUPPORTED:       return "SIP Version not supported";
	case SIP_MESSAGE_TOO_LARGE:           return "Message Too Large";
	case SIP_BUSY_EVRYWHERE:              return "Busy Everywhere";
	case SIP_DECLINE:                     return "Decline";
	case SIP_DOES_NOT_EXIST_ANYWHERE:     return "Does not exist anywhere";
	}

	return "Not Acceptable";
}
