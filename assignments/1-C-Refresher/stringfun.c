#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
void reverse_string(char*, int);
void word_print(char*, int, int);

//add additional prototypes here
bool is_whitespace(char);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
 	int user_str_len = 0;
    int space_count = 0;
	int buff_index = 0;
	char *start = buff; //the original start of buff

    while(*user_str != '\0'){
        user_str_len++;
        user_str++;
    }

    if (user_str_len > len){
        return -1; //the user string is too large
    }

    user_str -= user_str_len;

	user_str_len = 0; //reset to count whats actually being copied
    while (*user_str != '\0'){
        if (!(is_whitespace(*user_str)) && (space_count == 0)) {
			*buff = *user_str;
			user_str_len++;
        } else if (is_whitespace(*user_str) && (space_count == 0)) {
            *buff = ' ';
            space_count++;
            user_str_len++;
        } else if (!(is_whitespace(*user_str)) && (space_count == 1)) {
            *buff = *user_str;
            space_count = 0;
            user_str_len++;
        } else {
        	user_str++;
            continue;
        }

        buff++; //move buffer index over
        user_str++; //move str index over
        buff_index++;
    }

	while (buff_index < len){
		*buff = '.';
		buff++;
		buff_index++;
	}


	buff = start;

    return user_str_len; //return len of user string
}

void print_buff(char *buff, int len){
	printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count = 0;
	char *start = buff;

	for(int i = 0; i < len && i < str_len; i++){
		if (*(buff+i) == ' '){
			word_count++;
		}
	}

	buff = start;
    return word_count+1;
}

void reverse_string(char *buff, int user_str_len){
	int begin = 0; //beginning of string
	int end = user_str_len-1; //last index of string

	while(begin<end){
		char temp = *(buff+begin);
		*(buff+begin) = *(buff+end);
		*(buff+end) = temp;

		begin++;
		end--;
	}

	for (int i = 0; i < user_str_len; i++){
		printf("%c", *(buff+i));
	}

	printf("\n");
}

void word_print(char *buff, int user_str_len, int word_count){
	printf("Word Print\n");
	printf("----------\n");
	
	int letter_count = 0;
	int index = 0;

	for(int i = 0; i < word_count; i++){
		printf("%d. ", i+1);
		while (*buff != ' ' && *buff != '\0' && index < user_str_len){
			printf("%c", *buff);
			buff++;
			letter_count++;
			index++;
		}
		
		printf(" (%d)\n", letter_count);
		letter_count = 0;
		index++;
		buff++; //skip over space
	}

}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
bool is_whitespace(char test){
	return (test == ' ' || test == '\t' || test == '\n' || test == '\r' || 
			test == '\v' || test == '\f');
}


int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    /*
		First the program will check if argc is less than 2 
		which means that the user has only run the command
		"stringfun" without any arguments which will cause the program to do the 
		contents of the if statement body without checking the second half 
		of the "or" statement. If the first half of the
		"or" condition passes, it will then check if the second argument provided
		is a possible flag beginning with "-". If not, the program will carry out the 
		if statement body. In other words, this program is safe because it will 
		check if there are fewer than 2 arguments FIRST before even trying to access arv[1].

		The if statement body will call the usage() function will prints to the screen
		the arguments the program is expecting and their order. It will then also exit
		with the status of 1 which means that there was a command line problem.

		This first if statement is checking for the case where the user inputs
		"stringfun -h", since this flag does not require a third argument.
    */

    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    /*
		This if statement will check if the number of arguments is less than 3. Since the -h 
		flag has already been handled, if there are fewer than 3 arguments then the user's
		input is invalid. If the if statement's condition is true (i.e. there are less than
		3 arguments provided), then it will call the usage() function which shows the user
		the arugments and the order in which the program is expecting them, and exit with a 
		status of 1 which means that there was a command line problem.

		The purpose of this if statement is to check if the user has provided at least 3 
		arguments for the rest of the possible flags.
    */

    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3

    buff = malloc(sizeof(char)*BUFFER_SZ);
	if (buff == NULL){
		exit(99);
	}

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }
	
    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
		case 'r':
			reverse_string(buff, user_str_len);
			free(buff);
			exit(0); //success
			break;
        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'w':
			word_print(buff, user_str_len, count_words(buff, BUFFER_SZ, user_str_len));
        	free(buff);
        	exit(1);
        	break;
        case 'x':
			if (argc == 3) { //if user provided exactly 3 arguments
				printf("Not Implemented!\n");
			}
			free(buff);
			exit(1);
        	break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
/*         
		Since the buffer is just a collection of characters and not a string
		marked with the null terminating character, passing the length of the pointer
		is good practice because we can make sure that we stay within the range
		of buff's allocated memory since we don't have anything else marking the end
		of the range. This helps us make sure that we are working within our boundaries
		to avoid memory allocation problems.



*/
