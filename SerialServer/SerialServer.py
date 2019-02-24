import os
import sys
import struct
import serial
import time

# Print iterations progress
def print_progress(iteration, total, prefix='', suffix='', decimals=1, bar_length=80):
    """
    Call in a loop to create terminal progress bar
    @params:
        iteration   - Required  : current iteration (Int)
        total       - Required  : total iterations (Int)
        prefix      - Optional  : prefix string (Str)
        suffix      - Optional  : suffix string (Str)
        decimals    - Optional  : positive number of decimals in percent complete (Int)
        bar_length  - Optional  : character length of bar (Int)
    """
    str_format = "{0:." + str(decimals) + "f}"
    percents = str_format.format(100 * (iteration / float(total)))
    filled_length = int(round(bar_length * iteration / float(total)))
    bar = '█' * filled_length + '-' * (bar_length - filled_length)

    sys.stdout.write('%s |%s| %s%s %s\r' % (prefix, bar, percents, '%', suffix)),

    if iteration == total:
        sys.stdout.write('\n')
    sys.stdout.flush()

# Control characters
stx = b'S'
etx = b'E'
ack = b'A'

# Command line args
fd = sys.argv[1]
kernel_path = sys.argv[2]

# Initial variables
s = serial.Serial(fd, 115200)
bytes_sent = 0
total_bytes = os.stat(kernel_path).st_size
kernel = open(kernel_path, "rb")

print(kernel_path, "->", fd)
print_progress(bytes_sent,total_bytes)

def send(data):
	s.write(data)
	s.flush()

	receive = s.read(1)
	if not receive == ack:
		print()
		print("DID NOT GET ACK:", receive, "BYTE:", bytes_sent)
		exit(1)

while (True):
	data = kernel.read(1)

	if not data:
		send(etx)
		break


	send(stx)
	send(data)
	
	bytes_sent = bytes_sent + 1
	print_progress(bytes_sent,total_bytes)
