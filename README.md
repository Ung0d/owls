# owls
An implementation of signals and requests in modern C++

todo: documentation :)

For now:

A Signal is an object, that can have callbacks connected to it. If the Signal is invoked, 
arguments are forwarded to all registered callbacks. The use of signals can reduce code dependency. You
can just fire out an information about something interesting that happened and dont have to care about who actually recieves it.

A Request works like a signal with the addition that the callbacks have return values. This is useful if a part of code requires informations from other parts without introducing a direct dependency.

Library is header only and single file.

Look up SignalTest.cpp for basic usage.

