import subprocess
import struct

def call_cli(integers):
	process = subprocess.Popen(
		['./bin/cli/Polygonal-Intersections-CLI'], 
		stdin=subprocess.PIPE, 
		stdout=subprocess.PIPE, 
		stderr=subprocess.PIPE,
	)
	
	# Pack the integers into binary format
	input_data = struct.pack(f'{len(integers)}q', *integers)
	
	stdout, stderr = process.communicate(input=input_data)
	
	if process.returncode != 0:
		print(f"Error: {stderr.decode('utf-8')}")
	else:
		print(f"Output: {stdout.decode('utf-8')}")

if __name__ == "__main__":
	integers = [
		1, # num polygons
		1, # num polychains

		# polygon 0
		3,
		 100,    0,
		 -20,  100,
		-200, -100,
		
		# polychain 0
		2,
		0, -300,
		30, 30,
		-200, 100
	]

	call_cli(integers)
