/* File: SipWrapper.i */
%module (directors="1") SipWrapper

%include "typemaps.i"
%include "various.i"

%{
#include "SipClient.h"
%}

%include "SipClient.h"

