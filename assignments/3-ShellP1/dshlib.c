#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
void trimspc(char *str){
	while (isspace(*str)){
		str++;
	}

	/*if (*str == '\0'){
		return;
	}*/

	char *end = str + strlen(str)-1;
	while (end >= str && isspace(*end)){
		end--;
	}

	*(end+1) = '\0';

}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
	if (strlen(cmd_line) == 0){
		printf(CMD_WARN_NO_CMD);
		return OK;
	}


	clist->num = 0; //num of all commands

	char *cmdsave;
	char *cmdtoken = strtok_r(cmd_line, PIPE_STRING, &cmdsave); // First token

	while (cmdtoken != NULL){
    	int single = 0;   
		int argcount = 0;
		
		if (clist->num > 7){
			printf(CMD_ERR_PIPE_LIMIT, 8);
			return OK;
		}

		if (*cmdtoken == '\0'){
			cmdtoken = strtok_r(NULL, " ", &cmdsave);
			continue;
		}

		trimspc(cmdtoken);
    	//printf("cmd token = |%s|\n", cmdtoken);
		
		char *spcsave;
		char *spctoken = strtok_r(cmdtoken, " ", &spcsave);
	
    	clist->commands[clist->num].args[0] = '\0';
		
		while (spctoken != NULL) {
    		if (*spctoken == '\0'){
				spctoken = strtok_r(NULL, " ", &spcsave);
				continue;
    		}
			trimspc(spctoken);
    		//printf("spc token = |%s|\n", spctoken);
    		if (single == 0){
    			strcpy(clist->commands[clist->num].exe, spctoken);
    		} else {
				if (strlen(spctoken) > 0){ //if there are arguments
					if (argcount == 0){ //if on first argument
						strncat(clist->commands[clist->num].args, spctoken, strlen(spctoken));
						argcount++;
					} else { 
						strcat(clist->commands[clist->num].args, " ");
    					strncat(clist->commands[clist->num].args, spctoken, strlen(spctoken));
    				}
    			}
    		}
    		single++;
			spctoken = strtok_r(NULL, " ", &spcsave); // Next token
		}
	
		clist->num++;
		cmdtoken = strtok_r(NULL, PIPE_STRING, &cmdsave);
	}

	
    printf(CMD_OK_HEADER, clist->num);
    
    for(int i = 0; i < clist->num; i++){
		printf("<%d> %s", i+1, clist->commands[i].exe);
    	
    	if (strlen(clist->commands[i].args) > 0){
    		printf("[%s]\n", clist->commands[i].args);
    	} else {
			printf("\n");
    	}
    }


    return OK;
}
