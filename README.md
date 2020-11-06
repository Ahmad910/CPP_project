1. Build Server

Go to 'cpp/clientserver' in terminal and type 'make'.

2. Run tests

2.1 Start server
Type './test/myserver port'.
You can pick a random port (9999) for example.

2.2 Start test program
Open a new terminal (it can be on the same computer or a different one) and go to 'cpp'.
Type 'java -jar TestServer1.jar hostname port'.
You should use the same port as the one you used to start the server.
The hostname should be the ip-address of the computer you have the server on.
You can find out your ip address by typing 'hostname -I'.
