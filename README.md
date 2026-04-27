***Compile:***
~~~bash
gcc -o server server.c -lrt
gcc -o client client.c -lrt
~~~

***To run:***
~~~bash
example:
./server 5001
./client localhost 5001

./server 5001
./client 192.168.***.*** 5001
~~~
