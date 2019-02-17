#!/usr/bin/env python3

import sys

with open(sys.argv[1]) as f:
	for line in f.readlines():
		for x in line.split():
			sys.stdout.write(chr(int(x, 16)))
