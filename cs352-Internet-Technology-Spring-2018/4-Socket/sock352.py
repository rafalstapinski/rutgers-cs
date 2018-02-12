import socket as ip

class socket:

    def __init__(self):
        self.sock = ip.socket(ip.AF_INET, ip.SOCK_DGRAM)

    def socket(self):
        return self.sock

    def bind(self, address):
        self.sock.bind(address)

    def sendto(self, buff, address):
        self.sock.sendto(buff, address)

    def recvfrom(self, nbytes):
        return self.sock.recvfrom(nbytes)

    def close(self):
        self.sock.close()
