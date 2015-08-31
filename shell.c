#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <fcntl.h>

#define FALSE 0
#define TRUE 1
#define INPUT_STRING_SIZE 80
#define SOMES_SIZE 80

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"


int cmd_quit(tok_t arg[]) {
  printf("Bye\n");
  exit(0);
  return 1;
}

int cmd_help(tok_t arg[]);
int cmd_cd(tok_t arg[]);


/* Command Lookup table */
typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_quit, "quit", "quit the command shell"},
  {cmd_cd, "cd", "change the directory"},
};

int cmd_help(tok_t arg[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}

int cmd_cd(tok_t arg[]){
	chdir(arg[0]);

	//char cwd[2018];
	//getcwd(cwd, sizeof(cwd));
    //fprintf(stdout, "%s",cwd);

	return 1;
}


int lookup(char cmd[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
  }
  return -1;
}

void init_shell()
{
  /* Check if we are running interactively */
  shell_terminal = STDIN_FILENO;

  /** Note that we cannot take control of the terminal if the shell
      is not interactive */
  shell_is_interactive = isatty(shell_terminal);

  if(shell_is_interactive){

    /* force into foreground */
    while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
      kill( - shell_pgid, SIGTTIN);

    shell_pgid = getpid();
    /* Put shell in its own process group */
    if(setpgid(shell_pgid, shell_pgid) < 0){
      perror("Couldn't put the shell in its own process group");
      exit(1);
    }

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
  /** YOUR CODE HERE */
}

/**
 * Add a process to our process list
 */
void add_process(process* p)
{
  /** YOUR CODE HERE */
}

/**
 * Creates a process given the inputString from stdin
 */
process* create_process(char* inputString)
{
  /** YOUR CODE HERE */
  return NULL;
}



int shell (int argc, char *argv[]) {
  char *s = malloc(INPUT_STRING_SIZE+1);			/* user input string */
  tok_t *t;			/* tokens parsed from input */
  int lineNum = 0;
  int fundex = -1;
  pid_t pid = getpid();		/* get current processes PID */
  pid_t ppid = getppid();	/* get parents PID */
  pid_t cpid, tcpid, cpgid;
  char cwd[2018];

  //printf("\n%s%s%d%s%d\n",argv[0]," running as PID ", pid," under ", ppid);
  printf("%s running as PID %d under %d\n",argv[0],pid,ppid);

  
  //getcwd(cwd, sizeof(cwd));
  //fprintf(stdout, "%s\n%d:",cwd,lineNum);
  

  init_shell();

  

  lineNum=0;
  
  getcwd(cwd, sizeof(cwd));
  fprintf(stdout, "%s\n%d:",cwd,lineNum);
  while ((s = freadln(stdin))) {

    t = getToks(s); /* break the line into tokens */
    fundex = lookup(t[0]); /* Is first token a shell literal */
    if(fundex >= 0) {

    	cmd_table[fundex].fun(&t[1]);
    }
    else {
    	int flag =0;
    	int meh =-1;
    	cpid = fork();

    	pid_t mypid;
    	struct process current;
        
        if( cpid > 0 ) 
        { // parent process
          mypid = getpid();
          //printf( "[%d] parent of [%d]\n", mypid, cpid );
          int status;
          pid_t tcpid = wait(&status);
          current.status = status;

          

         // current -> completed = "name";

          if(flag == 0  || meh ==0)
          {
          	printf( "%s\n","Parent Process of fork - Done woth child");
          	current.completed = "true";
          }
          
          
        }

      if( cpid == 0 ){ // child process
     
      	int r=0;
      	int j=0;
      	int countWord=0;
      	int indexOf =0;

      	int fw;

      	int determine = 0;

       while (t[r] != NULL)
       {

       	if(strcmp(">", t[r]) == 0)
          {
          	determine = 1;
          	indexOf = r;
          }

          if(strcmp("<",t[r])==0)
          {
          	indexOf = r;
          	determine =2;

          }
       	  
          countWord++;
          r++;
          
       }

       char *toks [countWord];
       char *inputArr[SOMES_SIZE];
       char buffer[100];

       int increm =0;

        int p=0;

        pid_t newpid;

       	if(determine ==1)
       	{
       		increm = indexOf -1;
       		fw=open(t[indexOf+1], O_CREAT|O_WRONLY, S_IRWXU);
            dup2(fw,1);
       	    
        }

        else if(determine == 2)
        {

        	//increm = countWord;
        	int i=0;
        	

        	FILE *op=fopen(t[indexOf+1], "r");

        	if (op==-1)
        	{
        		printf("%s\n", "FAiled to Load input File");
        	}

        	int r=0;

        	while ( fscanf(op, "%s", buffer)==1 )
			{
				

				
				inputArr[r] = strdup(buffer);

				r++;
			}
				
        	
        } 


       else{
       	increm = countWord;
       }


       	for(j; j<= increm;j++)
       	{
       		toks[j] = t[j];
       	}

        char *path =  getenv ("PATH");
      
      
      mypid = getpid();



      if(determine == 2)
      {
      	flag = execvp(t[0], inputArr);
      	current.pid = mypid;
      }

      else{
      	 flag = execv(t[0], toks);
      	 current.pid = mypid;
      }
     


      //printf("%d\n", flag);



       char *pathName;


      char *token=  strtok(path, ":");
     // int meh=-1;

          if(flag==-1)
          {
          	while(token !=NULL)
          	{
          		//pathName=token;
				pathName = malloc(strlen(t[0])+ strlen(token+50)); /* make space for the new string (should check the return value ...) */
				strcpy(pathName, token); 
				//strcpy(pathName, "/");/* copy name into the new var */
				strcat(pathName,"/");
				strcat(pathName, t[0]);

				

          		//printf("%s\n", pathName);

          		if(determine == 2)
                {
                	//printf("%s\n", "Were here at inputArr");
                	meh = execv(t[0], inputArr);
                	//current->pid = mypid;
                	current.pid = mypid;
                }

                else{
 
          		meh = execv(pathName, toks);
          		token = strtok(NULL,":");
          		//current->pid = mypid;
          		current.pid = mypid;

          	}

          	}
          	
          	if(meh ==-1){
          	  fprintf(stdout, "This shell only supports built-ins & commands.\n");
          	}
          }
      }



      //fprintf(stdout, "This shell only supports built-ins. Replace this to run programs as commands.\n");
    }
    getcwd(cwd, sizeof(cwd));
    lineNum++;
    fprintf(stdout, "%s\n%d:",cwd,lineNum);
  }

  return 0;
}
