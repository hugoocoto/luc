all:
	rm report.log -f
	c99 src/*.c -o luc -Wall -Wextra
