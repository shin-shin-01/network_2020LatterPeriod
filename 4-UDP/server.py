from datetime import datetime
import socket

print('The server started at ', datetime.now())
print('Waiting...')

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server.bind(('0.0.0.0', 80))

data, addr = server.recvfrom(1024)
print('data: ', data, 'addr: ', addr)
server.close()
