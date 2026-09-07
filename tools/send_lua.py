#!/usr/bin/env python3
import sys
import socket
def send_payload(ip, port, filepath):
    with open(filepath, 'rb') as f:
        data = f.read()
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((ip, port))
    sock.sendall(len(data).to_bytes(8, 'little') + data)
    sock.close()
    print(f"Sent {len(data)} bytes to {ip}:{port}")
if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: send_lua.py <ip> <port> <lua_file>")
        sys.exit(1)
    send_payload(sys.argv[1], int(sys.argv[2]), sys.argv[3])
