import socket

s = socket.socket()
s.bind(('192.168.0.8', 8090 ))
s.listen(0)

while True:

    client, addr = s.accept()
    while True:
        content = client.recv(32)
        if len(content) ==0:
            break
        else:
            client.sendall(content)

    print("Closing connection")
    client.close()