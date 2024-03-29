= stacktrace

* https://github.com/SamSaffron/stacktrace

== DESCRIPTION:

The idea here is to have a more efficient and useful stacktrace than caller. Caller does not provide you with any class info. 

== FEATURES/PROBLEMS:

I think it only works on 1.9.3 for now, need to test

== SYNOPSIS:

Thread.current.stacktrace 
stacktrace 

stacktrace accepts 3 params: first is start, second is finish, last is flags

so: 

stacktrace(0,0) returns only 1 frame (the frame you are in) 
stacktrace(-1,-1) returns the earliest frame 
stacktrace(0,-1,StackFrame::Flags::METHOD | StackFrame::Flags::KLASS) will return only class and method info

== REQUIREMENTS:

* Ruby 1.9.3

== DEVELOPERS:

Sam Saffron

After checking out the source, run:

  $ rake newb

This task will install any missing dependencies, run the tests/specs,
and generate the RDoc.

== LICENSE:

(The MIT License)

Copyright (c) 2012 Sam Saffron

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
