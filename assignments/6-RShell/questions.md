1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_answer here_ The remote client determines when a commands output is full received by knowing the numbers of bytes that we intend to send. To handle partial reads and ensure that complete messages are sent we can loop and read up to the number of bytes that we stated the message is.  

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_answer here_ A networked shell should define and detect the beginning and end of a command by clearly defining the beginning and end. Like the last question, we can pass the number of bytes the message is and use that to define the end or we can read until we find a special character used to mark the end. If this is not handled correctly, the commands may not be read correctly or fully and the server might have difficulty differentiating where one command starts and another ends.

3. Describe the general differences between stateful and stateless protocols.

_answer here_ Stateful protocols remembers previous client requests and stores that data. This means that the state is maintained amongst the different requests, allowing the sever to refer to previous data. Stateless protocols are the opposite and do not remember any information about past client requests/interations. It treats every request as its own and does not refer to previous requests.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_answer here_ We would use UDP because it is faster than TCP and would be better in applications where you need the information to travel fast and you can afford its unreliability. Since it does not error correction like TCP it is faster, but that also makes it unreliable because you can lose some data.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_answer here_ The interaction/abstraction that is provided by the operating system which enables application to use the network communications is the sockets API. This is how we were able to send and receive data using the network.
