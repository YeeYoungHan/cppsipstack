copy /Y SipPlatform\*.h ..\..\MCU\trunk\cppsipstack\include
copy /Y SipParser\*.h ..\..\MCU\trunk\cppsipstack\include
copy /Y SdpParser\*.h ..\..\MCU\trunk\cppsipstack\include
copy /Y SipStack\*.h ..\..\MCU\trunk\cppsipstack\include
copy /Y SipUserAgent\*.h ..\..\MCU\trunk\cppsipstack\include
copy /Y ServerPlatform\*.h ..\..\MCU\trunk\cppsipstack\include
copy /Y XmlParser\*.h ..\..\MCU\trunk\cppsipstack\include

REM copy WIN32 debug

copy /Y Debug\SipPlatform.lib ..\..\MCU\trunk\cppsipstack\lib32\debug
copy /Y Debug\SipParser.lib ..\..\MCU\trunk\cppsipstack\lib32\debug
copy /Y Debug\SdpParser.lib ..\..\MCU\trunk\cppsipstack\lib32\debug
copy /Y Debug\SipStack.lib ..\..\MCU\trunk\cppsipstack\lib32\debug
copy /Y Debug\SipUserAgent.lib ..\..\MCU\trunk\cppsipstack\lib32\debug
copy /Y Debug\ServerPlatform.lib ..\..\MCU\trunk\cppsipstack\lib32\debug
copy /Y Debug\XmlParser.lib ..\..\MCU\trunk\cppsipstack\lib32\debug

REM copy WIN32 release

copy /Y Release\SipPlatform.lib ..\..\MCU\trunk\cppsipstack\lib32\release
copy /Y Release\SipParser.lib ..\..\MCU\trunk\cppsipstack\lib32\release
copy /Y Release\SdpParser.lib ..\..\MCU\trunk\cppsipstack\lib32\release
copy /Y Release\SipStack.lib ..\..\MCU\trunk\cppsipstack\lib32\release
copy /Y Release\SipUserAgent.lib ..\..\MCU\trunk\cppsipstack\lib32\release
copy /Y Release\ServerPlatform.lib ..\..\MCU\trunk\cppsipstack\lib32\release
copy /Y Release\XmlParser.lib ..\..\MCU\trunk\cppsipstack\lib32\release
