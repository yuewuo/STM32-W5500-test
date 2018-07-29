import socket

connect_ip = '192.168.1.105'
connect_port = 22
def handleConnect(conn):
    conn.sendall(bytes('GET / HTTP/1.0\r\n\r\n\0\r\n\r\n', encoding='utf-8'))
    server_rply = conn.recv(1024)
    print(str(server_rply))

# under are the simple client functions
ip_port = (connect_ip, connect_port)
conn = socket.socket()
conn.connect(ip_port)
print("----connected on port:", ip_port)
handleConnect(conn)
conn.close()