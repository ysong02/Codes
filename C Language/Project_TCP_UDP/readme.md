# Internet and system programming
## Goal:
The client uses UDP to transfer documents, we want to improve the UDP performance by using some methodes in TCP. If possible, let many clients connect to the server at the same time. 
## Solution
We add some methods in TCP which include Three-way Handshake, Congestion Control: Slow start, Congestion Avoidance, Fast Retransmit and Fast Recovery. Moreover, we use RTT to evaluate the internet performance. To let many clients connect to the server at the same time, we use fork() and let different son processes connect to the clients.
## Command line to execute the codes
server which connect to one client:<br>
gcc -Wall -lm server1_rtt.c -o server1_rtt <br>
./server1_rtt <PortNumber> <br>
server which can connect to many clients: <br>
gcc -Wall -lm server3_00.c -o server3_00 <br>
./server3_00 <PortNumber> <br>
for clients, the file is a binary file, so it is recommended to use a system Linux for excution. <br>
./client1 <ServerIP> <PortNumber> <NameOfFile>
 
