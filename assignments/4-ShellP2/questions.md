1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We call fork() before execvp() to start a new process. It duplicates the parent process and keeps track of the memory, which allows up to keep the parent process running while starting a new program.  

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork() fails my implementation will print out the error and not run the attempted command, instead moving onto the next input. If the fork() system call fails it will return -1 and it tells us that there is a problem will allocating sources or memory for this new process or other kernel issues. 

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  When we call execvp() we give it the command as an index in an array of strings and the entire array of the command and arguments. execvp() does not take a path but instead it automcatically handles the search for the command in the directories of the current PATH. If it finds the command it will replaces the current process with the process of the command, else it returns -1.   

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  We call wait() to allow the parent process to collect any information from the child process after it terminates, like its exit status. With this exit status we can remove the child process from the system and allow the parent process to continue. If we didn't call wait() the child, in a zombie state, would use up system resources and we would not be able to see whether or not the child was successful because we wouldn't have access to its exit status.  

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() is what allows us to return the exit status of the child to its parent. Since wait() contains a lot of information on the child, not just its exit status, WEXITSTATUS() actually extract the child's exit code.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My implementation of exec_local_cmd_loop() handles quoted arguments by keeping a character index for our current position in the original string. As we tokenize by spaces, if the first character of a token is a quotation mark we call a different function exactString. This function copies the string character by character into a temporary string until it reaches an end quotation mark. This temporary string is then added into the cmd.argv[] array and the function returns the length of the quoted argument to properly increment the original loop's index and continue tokenizing by space until the end of the string. 

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: In my parsing logic I struggled with keeping the character indexing accurate and then updating the string used by strtok_r(). I needed to make sure that I updated the string to be tokenized to start at the first word after the end quotation until the end of the entire starting string as a whole. I also need to make sure that every time the tokenization occured, the index properly reflected the number of characters passed.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals in a Linux system are typically used to make sure that a process is moving smoothly and that the process terminates early if it needs to. Each signal has a certain dispositionand from here processes know how to behave based on the signal. For example there is a disposition to signal to terminate a process, to ignore a signal, or to stop the process. Signals differ from other IPC since they do not share data directly as they are more so notifications and they can be be delivered at any time. 

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL = immediately/forcibly kill signal, might be used if a program is stuck or looping infintely; if a process is blocking system resources; a parent needs to kill an unresponsive child 
    SIGTERM = termination signal which requests the termination of a process rather than forcibly killing to aloows for  cleanup tasks like saving data or opening/closing files, might be used if you want to stop a program but save a user's data so far; avoid data corruption even if you need to terminate the program 
    SIGINT = interrupt from keyboard and terminate but cleanup before terminating, might be used if a user needs to a stop a program that is stuck/not loading; if a user wants to stop a process while its running 

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP stops or pauses a process. It cannot be caught or ignoreed like SIGINT so it will not execute cleanup tasks, instead it stops the process which can be resumed later. SIGSTOP works this way since it manages the execution of the process and does not request termination. It's supposed to be uncatchable since the operating system is supposed to have the power to control the process.





