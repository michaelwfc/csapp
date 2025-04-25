#!/usr/bin/python

# nop-server.py - This is a server that we use to create head-of-line
#                 blocking for the concurrency test. It accepts a
#                 connection, and then spins forever.
#
# usage: nop-server.py <port>  
#  <port>, which specifies the port number on which the server will listen.
# 
# Behavior:
# Once started, the server listens on the specified port. When a client connects, 
# the server accepts the connection but does nothing further, creating a blocking scenario that can be used to test concurrency in other systems.
#
import socket
import sys

#create an INET, STREAMing socket
# Creates a TCP socket using socket.AF_INET (IPv4) and socket.SOCK_STREAM (TCP protocol).
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
# Binds the socket to the specified port (int(sys.argv[1])) on all available interfaces ('').
serversocket.bind(('', int(sys.argv[1])))
# Sets the socket to listen for incoming connections with a backlog of 5.
serversocket.listen(5)

print(f"Server listening on port {sys.argv[1]}")
# Accepts an incoming connection, storing the client's channel and details.
# Enters another infinite loop (while 1: continue), causing the server to "spin" indefinitely without processing or closing the connection.
while 1:
  channel, details = serversocket.accept()
  while 1:
    continue
