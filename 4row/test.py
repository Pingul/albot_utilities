import socket
import random

TCP_IP = '127.0.0.1'
TCP_PORT = 5000
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

while True:
    data = s.recv(BUFFER_SIZE)
    message = data.decode("utf-8").split( )
    msg = str(random.randint(0, 6))
    print(msg)
    s.send(bytes(msg, "utf-8"))
    
s.close()
