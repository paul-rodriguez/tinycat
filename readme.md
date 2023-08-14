
# tinycat

This program is a small footprint replacement for netcat.

Tinycat only handles IP and UNIX sockets and only supports the -k and -l
options of netcat.
In other words, it can only be a simple client or a simple server.

Tinycat is completely self-contained.
It is statically linked with all the libraries it depends on.
Tinycat is meant to be used in restricted environments where the supporting
libraries of netcat aren't present.
