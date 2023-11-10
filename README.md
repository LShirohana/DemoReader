# DemoReader
This reads a demo's header and parses it. It also supports larger than 2 GB files of reading the header.

# Why
Because TF2 is a 32 bit program and cannot actually parse a file that is larger than what can fit in an integer of 4 bytes. I originally did not know this, and created this program (by re-implementing exactly how TF2 reads the file) so I could debug it locally and determine where and why it is failing.

# Result of this mini project
I figured out that TF2, in multiple places, cannot actually open this. The UltBuffer and UltVector classes the source engine use are littered with 32 bit ints for reading and writing files, and would need a major update to actually support the reading of the file. Thus, the demo that I was debugging is actually fine, it is just too big.

# Notes of this code
This code should work out-of-the-box except for a single thing, and that is that I hard-coded the demo file path it reads from. This can be chanegd in CDemoFile.cpp (sorry!)

# How to use
Replace the path to the demo you want to read, and then put a breakpoint on `ReadDemoHeader` in CDemoFile.cpp on line #170. It should properly show you the demo. If you want, just add some lines to send output to std out so you can just read it.
