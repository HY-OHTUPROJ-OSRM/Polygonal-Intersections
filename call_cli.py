import subprocess
import struct

def call_cli(integers):
	process = subprocess.Popen(
		['./bin/cli/Polygonal-Intersections-CLI'], 
		stdin=subprocess.PIPE, 
		stdout=subprocess.PIPE, 
		stderr=subprocess.PIPE,
	)

	input_data = struct.pack(f'{len(integers)}q', *integers)

	stdout, stderr = process.communicate(input=input_data)

	for x in struct.unpack(f'{len(stdout) // 8}q', stdout):
		print(x)

if __name__ == "__main__":
	integers = [
		2, # num polygons
		2, # num polychains

		# polygon 0
		3,
		 100,    0,
		 -20,  100,
		0, 0,

		# polygon 1
		3,
		-150, -100,
		-150, -150,
		 -50,  -50,
		
		# polychain 0
		8,
		100, -100,
		0, -100,
		30, 30,
		-50, 50,
		-50, 0,
		-100, -50,
		-100, -100,
		-100, -200,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		8,

		# polychain 1
		8,
		100, -100,
		0, -100,
		30, 30,
		-50, 50,
		-50, 0,
		-100, -50,
		-100, -100,
		-100, -200,
		11,
		12,
		13,
		14,
		15,
		16,
		17,
		18
	]

	call_cli(integers)
