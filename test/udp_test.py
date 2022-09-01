import socket
import struct
import numpy as np
import time



UDP_IP = "127.0.0.1"
UDP_PORT = 5555

def old_main():
    print("socket")
    csock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #pkt = bytearray(1458)
    pkt = np.zeros(729, dtype = np.uint16)
    pkt[0] = 3
    pkt[1] = 0
    pkt[2] = 0
    
    # p1
    pkt[3] = 200
    pkt[4] = 200
    pkt[5] = 8
    
    # p2
    pkt[3] = 200
    pkt[4] = 200
    pkt[5] = 8
    
    # p3
    pkt[3] = 200
    pkt[4] = 200
    pkt[5] = 8
    
    
    csock.sendto(pkt.tobytes(), (UDP_IP, UDP_PORT))
    csock.sendto(pkt.tobytes(), (UDP_IP, UDP_PORT))
    csock.sendto(pkt.tobytes(), (UDP_IP, UDP_PORT))

def main(bits, pbits):
    print("socket")
    csock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #pkt = bytearray(1458)
    pkt = np.zeros(729, dtype = np.uint16)
    pkt[0] = 242
    #pkt[0] = 1

    pkt[1] = 0
    pkt[2] = 0
    
    
    N = 1 << bits
    mask = 0xffff >> (16 - bits)
    sig = N/100
    
    NP = 1 << pbits
    pmask = 0xffff >> (16 - pbits)
    psig = NP/50
    
    
    for i in range(1000):
        start = time.time()
        for i in range(242):
            x = abs(int(np.random.normal()*sig + N//2))
            y = abs(int(np.random.normal()*sig + N//2))
            p = abs(int(np.random.normal()*5 + 128))

            pkt[3 + i] = x & mask
            pkt[4 + i] = y & mask
            pkt[5 + i] = p & pmask
        
    
        csock.sendto(pkt.tobytes(), (UDP_IP, UDP_PORT))
        time.sleep(0.001)
        end = time.time()
        print(end - start)
        
        
        
    #csock.sendto(pkt.tobytes(), (UDP_IP, UDP_PORT))
    #csock.sendto(pkt.tobytes(), (UDP_IP, UDP_PORT))


if __name__ == '__main__':
    bits = 11
    pbits = 8
    main(bits, pbits)