#define CON_DEBUG

#include "logger.h"
#include <stdio.h>

int main () {
	
	con_logfile("logger_test.log");
	con_timestamp_format("%H:%M:%S");
	
	conft("zsiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiir");
	conft("yeahhhh");
	conft("ok");
	
	con_debug("fasza %d", 10);
	
	return 0;
}

