# myshell
This is a shell written in C that will allow the user to execute given files and commands using UNIX system calls.

## How to Use
- First, you will need GCC installed to compile the program.
- Onced cloned, simply navigate your way to the myshell.c file with cd and GCC compile it.
- When running the program, a myshell> prompt will be visible.
- To run a file and its commands, use the 'run' operation. IE: 'run ls -l'
- To exit the shell, use the 'exit' operation by itself.

## How it Works
- When provided with the proper run operation and an existing file and commands, the program will fork a new process, creating a child process, and run the given file and commands there.
Once the file is done running, the child process returns out.

