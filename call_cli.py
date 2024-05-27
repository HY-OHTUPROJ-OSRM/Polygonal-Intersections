import subprocess
import struct

def call_cli(input_data):
	process = subprocess.Popen(
		['./bin/cli/Polygonal-Intersections-CLI'], 
		stdin=subprocess.PIPE, 
		stdout=subprocess.PIPE, 
		stderr=subprocess.PIPE,
	)

	stdout, stderr = process.communicate(input=input_data)

	if process.returncode != 0:
		print(stderr)
		return

	for x in struct.unpack(f'{len(stdout) // 8}q', stdout):
		print(x)

def pack_32(*x):
	return struct.pack(f'{len(x)}i', *x)

def pack_64(*x):
	return struct.pack(f'{len(x)}q', *x)

if __name__ == "__main__":
	input_data = (
		  pack_64(2) # num polygons
		+ pack_64(2) # num polychains

		# polygon 0
		+ pack_64(3)
		+ pack_32(100, 0, -20,  100, 0, 0)

		# polygon 1
		+ pack_64(3)
		+ pack_32(-150, -100, -150, -150, -50, -50)

		# polychain 0
		+ pack_64(8)
		+ pack_32(100, -100, 0, -100, 30, 30, -50, 50, -50, 0, -100, -50, -100, -100, -100, -200)
		+ pack_64(1, 2, 3, 4, 5, 6, 7, 8)

		# polychain 1
		+ pack_64(8)
		+ pack_32(100, -100, 0, -100, 30, 30, -50, 50, -50, 0, -100, -50, -100, -100, -100, -200)
		+ pack_64(11, 12, 13, 14, 15, 16, 17, 18)
	)

	call_cli(input_data)

"""
expected output:

3
3
4
6
7
7
8
12
13
13
14
16
17
17
18
"""
