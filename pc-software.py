from __future__ import print_function
import serial, os
import matplotlib.pyplot as plt
import matplotlib.animation as animation

ser = serial.Serial('COM6', 500000)

w, h = 138, 48;
matrix = [[0 for x in range(w)] for y in range(h)]


def generate_data():
	while not ord(ser.read()) == 0:
		pass
	for y in range(h):
		for x in range(w):
			readByte = ser.read()
			if  ord(readByte)==0:
				break
			else:
				matrix[y][x]=ord(readByte)
	print('\n'.join([''.join(['{:4}'.format(item) for item in row]) 
      for row in matrix]))
	return matrix
	  
def update(data):
    mat.set_data(data)
    return mat
	
def data_gen():
    while True:
        yield generate_data()
		
fig, ax = plt.subplots()
mat = ax.matshow(generate_data(),  vmin=0, vmax=100)
ax.autoscale(False)
plt.colorbar(mat)
ani = animation.FuncAnimation(fig, update, data_gen)

plt.show()

