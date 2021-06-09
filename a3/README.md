# CSC360 Assignment3: Round Robin CPU Scheduler Simulation by Kanato Sato (V00948631)
## Task1: Implementaion of Round Robin CPU-Scheduler Simulator in C language
Task1 is a Round Robin CPU-Scheduler simulator.
To test out my simulator, you need to have my *rrsim.c*,*makefile*,*linkedlist.c*,*linkedlist.h*,*simgen.c*
I used linux computer for the test-run (procedures bellow may not work on other OS)
1. On terminal, run *make* to create all the excutables.
2. You need to change couple lines in my rrsim.c file to see (print) actual result.
   Scroll all the way down rrsim.c and comment out *run_simulation_result()* and *printf()*, undo comment on *run_simulation()*.
   (I need those *run_simulation_result()* and *printf()* to do Task2.)
   Save the file and run *./rrsim --quantum <num> --dispatch <num>*, and input data as follows:

$ ./rrsim --quantum 3 --dispatch 
$ 1 5 3.5
$ 2 7 4.9
$ 3 14 1.1
$ Press Ctrl-d

 By default, if program ran as above, it shows quantum, dispatch, average waiting, average turnaround like:
   $ quantum: 3, dispatch: 2, avg_wait: 10.17, avg_ta: 13.33

3. Or instead of running manually, you can run like:
    $ ./simgen 1000 <seed> | ./rrsim --quantum <num> --dispatch <num>

    which generates 1000 random tasks and runs simulator using those tasks as input.

## Task2: Analyzing output from the Task1 simulator
Task2 generates two graphs to visualyze the differences if quantum and dispatch vary for scheduler.
To analyze Task1 output, make sure the rrsim.c runs *run_simulation_result()* and *printf()* and should not run *run_simulation()*.
Also, you need my *analyze_result.py* which collects tasks from 20 random seeds and runs rrsim.c with quantum lengths: 50, 100, 250, 500
and dispatch length: 0, 5, 10, 15, 20, 25 for each random seed. Then generates *dispatch VS average waiting time graph* and *dispatch VS average turnaround time* as PDF. You can change quantum and dispatch length by modifying my code. 

Before running python file, please install matplotlib by typing: 
$ sudo spt install python3-pip then $ pip3 install matplotlib 
Also, please make sure you have compiled all the c files you need for the simulation.
1. All you have to do is run $ python analyze_result.py and wait for seconds. (Please make sure *rrsim.c* runs *run_simulation_result()* 
   and *printf()* and not *run_simulation()* in the rrsim.c file. After modifying C file, please run $ make on terminal.)
   When you run this python file, it creates (overwrites) result.txt, extracts data line by line from the file, store avrage waiting and turnaround in a ductionary, and finally uses the dictionary to draw two graphs.
   
2. Check two PDF files to see the graphs.

## References
Dr. Michael Zastre: Uvic csc360 assignment3 sample codes
