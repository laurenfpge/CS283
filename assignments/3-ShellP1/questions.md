1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  The fgets() function is a good choice because it allows us to specify the number of characters to read to prevent buffer overflow making it a safer option. Also, since we are reading line by line fgets() is a god option because it stops reading after an EOF or newline and then stores it into a buffer.  

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  We used malloc() to allocate memory for cmd_buff instead of allocating a fixed-sizearray to ensure that we only allocate as much memory as we need and that it is allocated efficiently, without waste. It also gives us to option to use realloc() to be more flexible to user input length. 


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: If we didnt trim leading and trailing spaces from each command we could run into issues with buffer space but also problems with executing commands would arise because the extra spaces would cause command lookup to fail later since "  ls" and "ls" are not exact matches. It could also mess up are command arguments are handled since they are supposed to be separated by a single space.  

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  (#1) command > errors.txt (challenges might be with getting the redirection and its syntax to work properly) 
    (#2) ./dsh < input.txt (the problem might be with making sure it is a valid input file and parsing the contents by newlines) 
    (#3)  command > output.txt (similar to #1 the challenges might be with the redirection syntax but also file permissions or nonexisting files)

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  The difference between redirection and piping is that redirection changes where the output or input of a commands goes to/comes from. Piping is used to send the output of one commend to be used as input for another command. With redirection we can take the output of a program and input it to a file. With piping we could do ls | sort | wc -l which will print the items of a directory, then sort that list, and then report back the number of lines.  
    

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep STDOUT and STDERR separate so the user and programmer can distinguish between a successful run of the program versus a failed one or one with many problems/errors. This can make it easier to debug and handle errors if STDERR is grouped separately from STDDOUT. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Commands that fail should have proper error messages and return statuses so then it is easy to see where the error occurred. To merge STDOUT and STDERR we can output them to the same file. It might be helpful to have a way to merge them so we can see everything that went right and wrong at once. It might helps us pinpoint where the error is occuring too.  
