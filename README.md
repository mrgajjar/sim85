# sim85
User Manual For 8085 Simulator
------------------------------
This is a humble try to present a 8085 microprocessor simulator.
It lets you simulate programs of 8085 on 80x86 based microprocessors. It
can be very useful tool for anyone who wants to do 8085 assembly programming
without having actual hardware. Here are some key features

-You can directly write your programs in mnemonics form in file. So opcodes
 and hand coding part becomes transperant.
-Excellent delay simulation with user specified clock frequencies for 8085.
 You can get total T-states since start of execution. Very useful for writing
 delay loops for actual programs, calculting delay for different speed CPUs.
-Single steping through program.
-You can insert break points in the program.
             
--------------------------------
ABOUT 8085 SIMULATOR COMMANDS :-
--------------------------------
When simulator is started it shows "-" prompt in lower left corner of the
screen. Although commands are always listed on screen, here is more
detailed description.

1. "L" COMMAND : (load) Used to load an assembly program file for 8085.
                  This file must have an extension ".85".
2. "E" COMMAND : (execute) Executes (simulates) loaded assmebly file.
3. "M" COMMAND : It is primarily used for viewing memory contents after
                 simulation.
4. "S" COMMAND : (store) Used for storing memory locations before
                 simulation.
5. "T" COMMAND : (trace) Used for single stepping through program. After each
                 instruction executed program stops and displays registers,
                 contents of memory location pointed to by HL pair register,
                 etc.
6. "F" COMMAND : Used for setting the clock frequency of the 8085 CPU before
                 simulation.
----------------------------------------------------------
ABOUT SYNTEX OF THE ASSEMBLY PROGRAM FOR 8085 SIMULATOR :-
----------------------------------------------------------

1. Every program must begin with ".BEGIN" directive.
   Every program should end with ".END" directive.
   All lines before ".BEGIN" are ignored.
   All lines after ".END" are ignored.
2. Only one instruction is allowed per line.
3. Only one label is allowed per instruction.
   Label must be in the same line as the instuction.
   Each label must end with ":" and second ":"in the same line is not allowed.
4. Line is ignored after ";".
5. There can be any number of spaces between instruction operands but
   spaces between mnemonic is not allowed.
   (i.e. "MOV       A           B" is same as "MOV A B")
6. ".BREAK" directive can be used for inserting break point in simulation.
   It must be written on new line and no instruction allowed on the same
   line.
7. All mnemonics must be written in capital latters.
8. Maximum label length=16 characters.

--------------
LIMITATIONS :-
--------------
1. Following instructions of 8085 can not be used in program.
   DAA,EI,DI,RIM,SIM,RST 0 TO RST 7
2. Auxillary Carry (ac) and Parity flags are not available.
3. 4k bytes data memory.
4. Maximum 1000 instructions per program.

-------
NOTE :-
-------

Some assembly programs are provided with simulator for getting idea about
syntex.
