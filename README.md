# Shell_Simulator
Linux shell simular that was implemented using some C functions like fork(), execlp(), and waitpid()

## Purpose
Learning process management in C/C++.
## Running instruction
1.clone the repository \
2.type `Make` or `Make shell` to produce the executable file \
3. run it with `./shell` and you can type the commmands below

## Commands List
1. `cdir <pathname>`: change current directory to pathname. can Be absolute or relative path. environmental variable works as well \
    example: `cdir $HOME/document` \
2. `pdir`: prints the path of the current directory
3. `lstasks`: list all the processes that were executed by `run` command
4. `run <programName> arg1 .. arg4`: runs <programName> with their argument. Implemented using fork() and execlp(). <programName> becomes a task\
    example: `run xclock -geometry 200x200`
5. `stop taskNumber`: stops the task using SIGSTOP
6. `continue taskNumber`: conitune the tasks that were stopped
7. `terminate taskNumber`: terminate the task
8. `check <PID>`: list out the descendent process of process with this PID
9. `exit`: terminate the program. this will free up all the processes that were ran
10. `quit`: termiane without freeing the process. They will become a zombie process.
