all:
	gcc -Wall -o Wpmotor wpmotor.c -lwiringPi
clean:
	rm Wpmotor
