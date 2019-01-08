/*
Author: Elizabeth Ralyea
Purpose: create a simple shell program
date: 1/29/18
*/

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <csignal>

using namespace std;

extern char **environ;
vector<string>  command_history;

int doit(vector<string> commands){
	if(!commands.size()|| commands[0] == "")
		return 0;
	else if(commands[0] == "chgd"){ //change directory
		if(commands.size() > 1)
			chdir(commands[1].c_str());
		else{ chdir(getenv("HOME"));
			cout << " NO Directory Specified";}
		return 0;}
	else if(commands[0] == "myprocess"){ // display process
		cout << getpid() << endl;
	}
	else if(commands[0] == "allprocesses"){// display all processes
		string comm = "ps";
		const char *com = comm.c_str();			
		system(com);
	}
	else if(commands[0] == "clr"){// clear screen
  		string comm2 = "clear";
		const char *com2 = comm2.c_str();
		system(com2);
	}
	else if(commands[0] == "dir"){// list what is in a file
		if(commands.size() > 1){
			string comm3 = "ls -al " + commands[1];
		const char *com3 = comm3.c_str();
		system(com3);}
		else cout << " NO Directory Specified";
		return 0;
	}
	else if(commands[0] == "environ"){
		char ** env=environ;
		while (*env)
			cout << (*env++) << endl;
	}
	else if(commands[0] == "help"){ //help menu
		cout << "ralyeaShell accepts the following commands: " << endl
		     << "clr: 				clears screen"<< endl
		     << "repeat <text>: 		writes your text to the screen"<< endl
		     << "repeat <text> > <filename>: 	writes your text to the specified file"<< endl
		     << "hiMom: 			creates a child and parent pipeline to deliver a message"<< endl
		     << "dir <directory>:		lists all entries in a directory"<< endl
		     << "allprocesses: 			lists all processes"<< endl
		     << "myprocess: 			gives the current process"<< endl
		     << "chgd <directory>: 		changes directory to that specified"<< endl
		     << "help: 				lists all possible commands"<< endl;
		
	}
	else if(commands[0] == "repeat"){
		string repeat_string;
		int i = 0;
		if(commands.size() > 1)
			{
		for(int i=1; i<commands.size(); ++i)
			{if(commands[i] == ">")break;
			repeat_string = repeat_string + commands[i];
				}
		if(commands[i] == ">"){
			int file_desc = open(commands[3].c_str(), O_WRONLY | O_CREAT, 0644);
			if(file_desc < 0)
				cout << "Error creating file" << endl;
			dup2(file_desc, 1);
			const char *repeat = repeat_string.c_str();
			printf("%s",repeat);}
		else cout << repeat_string << endl;
	}else{ chdir(getenv("HOME"));}}
	else if(commands[0] == "hiMom"){
		int fd[2], nbytes;
		pid_t childpid;
		char    string[] = "Hello, Mom!\n";
		char    readbuffer[80];
		
		pipe(fd);
		if((childpid = fork())== -1)
		{
			perror("fork");
			exit(1);
		}
		if(childpid == 0)
                {
                        /* Child process closes up input side of pipe */
                        close(fd[0]);
			write(fd[1], string, (strlen(string)+1));
              	        exit(0);
                }
                else
                {
                        /* Parent process closes up output side of pipe */
                        close(fd[1]);
			nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
                	printf("Received string: %s", readbuffer);
                }
	
	}
	else{cout << "That is not a command" << endl;}


}

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n Now Terminiating \n";

   // cleanup and close up stuff here  
   // terminate program  
for(int i=0; i<command_history.size(); ++i)
  		cout << command_history[i] << endl;
  exit(1);  
}

int main(){


	char command[124];
	bool signal_bool;

	signal(SIGINT, signalHandler);

while(!cin.eof()){ //end  program with
	cout << "ralyeaShell >> ";

	string command;
	getline(cin, command);
	command_history.push_back(command);
	 
	if(command == "quit")
		break;
	

	vector<string> command_strings;
	stringstream foo(command);
	string s;

	while(foo >> s){
	if(s[0]=='~')
	 s = getenv("HOME") + s.substr(1);
	command_strings.push_back(s);
	}
	doit(command_strings);
}

	for(int i=0; i<command_history.size(); ++i)
  		cout << command_history[i] << endl;
	return 0;

}


