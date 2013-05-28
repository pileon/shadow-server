Server design
=============

The purpose of this text is to document the design of the Shadow MUD server
application.

Overview
--------

Logging
=======
Logging is essentially split into two components:

1. Back-end
2. Channel

Back-end
---------
The back-end is the component handling the actual writing of the log to the
underlying system.

From the start there will be three standard back-ends:

1. Standard C++ output stream
2. POSIX system log
3. Windows system log

A back-end contains functionality to write strings to the system, and not much
more.

Besides the three standard back-ends mentioned above, there will also be a
fourth, though it will not be used directly. Instead is will be a buffered back-
end that will act as parent class to those back-ends that doesn't have
underlying systems already containing buffers.

Channel
-------

The channel component is the actual stream that is used to write to from the
program. A channel can have multiple back-ends. Channel objects are created
dynamically when something needs to be logged.

The channel classes will be based on `boost::iostreams::stream`, and so will be
usable as normal C++ stream objects like `std::cout`.

Network
=======



Library
=======


Database
========
