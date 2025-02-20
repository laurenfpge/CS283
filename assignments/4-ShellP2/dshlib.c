#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
char* trimspc(char *str){
	while(isspace(*str)){
		str++;
	}

	char *end = str + strlen(str)-1;
	while(end >= str && isspace(*end)){
		end--;
	}

	*(end+1) = '\0';
	return str;
}

int exactString(char* str, cmd_buff_t *cmd){
	int quotelen = 0;
	while(isspace(*str)){
		str++;
		quotelen++;
	}
	char *temp = malloc(sizeof(char)*strlen(str)+1);
	char *tempstart = temp;
	
	str++;
	quotelen++;

	while (*str != '"'){
		*temp = *str;
		temp++;
		str++;
		quotelen++;
	}
	*temp = '\0';

	temp = tempstart;

	cmd->argv[cmd->argc] = malloc(sizeof(char)*(strlen(tempstart)+1));
	strcpy(cmd->argv[cmd->argc], tempstart);
	
	return quotelen+2; //ignore quotes
}


int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(sizeof(char)*SH_CMD_MAX);
    int rc = 0;
    cmd_buff_t cmd;
   	
    while(1){
		cmd.argc = 0;
		
		printf("%s", SH_PROMPT);
 		if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 			printf("\n");
 		break;
 		}

 		//remove the trailing \n from cmd_buff
 		cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

 		if (strcmp(cmd_buff, EXIT_CMD) == 0){
			exit(rc);
 		}

		cmd_buff = trimspc(cmd_buff);
		cmd._cmd_buffer = malloc(sizeof(char)*(strlen(cmd_buff)));
    	strcpy(cmd._cmd_buffer, cmd_buff); //copy entire string

    	char *spcsave;
    	char *spctoken = strtok_r(cmd_buff, " ", &spcsave);
    
    	int index = 0;
    	while (spctoken != NULL){
			spctoken = trimspc(spctoken);
			if (cmd.argc > CMD_ARGV_MAX){
				printf("too many arguments\n");
				break;
			}

			if (*spctoken == '\0'){ //at the end of the string
				spctoken = strtok_r(NULL, " ", &spcsave);
				continue;	
			}

			if (spctoken[0] == '"'){
				int returnvalue = exactString(&cmd._cmd_buffer[index], &cmd);
				index += returnvalue;
				cmd.argc++;

				spctoken = strtok_r(&cmd_buff[index], " ", &spcsave);
				continue;
			}

			if (strlen(spctoken) > 0){
				cmd.argv[cmd.argc] = malloc(sizeof(char)*(strlen(spctoken)));
				strcpy(cmd.argv[cmd.argc], spctoken);
				cmd.argc++;
				index += strlen(spctoken)+1;

			}
			
			spctoken = strtok_r(NULL, " ", &spcsave);
    
		} //parse by space
		
		if (cmd.argc == 0){ //if nothing provided
			printf(CMD_WARN_NO_CMD);
			return WARN_NO_CMDS;
		}

		cmd.argv[cmd.argc] = 0;


		if (strcmp(cmd.argv[0], "cd") == 0){
			if (cmd.argc == 2){ //exactly one argument
				if (chdir(cmd.argv[1]) == 0){ //success
				} else { //error
					perror("cd error");
					rc = ERR_CMD_ARGS_BAD;
				}
			} else if (cmd.argc == 1){ //no arguments
				continue; 
			} else if (cmd.argc > 2){ //too many arguments
				printf("too many arguments for cd\n");
				rc = ERR_CMD_ARGS_BAD;
			}
		} else {
			int c_result;
			int f_result = fork();
			
			if (f_result < 0){
				perror("fork error");
				rc = 44;
				continue;
			}

			if (f_result == 0){
				rc = execvp(cmd.argv[0], cmd.argv);
				if (rc < 0){
					perror("execvp error");
					exit(22);
				}	
    		} else {
				wait(&c_result);
    		}
    	}
    }
   	

	
    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    return rc;
}
