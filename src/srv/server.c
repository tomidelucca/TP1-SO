
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "include/server.h"
#include "include/srvapi.h"
#include "include/comm.h"

#define CSV_TABLE_FORMAT "%d,%d;"

typedef struct {
	int id;
	TableStatus status;
} Table;

static struct flock rdlock = {
	.l_type = F_RDLCK,
	.l_whence = SEEK_SET,
	.l_start = 0,
	.l_len = 0
};

static struct flock wrlock = {
	.l_type = F_WRLCK,
	.l_whence = SEEK_SET,
	.l_start = 0,
	.l_len = 0
};

static struct flock unlock = {
	.l_type = F_UNLCK,
	.l_whence = SEEK_SET,
	.l_start = 0,
	.l_len = 0
};

/**
 *	Read the tables data from the database (csv file)
 *
 *	@param	tables	Table pointer to read the rows into
 */
static void
get_tables(Table *tables)
{
	int fd, table_id, n = 0;
	TableStatus table_status;
	FILE *file;

	fd = open(DB_PATH, O_RDONLY);
	file = fdopen(fd, "r");

	rdlock.l_pid = getpid();
	fcntl(fd, F_SETLKW, &rdlock);
	sleep(3);

	while (fscanf(file, CSV_TABLE_FORMAT, (int *) &table_id, (int *) &table_status) != EOF || n < MAX_TABLES) {
		tables[n].id = table_id;
		tables[n].status = table_status;
		n++;
	}

	unlock.l_pid = getpid();
	fcntl(fd, F_SETLK, &unlock);

	fclose(file);
}

/**
 *	Write the tables data into the database (csv file)
 *
 *	@param	tables	Table pointer to write the db rows
 */
static void
write_tables(Table *tables)
{
	int fd, n;
	FILE *file;

	fd = open(DB_PATH, O_WRONLY);
	file = fdopen(fd, "w");

	wrlock.l_pid = getpid();
	fcntl(fd, F_SETLKW, &wrlock);
	sleep(3);

	for (n = 0; n < MAX_TABLES; n++)
		fprintf(file, CSV_TABLE_FORMAT, tables[n].id, tables[n].status);

	unlock.l_pid = getpid();
	fcntl(fd, F_SETLK, &unlock);

	fclose(file);
}

int
check_table(int id, TableStatus * status)
{
	Table tables[MAX_TABLES];
	get_tables(tables);	
	
	if (id >= MAX_TABLES) {
		return -1;
	}

	*status = tables[id].status;

	return 0;
}

int
check_tables(TableStatus * status)
{
	Table tables[MAX_TABLES];
	get_tables(tables);

	int n;
	for (n = 0; n < MAX_TABLES; n++)
		status[n] = tables[n].status;
	
	return 0;
}

int
occupy_table(int id, bool *success)
{
	Table tables[MAX_TABLES];
	get_tables(tables);
	
	if (id >= MAX_TABLES) {
		*success = false;
		return -1;
	}

	tables[id].status = OCCUPIED;
	write_tables(tables);
		
	*success = true;
	
	return 0;
}

int
free_table(int id, bool *success)
{
	Table tables[MAX_TABLES];
	get_tables(tables);

	if (id >= MAX_TABLES) {
		*success = false;
		return -1;
	}

	tables[id].status = AVAILABLE;
	write_tables(tables);
	
	*success = true;

	return 0;
}

int
reserve_table(int id, bool *success)
{
	Table tables[MAX_TABLES];
	get_tables(tables);

	if (id >= MAX_TABLES || tables[id].status != AVAILABLE) {
		*success = false;
		return -1;
	}

	tables[id].status = RESERVED;
	write_tables(tables);

	*success = true;

	return 0;
}
