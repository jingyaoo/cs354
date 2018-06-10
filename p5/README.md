Learning Goals:

There are two main objectives of this project. The first is to quickly become familiar with x86 assembly language. The second relates to the first: to gain some familiarity with powerful tools that help with this process, namely gdb (the debugger) and objdump (the disassembler).

Binary Bombs:

In this assignment, you will be defusing four binary bombs. The idea is simple: each bomb is an executable program that prompts the user for five inputs via the stdin console, one at a time, in order to defuse the bomb. If you type in the right values, you successfully defuse the bomb. If not, the bomb explodes! (Don't worry, it just prints that the bomb explodes; no real harm is done to you or your computer)
