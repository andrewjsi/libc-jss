#define CON_DEBUG

#include "logger.h"
#include <stdio.h>
#include <errno.h>

int main () {
	
	con_logfile("logger_test.log");
	con_timestamp_format("%a %H:%M:%S");
	
	conft("zsiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiir");
	conft("yeahhhh");
	conft("ok");
	
	FILE *f;
	if (!(f = fopen("/proc/ioports", "r"))) {
		perror("fopen");
		return 1;
	}
	
	char tmp[128];
	int i = 0;
	while (fgets(tmp, sizeof(tmp), f)) {
		con_debug("i=%d %s", i, tmp);
		i++;		
	}
	


	fclose(f);
	
	return 0;
}

