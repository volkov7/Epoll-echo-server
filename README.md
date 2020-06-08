# The (learning) goals of the project:
[![forthebadge](https://forthebadge.com/images/badges/made-with-c.svg)](https://forthebadge.com)

Task is to create epoll echo server that:
- Listens the predefined ip address and port, and "register" all clients.
- Identifies each new client when connecting.
- Receives a message from any client, and sends message back.
# Requirements:
- Linux OS with a kernel no lower than version 2.5.66
- GNU gcc
- GNU make
# Installing and running:
Install:
```
make
```
Running:
```
./server
```
![Server](https://github.com/volkov7/Epoll-echo-server/raw/master/epoll_server.png)
