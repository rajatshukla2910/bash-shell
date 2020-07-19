#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<readline/readline.h> 
#include<readline/history.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<dirent.h>

#define ANSI_COLOR_BLUE "\x1b[1;32m" //for terminal prompt color
#define ANSI_COLOR_RESET "\x1b[0m"//for resetting terminql color
#define clear() printf("\033[H\033[J") //define for clearing terminal window

char prompy[1000];
/******************************************** Shell prompt initialization *************************************************************/

void init_shell(void)
{ 
   
	char* get_user = getenv("USER"); //variable to get username 
	char prompt[100];
	strcpy(prompt,get_user);    
	struct utsname detect;//to detect system name
	uname(&detect);
	//for getting current working directory	
	char cwd[1024]; 
	getcwd(cwd, sizeof(cwd));
        char temp[1024] = "/home/";
        strcat(temp,get_user);
	
	if(strcmp(cwd,temp)==0) 
	{
		strcat(prompt,"@");
    		strcat(prompt,detect.nodename);
  	 	strcat(prompt,":~$");
	}
        else
	{
		strcat(prompt,"@");
		strcat(prompt,detect.nodename);//getting system name
		strcat(prompt,":~");
		strcat(prompt,cwd);//getting current working directory if it is not home directory
		strcat(prompt,"$");
	}
	
     
   
	strcpy(prompy,ANSI_COLOR_BLUE);
	strcat(prompy,prompt);//setting prompt value
	strcat(prompy,ANSI_COLOR_RESET);
  
} 
/**************************************************************************************************************************************/



/*************************************************** Function for taking Input ********************************************************/
int Input(char* inp) 
{ 
	char* buffer; //buffer to store entered command by user
	init_shell();//get prompt string
     
	buffer = readline(prompy); //reading input from user
	if (strlen(buffer) != 0)//if buffer is not empty 
	{ 
	        add_history(buffer); //add the command to history so that it can be get by using arrow keys in future commands
	        strcpy(inp, buffer); //copying the input to inp variable
	        return 0; 
	}     
	else 
	{ 
	        return 1; 
	} 
} 

/*************************************************************************************************************************************/

/*************************************************** Function for Parsing Input ********************************************************/
void split(char* inp, char** args) 
{ 
  
	for (int i = 0; i < 10; i++) //assuming we can have maximum of 10 arguments
	{ 
        	args[i] = strsep(&inp, " "); //string separator with blank space as separator
        
        	if (args[i] == NULL) //if string is terminmated then break loop
            		break; 
        	if (strlen(args[i]) == 0) 
            		i--; 
	} 
} 

/*************************************************************************************************************************************/

/*********************************************************** Command Execution Function  **************************************************/
int execute(char** args) 
{ 
	if(strcmp(args[0],"exit")==0)
	{
		printf("GOOD BYE...\n");
		return 0;
	}
	if(strcmp(args[0],"cd")==0)    
	{
         
         	chdir(args[1]); 
		return 1; 

	}
	char temp[100]="/home/";
	
	char* get_user = getenv("USER");
	strcat(temp,get_user);
	strcat(temp,"/name_shelll/");
	strcat(temp,args[0]);
	pid_t pid = fork();//creating a child process by using fork()  
  	
	if (pid == 0) //child proces created
	{	
        	if (execvp(temp, args) < 0)//checking whether command can be executed or not
		{	
            		printf("Failed to execute command..\n"); 
        	} 
        	exit(0); 
	} 
	else if (pid == -1) //failed to create child process
	{	 
	       	printf("Child Process can't be created\n")	; 
        	return -1; 
        
    	}
	else 
	{ 
        
	        wait(NULL);//waiting unless child process will not terminate  
	        return 1; 
	} 
} 
/*************************************************************************************************************************************/


/*********************************************************** REPL Loop*****************************************************************/
void repeat(void)//function that repeats till exit from shell
{

	char input[1000];//line entered by user
	char ** arguments;//arguments that are used in line 
	clear();
	int status;//status that is return by commands
	
	do{
	if (Input(input)) //taking input from user
            continue; 
	
	split(input,arguments);//parsing the command
	status=execute(arguments);//execution of command
	
        

	}while(status);//status is 0 when user enters exit

}
/****************************************************************************************************************************************/

/************************************************************ MAIN FUNCTION **************************************************************/

int main()
{

repeat();
return 0;

}
/*

*/

/****************************************************************************************************************************************/
