from __future__ import print_function
import serial, os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

ser = serial.Serial('/dev/cu.usbmodem1421', 500000)
move = 1

def generate_data():
	while not ord(ser.read()) == 0:
		pass
	for y in range(h):
		for x in range(w):
			readByte = ser.read()
			if  ord(readByte)==0:
				ser.flushInput()
				break
			else:
				matrix1[y][x]=ord(readByte)
	print('\n'.join([''.join(['{:4}'.format(item) for item in row]) 
      for row in matrix1]))
	ser.flushInput()
	return matrix1

def data_gen():
    while True:
        yield generate_data(), generate_positions()


def generate_positions():
	global move
	matrix2 = np.zeros((w, h))
	
	if move==1:
		for i in range(14,28):
			for j in range(13,22):
				matrix2[i,j] = 150
				if matrix1[i][j]>150:
					move = 2
			for j in range(26, 35):
				matrix2[i,j] = 150
				if matrix1[i][j]>150:
					move = 2

	elif move == 2:
		for i in range(6,20):
			for j in range(26,35):
				matrix2[i,j] = 150

		for i in range(26, 40):
			for j in range(13, 22):
				matrix2[i,j] = 150
				if matrix1[i][j]>150:
					move = 3

	elif move == 3:
		for i in range(6,20):
			for j in range(13,22):
				matrix2[i,j] = 150

		for i in range(26, 40):
			for j in range(26, 35):
				matrix2[i,j] = 150
				if matrix1[i][j]>150:
					move = 1
	else:
		for i in range(14,28):
			for j in range(15,24):
				matrix2[i,j] = 150
			for j in range(28, 32):
				matrix2[i,j] = 150
	return matrix2

def run(data):
    # update the data
    matrix1, matrix2 = data

    # update the data of both line objects
    mat.set_data(matrix1)
    example.set_data(matrix2)

    return mat, example


# create a figure with two subplots
fig, ax = plt.subplots(1,2)
ax1 = ax[0]
ax2 = ax[1]

# intialize two line objects (one in each axes)
w, h = 44,48;
matrix1 = [[0 for x in range(w)] for y in range(h)]
matrix2 = [[0 for x in range(w)] for y in range(h)]
matrix = [matrix1, matrix2]

ax1.set_title('Your current stance')
ax2.set_title('Move to this stance')

mat = ax1.matshow(generate_data(),  vmin=0, vmax=250)
example = ax2.matshow(generate_positions(), vmin=0, vmax=250)

plt.colorbar(mat, orientation='horizontal')

ani = animation.FuncAnimation(fig, run, data_gen)
plt.show()

