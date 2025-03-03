1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

_To ensure that all child processes complete before the shell continues we use waitpid() in a for-loop so that its called for each individual child process. If we forgot to call waitpid() the children would go into zombie states until the parent gets their wait status. Over time having all of thse zombie child processes can use up a lot of system resources. In other cases if the parents exits before the child it would cause the piping not to work and and we would get incorrect output._

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

_Using dup2() is necessary to close pipe ends to ensure that we dont have processes left hanging and that instead they are properly terminated. This helps us make sure to conserve system resources, because if you leave pipes open they could continously eat up system resources. It also helps us make sure that the redirection of commands works properly at the redirected input/output is going to the correct pipe._

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

_"cd" is implemented as a built-in commands because it actually changes the directory the user is in. If cd were implemented as an external command it would mean that it runs as a child process instead of the parent. It would change the working directory of the child but when it exits and we return to the parent process, the parent directory would remaind unchanged._

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

_To allow an arbitrary number of piped commands I would have to repeatedly realloc space for the arrays that hold the command strings. Without a fixed number of pipes it would make freeing memory even more important to preserve system resources. You would need to consider the efficiency of frequent memory allocation and you would need to be very vigilant when it comes to error handling._
