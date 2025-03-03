#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"


void print_clist(command_list_t *clist){
	printf("-----------\n");
	printf("printing clist -- clist.num = %d\n\n", clist->num);

	for(int i = 0; i < clist->num; i++){
		printf("cmd_buffer[%d]: %s, arg count: %d\n", i, clist->commands[i]._cmd_buffer, clist->commands[i].argc);
		
		for(int j = 0; j < clist->commands[i].argc; j++){
			if(j  == 0){
				printf("command: %s\n", clist->commands[i].argv[j]);
			} else {
				printf("arg[%d]: %s\n", j, clist->commands[i].argv[j]);
			}
		}
		printf("\n");
	}
	printf("-----------\n");
}

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

int execute_pipeline(command_list_t *clist){
	int num = clist->num;
	int rc = OK;
	
	int pipes[num-1][2];
	pid_t pids[num];

	for (int i = 0; i < num-1; i++){
		if (pipe(pipes[i]) == -1){
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}
	
	//create processes for each command
	for (int i = 0; i < num; i++){
		pids[i] = fork();

		if (pids[i] == -1){
			perror("fork");
			rc = 44;
			exit(EXIT_FAILURE);
		}

		if (pids[i] == 0){
			cmd_buff_t *cmd = &clist->commands[i];

			if (i > 0){ //input pipes for all children except first
				dup2(pipes[i-1][0], STDIN_FILENO);
			}

			if (i < num-1){ //output pipes for all children except last 
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			
			for (int j = 0; j < num-1; j++){
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			
			execvp(cmd->argv[0], cmd->argv);
			perror("execvp");
			rc = 88;
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < num-1; i++){
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	for (int i = 0; i < num; i++){
		waitpid(pids[i], NULL, 0); //wait for all children
	}

	return rc;

}

int exec_one_cmd(cmd_buff_t cmd){
    int rc = 0; //success
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
   
   return rc;
}

int space_parse(char *cmdtoken, cmd_buff_t *cmd){
	char *spcsave;
    char *spctoken = strtok_r(cmdtoken, " ", &spcsave);

    int index = 0;
	while (spctoken != NULL){
        spctoken = trimspc(spctoken);

        if (cmd->argc > CMD_ARGV_MAX){
			printf("too many arguments\n");
			break;
	     }
	
		if (*spctoken == '\0'){ //at the end of the string
            spctoken = strtok_r(NULL, " ", &spcsave);
            continue;
        }

        if (spctoken[0] == '"'){
            int returnvalue = exactString(&cmd->_cmd_buffer[index], cmd);
            index += returnvalue;
            cmd->argc++;

            spctoken = strtok_r(&cmd->_cmd_buffer[index], " ", &spcsave);
            continue;
        }

        if (strlen(spctoken) > 0){
            cmd->argv[cmd->argc] = malloc(sizeof(char)*(strlen(spctoken)+1));
            strcpy(cmd->argv[cmd->argc], spctoken);
        	cmd->argc++;
            index += strlen(spctoken)+1;
        }

        spctoken = strtok_r(NULL, " ", &spcsave);
	}	
	return OK;
}


int exec_local_cmd_loop()
{
    char *cmd_buff = malloc(sizeof(char)*SH_CMD_MAX);
    int rc = 0;

    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
        break;
        }

        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        if (strcmp(cmd_buff, EXIT_CMD) == 0){
   			printf("exiting...\n");
   			exit(rc);
        }

        cmd_buff = trimspc(cmd_buff);
     
	 	if (strlen(cmd_buff) == 0){
        	printf(CMD_WARN_NO_CMD);
        	return OK;
    	}

		command_list_t clist;
   		clist.num = 0; //num of all commands

    	char *cmdsave;
    	char *cmdtoken = strtok_r(cmd_buff, PIPE_STRING, &cmdsave); // First token

    	while (cmdtoken != NULL){
        	if (clist.num >= CMD_MAX){
            	printf(CMD_ERR_PIPE_LIMIT, 8);
            	return OK;
        	}

        	if (*cmdtoken == '\0'){
            	cmdtoken = strtok_r(NULL, " ", &cmdsave);
            	continue;
        	}

        	trimspc(cmdtoken);
        	
			cmd_buff_t cmd;
			cmd._cmd_buffer = malloc(sizeof(char)*(strlen(cmdtoken)+1));
        	strcpy(cmd._cmd_buffer, cmdtoken); //copy entire string
			cmd.argc = 0;

			space_parse(cmdtoken, &cmd);
			cmd.argv[cmd.argc] = 0;

			clist.commands[clist.num] = cmd;
			clist.num++;
        	cmdtoken = strtok_r(NULL, PIPE_STRING, &cmdsave);

    	} // parse by command
		
		//print_clist(&clist);
		
		if (clist.num == 1){
			rc = exec_one_cmd(clist.commands[0]);
		} else {
			rc = execute_pipeline(&clist);
		}

    
    } //while loop
	
	return rc;
}
