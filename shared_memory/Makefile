ll: init shared

init: 
	mkdir -p build

shared: timeshmm.c 
	gcc -g -Wall -o build/time timeshmm.c -lrt

clean:
	 rm build/*
