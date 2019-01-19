#include "error.h"
#include "stdio.h"

// Unrecoverable error has occurred
void error(char* msg) {
	println("########### ERROR ###########");
	println(msg);
	println("#############################");
	while(1) {}
}

void generic_error() {
	error("A generic error occurred");
}

void marker() {
	error("Marker was triggered!");
}
