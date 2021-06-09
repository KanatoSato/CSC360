Kanato Sato
V00948631

Guideline

sh360.c


1. Before using sh360.c, please make sure you have .sh360rc file in the same directory as sh360.c
   In the .sh360rc file, put any directory names (ie, /bin/, /sbin/, etc), otherwise the program will
   return an error message.

2. Compile sh360.c 

3. Type command such as:
    Ordinary command (ie, ls -l, df, ps aux, etc)
    Output to a file (ie, OR "command" -> "file name")
    Pipe commands (ie, PP "command1" -> "command2", PP "command1" -> "command2" -> "command3")

a. By extracting the first line of .sh360rc file using fgets() function, the program keeps showing prompt message until "exit" is entered.

b. By counting the number of arguments (except OR, PP, ->, and other invalid characters), if the number of arguments is greater than 7,
   the program returns error message. And other error handling.

c. By finding out the position of OR and -> using strcmp(), the program can extract command and file name, and output the result to the provided file.
   Then I followed the appendix program for file output.
   For file output, I used execve(), fork(), dup2().


d. By finding out the position of PP and -> using strcmp(), the program extracts the multiple commands to be piped together.
   Also counting the number of -> determines using either single or double pipes.
   Then I followed the appendix program for piping and the resources on the internet.
   For piping, I used the pipe(), execve(), fork(), dup2().

Refferences:
    https://gist.github.com/mplewis/5279108
    Dr. Zastre's 5 appendixes
