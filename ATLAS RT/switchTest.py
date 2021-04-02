import RPi.GPIO as GPIO
import socket
import time

UDP_IP = "127.0.0.1"
UDP_PORT = 8080

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

switches = [19, 21, 22, 24]
previous = len(switches) * [None]

print("Switch Client Started")

def main():
	GPIO.setmode(GPIO.BOARD)
	for switch in switches:
		GPIO.setup(switch, GPIO.IN)
		pass
	try:
		while True:
			time.sleep(0.001)
			for i in range(len(switches)):
				switch = switches[i]
				prev_value = previous[i]
				value = GPIO.input(switch)
				if value != prev_value:
					MESSAGE = str(switch) + " " + str(value)
					sock.sendto(bytes(MESSAGE, 'UTF-8'), (UDP_IP, UDP_PORT))
					previous[i] = value
	finally:
		GPIO.cleanup()

if __name__ == '__main__':
    main()
