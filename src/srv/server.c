
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "include/server.h"
#include "include/srvapi.h"
#include "include/comm.h"

int
check_table(int id, TableStatus * status)
{
	*status = AVAILABLE;

	return 0;
}

int
check_tables(TableStatus * status)
{
	int n;

	for (n = 0; n < MAX_TABLES; n++)
		status[n] = AVAILABLE;
	
	return 0;
}

int
occupy_table(int id, bool *success)
{
	*success = true;
	
	return 0;
}

int
free_table(int id, bool *success)
{
	*success = true;

	return 0;
}

int
reserve_table(int id, bool *success)
{
	*success = true;

	return 0;
}
