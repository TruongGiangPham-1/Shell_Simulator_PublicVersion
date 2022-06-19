/**
 * Simulate Linux Shell by servicing request entered by the user 
 * Author: Truong-Giang Pham
 * 
 * */



#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <unistd.h>
#include <sys/times.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <cstring>

using namespace std;

struct task_struct {
   int task_pid;
   int head_pid;
   int index;
   //const char * cmd2;
   string cmd2;
   bool isRunning;
};

string getFirstArg(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] != ' ') {
            result += s[i];
        } else  {
            goto return_here;
        }
    }
    return_here:
    return result;
}



string getPathName(string s) {
    string result = "";
    int nthSpace = 0;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ' ' and nthSpace == 0) {
            nthSpace = 1;
        } else if (nthSpace == 1) {
            if (s[i] != '$') {
                result += s[i];
            } else {  // $ENVIRONMENT variable
            
            }
        } 
        
    }
    // replace the all the environment variable
    return result;
}

/* ----------------------------------------------
SplitString: given a string, split them into tokens 
             and append to vector
Arguments: 
-s: input string
-v: vector of tokens
Return: 
- void
-----------------------------------------------*/ 
void SplitString(string s, vector<string> &v){
	
	string temp = "";
	for(int i=0;i<s.length();++i){
		
		if(s[i]==' '){
			v.push_back(temp);
			temp = "";
		}
		else{
			temp.push_back(s[i]);
		}
		
	}
	v.push_back(temp);
	
}
/* ----------------------------------------------
runTask: execute the "run pgg arg1, ... " command
            
Arguments: 
-tokens: vector of tokens
-taskArray: NTASK array
-index: current number of pocesses
-cmd: the command for execlp
Return: 
- void
-----------------------------------------------*/ 
void runTask(vector<string>&tokens, struct task_struct* taskArray, int &index, string cmd) {
    // create populate task_list struct array
    int n = tokens.size();
    if (n == 6) {  // there are 4 argument
        int pid = fork();
        if (pid == 0) { // child process
            const char* p = tokens[1].c_str(); //programName
            const char * a1 = tokens[2].c_str();
            const char * a2 = tokens[3].c_str();
            const char * a3 = tokens[4].c_str();
            const char * a4 = tokens[5].c_str();
            if (execlp(p, p, a1, a2, a3, a4, (char*)NULL) == -1) {
                cout << "execlp failed" << endl;
                _Exit(EXIT_FAILURE);
            }
        } else if (pid == -1){          // fork error
            cout << "FORK ERROR" << endl;
        } else {                        // HEAD PROCESS
            //sleep(1);
            // we wait to ensure the chlid is before us
            usleep(100000);  // 100000 microsec = 0.1 seconds
            if (waitpid(pid, NULL, WNOHANG) == pid) {
                cout << "child terminated " << pid <<endl;
                return;
            }
            const char * cmd1 = cmd.c_str();
            //struct task_struct T = {pid, getpid(), index, cmd1};
            taskArray[index].task_pid = pid;
            taskArray[index].head_pid = getpid();
            taskArray[index].index = index;
            taskArray[index].cmd2 = cmd;
            taskArray[index].isRunning = true;
            index ++;
            
        }
        return;
    } else if (n == 5) {  // there are 3 arg
        int pid = fork();
        if (pid == 0) { // child process
            const char* p = tokens[1].c_str(); //programName
            const char * a1 = tokens[2].c_str();
            const char * a2 = tokens[3].c_str();
            const char * a3 = tokens[4].c_str();
            if (execlp(p, p, a1, a2, a3, (char*)NULL) == -1) {
                cout << "execlp failed" << endl;
                _Exit(EXIT_FAILURE);
            }


        } else if (pid == -1) {
            cout << "fork failed" << endl;
        } else {
            //sleep(1);
            // we wait to ensure the chlid is before us
            usleep(100000);  // 100000 microsec = 0.1 seconds
            if (waitpid(pid, NULL, WNOHANG) == pid) {
                cout << "child terminated " << pid <<endl;
                return;
            }
            const char * cmd1 = cmd.c_str();
            taskArray[index].task_pid = pid;
            taskArray[index].head_pid = getpid();
            taskArray[index].index = index;
            taskArray[index].cmd2 = cmd;
            taskArray[index].isRunning = true;
            index ++;
        }
        return;

    } else if (n == 4) {  // there are 2 arg
        int pid = fork();
        if (pid == 0) { // child process
            const char* p = tokens[1].c_str(); //programName
            const char * a1 = tokens[2].c_str();
            const char * a2 = tokens[3].c_str();
            if (execlp(p, p, a1, a2, (char*)NULL) == -1) {
                cout << "execlp failed" << endl;
                _Exit(EXIT_FAILURE);
            } 
           
        } else if (pid == -1) {
            cout << "fork error" << endl;
        } else  {
            usleep(100000);  // 100000 microsec = 0.1 seconds
            if (waitpid(pid, NULL, WNOHANG) == pid) {
                cout << "child terminated " << pid <<endl;
                return;
            }
            const char * cmd1 = cmd.c_str();
            //struct task_struct T = {pid, getpid(), index, cmd1};
            taskArray[index].task_pid = pid;
            taskArray[index].head_pid = getpid();
            taskArray[index].index = index;
            taskArray[index].cmd2 = cmd;
            taskArray[index].isRunning = true;
            index ++;
        }
        return;
        
    } else if (n == 3) {  // there are one argument
        int pid = fork();
        if (pid == 0) {            
            string programName = tokens[1];
            const char* p = programName.c_str();
            string arg1 = tokens[2];
            const char * a1 = arg1.c_str();
            if (execlp(p, p, a1, (char*) NULL) == -1 ) {
                cout << "failed execlp" << endl;
                _Exit(EXIT_FAILURE);
            }

        } else if (pid == -1 ) {
            cout << "fork error" << endl;
        } else {
            usleep(100000); 
            if (waitpid(pid, NULL, WNOHANG) == pid) {
                cout << "child with pid " << pid << " terminated"<<endl;
                return;
            }
            const char * cmd1 = cmd.c_str();
            //struct task_struct T = {pid, getpid(), index, cmd1};
            taskArray[index].task_pid = pid;
            taskArray[index].head_pid = getpid();
            taskArray[index].index = index;
            taskArray[index].cmd2 = cmd;
            taskArray[index].isRunning = true;
            //taskArray[index] = T;
            index ++;        
            return;
        }
        
        
    } else if (n == 2) {    // no argument
        int pid = fork();
        if (pid == 0) {
            //sleep(1);
            const char* p = tokens[1].c_str();
            if (execlp(p, p, (char*)NULL) == -1) {
                cout << "failed execlp" << endl;
                _Exit(EXIT_FAILURE);
            }


        } else if (pid == -1) {
            cout << "Fork error" << endl;
        } else {
            //sleep(1);
            // we wait to ensure the chlid is before us
            usleep(100000);  // 100000 microsec = 0.1 seconds
            if (waitpid(pid, NULL, WNOHANG) == pid) {
                cout << "child terminated " << pid <<endl;
                return;
            }
            const char * cmd1 = cmd.c_str();
            //struct task_struct T = {pid, getpid(), index, cmd1};
            taskArray[index].task_pid = pid;
            taskArray[index].head_pid = getpid();
            taskArray[index].index = index;
            taskArray[index].cmd2 = cmd;
            // EDIT
            taskArray[index].isRunning = true;
            //taskArray[index] = T;
            index ++;
        }

    }
}


void lstasks(struct task_struct * taskArray, int & index) {
    for (int i = 0; i < index; i++) {
        printf("%d: (pid= %d, cmd=%s)\n", taskArray[i].index, taskArray[i].task_pid, taskArray[i].cmd2.c_str());
    }
}

/* ----------------------------------------------
mysbstr: given a string s, return spliced s[index: ]
Arguments: 
-s: input string
-index: index to splice
Return: 
- s[index: ]
-----------------------------------------------*/ 
string mysubstr(int index, string s) {
    string result = "";
    // run s
    for (int i = index; i < s.length(); i++) {
        result += s[i];
    }
    return result;
}
/* ----------------------------------------------
decodeEnvironmentPath: decode entered path
             
Arguments: 
-v: empty vector
-path: path string
Return: 
- a path where all environment var is expanded
-----------------------------------------------*/ 
string decodeEnvirontmentPath(vector<string>&v, string path) {
    // split by /
    v.clear();
    string temp = ""; 
    for (int i = 0; i < path.length(); i++) {
        if (path[i] == '/') {
            v.push_back(temp);
            temp = "";
        } else {
            temp += path[i];
        }
    }
    v.push_back(temp);
    int environmentValIndex[v.size()] = {0}; // keep track of path token that is environmentvar
    // it replaces all environmental variable with full path
    for (int i = 0; i < v.size(); i++) {
        if (v[i][0] == '$') {
            string thistoken = v[i].substr(1);
            const char * temp = getenv(thistoken.c_str());
            if (temp == NULL) {
                return "INVALID ENV";
            }
            string realpath = getenv(thistoken.c_str());
            v[i] = realpath;
            environmentValIndex[i] = 1;  // indicate that this token was an env var
            temp = NULL;  // prevent dangling pointers
        }
    }
    string fullpath = "";
    for (int i = 0; i < v.size(); i++) {
        if (environmentValIndex[i] == 1) {
            fullpath += v[i];
        } else {
            fullpath += ("/" + v[i]);
        }
        
    }
    return fullpath;
}
/* ----------------------------------------------
isInVect: check if string s is in the vector v
Arguments: 
-s: string to check
-v: vector of string
Return: 
- return 1 if s is in v, else return 0
-----------------------------------------------*/ 
int isInVect(vector<string>&v, string s) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == s) {
            return 1;
        }
    }
    return 0;
}
/* ----------------------------------------------
check_pid_function: use bfs like algo to print descendants.
                    described in self report pdf. and handle
                    check command
Arguments: 
-buffer: buffer to store each line from popen 
-bufferCopied:
-pid_checking: the pid we want to check 
Return: 
- void
-----------------------------------------------*/ 
void check_pid_funciton(char* buffer, char* bufferCopied, string pid_checking) {
    string buffer_str = "";
    string command = "ps -u $USER -o user,pid,ppid,state,start,cmd --sort start";
    vector<string>tokens;
    vector<string> frontier;     // this vector to do bfs
    bool flag = false;           // false means we are not doing bfs; we still looking for line
    

    FILE * pipe = popen(command.c_str(), "r");
    if (!pipe) {
        cout << " popen failed" << endl;
    }

    fgets(buffer, 500, pipe);  // get 1st line
    string firstline = "USER         PID    PPID S  STARTED CMD";
    while (fgets(buffer, 500, pipe) != NULL) {
        //printf("%s", buffer);
        tokens.clear();
        buffer_str = "";
        for (int i = 0; i < 500; i++) {    // buffer -> string(not needed)
            if (buffer[i] == '\0') {
                break;
            }
            buffer_str += buffer[i];
        }
        // https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
        char * token = NULL, *theRest = NULL;  // avoid dangling pointer
        
        memset(bufferCopied, '\0', sizeof(bufferCopied));
        strcpy(bufferCopied, buffer);
        theRest = bufferCopied;  // need to copy or else strtok_r gunna modify buffer

        for (int i = 0; i < 3; i++) {  
            token = strtok_r(theRest, " ", &theRest);
            string tok_str = "";
            for (int j = 0; j < strlen(token); j++) {  // char array -> str
                tok_str += token[j];
            }
            tokens.push_back(tok_str);
        }
        // tokens[0] == truonggi, tokens[1] == PID, tokens[2] == PPID
        string pid = tokens[1];
        string ppid = tokens[2];

        if (pid == pid_checking and flag == false) {  // this line matches check pid
            if (buffer_str.find("<defunct>") != string::npos) {  //<defunc> found
                printf("target_pid=  %d terminated\n", stoi(tokens[1]));
                cout << firstline << endl;
                cout << buffer;
                goto finished_checkpid;


            } else  {  // this pid is running
                printf("target_pid= %s is running:\n", pid.c_str());
                printf("%s", buffer);
                // print all descendant
                // enqueue this pid  like dfs
                frontier.push_back(pid);   
                flag = true;            // set this flag to start bfs from now
            }
        }
        if (isInVect(frontier, ppid)) {  // if this process' parent is in frontier
            // then this pid is descendant of the check pid process
            // we enqueue this process
            printf("%s", buffer);
            frontier.push_back(pid);
        }
    }
    if (flag == false) {
        printf("target_pid dont exist");
    }
    printf("\n");
    finished_checkpid:
    pclose(pipe);
    // truonggi 2512164       1 S   Jan 22 /bin/sh ./myclock outfile
    return;
}
/* ----------------------------------------------
pr_times: AUPE 3/e function in section 8.17
Arguments: 
Return: 
- void
-----------------------------------------------*/ 
void pr_times(clock_t real, struct tms* tmsstart, struct tms* tmsend) {
    static long clktck = 0;
    if (clktck == 0) {
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0) {  // getting clock per sec
            cout << "error sysconf" << endl;
        }
    }
    printf("real:   %7.2f\n", real / (double)clktck);
    printf("user:   %7.2f\n", ((tmsend -> tms_utime) - (tmsstart -> tms_utime)) / (double)clktck);
    printf("child user: %7.2f\n", (tmsend->tms_cutime - tmsstart->tms_cutime) / (double)clktck);
    printf("child sys: %7.2f\n", (tmsend->tms_cstime - tmsstart->tms_cstime)/ double(clktck));
}

int main() {
    struct task_struct task_list[32];    // declare the struct of accepted tasks
    int parentID = getpid();             // get pid of the parent
    char buffer_getcwd[500];             // buffer to read from getcwd
    char buffer_popen[500];              // buffer to read from popen
    char buffer_popen_copy[500];         // duplicate buffer to dup buffer_popen
    int currentTaskIndex = 0;            // task index tracker
    vector<string>path_split;            // store decoded path tokens


    struct rlimit limistruct;
    limistruct.rlim_cur = 10;
    limistruct.rlim_max = 10;


    // im just following APUE figure 8.31 ------
    struct tms tmsstart; struct tms tmsend;
    clock_t start, end;
    int status;
    if ((start = times(&tmsstart)) == -1) {
        cout << "times error" << endl;
    }
    // ---------------------------------

    while (true) {
        vector<string>tokens;             // list of command line tokens
        int INDEX = 0;
        string s;                         // s == input string
        printf("msh379 [%d] ", parentID);
        getline(cin,s);
        SplitString(s, tokens);           // split s into tokens vector

        if (tokens[0] == "cdir") {
            string pathName = tokens[1];
            string realPath = decodeEnvirontmentPath(path_split, pathName);
            if (realPath == "INVALID ENV") {  // environet val didnt exist
                getcwd(buffer_getcwd, 500);
                printf("cdir: failed (pathname=  %s)\n", buffer_getcwd);
                continue;
            }
            if (chdir(realPath.c_str()) != 0) {  // cdir failed itself
                getcwd(buffer_getcwd, 500);
                printf("cdir: failed (pathname=  %s)\n", buffer_getcwd);
                continue;
            }

            getcwd(buffer_getcwd, 500);
            printf("cdir: done (pathname=  %s)\n", buffer_getcwd); 
           
        } else if (tokens[0] == "pdir") {
            getcwd(buffer_getcwd, 500);
            cout << buffer_getcwd << endl;

        } else if (tokens[0] == "run") {
            if (currentTaskIndex >= 32) {
                cout << "Maximum tasks execeeded. No longer accepting tasks" << endl;
                continue;
            }
            //string cmd_ = s.substr(4);          // cmd = command line string //THIS DID SOMETHING
            string cmd_ = mysubstr(4, s);
        
            runTask(tokens, task_list, currentTaskIndex, cmd_); 
            printf("\n");      // print new line becuase input minght get clogged by warnin msg of child   
           
        } else if (tokens[0] == "lstasks") {
            for (int i = 0; i < currentTaskIndex; i++) {
                if (task_list[i].isRunning == true)  // if this pid exist
                    printf("%d: (pid= %d, cmd=%s)\n", task_list[i].index, task_list[i].task_pid, task_list[i].cmd2.c_str());
            }
        } else if (tokens[0] == "stop") {
            // send kill(pid, sigSTOP)
            string index_toStop = tokens[1];
            int index_toStop_int = stoi(index_toStop);
            //printf("stop index: %d\n", index_toTerminate_int);
            int pid_stop = task_list[index_toStop_int].task_pid;
            kill(pid_stop, SIGSTOP);
            
        } else if (tokens[0] == "terminate") {
            string index_toTerminate = tokens[1];
            int index_toTerminate_int = stoi(index_toTerminate);
            int pid_terminate = task_list[index_toTerminate_int].task_pid;
            kill(pid_terminate, SIGKILL);
            // set the running status to false
            task_list[index_toTerminate_int].isRunning = false;
            printf("%d:  (pid= %d,  cmd= %s) terminated\n", index_toTerminate_int, pid_terminate, task_list[index_toTerminate_int].cmd2.c_str());
        } else if (tokens[0] == "check") {

            check_pid_funciton(buffer_popen, buffer_popen_copy, tokens[1]);

        } else if (tokens[0] == "continue") {
            // send kill(pid, SIGCONT)
            int index_toContinue = stoi(tokens[1]);
            int pid_cont = task_list[index_toContinue].task_pid;
            kill(pid_cont, SIGCONT);
        } else if (tokens[0] == "exit") {
            // terminate all processes and exit main loop
            for (int i = 0; i < currentTaskIndex; i++) {
                if (task_list[i].isRunning == true) {
                    kill(task_list[i].task_pid, SIGKILL);
                    printf("task %d terminated\n", task_list[i].task_pid);
                }
            }
            goto fininishC379;
        } else if (tokens[0] == "quit") {
            // terminate without killing head processes
            goto fininishC379;
        } else {
            printf("command: %s  is invalid\n", tokens[0].c_str());
        }
    }

    fininishC379:
    if ((end = times(&tmsend)) == -1 ) {
        cout << "end time error" << endl;
    }
    pr_times(end - start, &tmsstart, &tmsend);
    return 0;
}
// run xclock -geometry 200x200 -update 1
