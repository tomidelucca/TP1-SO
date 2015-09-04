
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "include/server.h"
#include "include/srvapi.h"
#include "include/comm.h"

TableStatus
check_table(int id)
{
	return AVAILABLE;
}

void
check_tables(TableStatus * status)
{
	int n;

	for (n = 0; n < MAX_TABLES; n++)
		status[n] = AVAILABLE;
}

bool
occupy_table(int id)
{
	return true;
}

bool
free_table(int id)
{
	return true;
}

bool
reserve_table(int id)
{
	return true;
}
