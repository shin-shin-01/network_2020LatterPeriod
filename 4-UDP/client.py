from datetime import datetime
import socket

print('The client started at ', datetime.now())

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.sendto(b'Hello UDP', ('0.0.0.0', 80))
client.close()
