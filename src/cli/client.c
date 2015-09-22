
#include <stdio.h>

#include "include/server.h"

// #define FCNTL

#define WELCOME_MSG			"Insert your command (0 for help): "
#define SUCCESS_MSG			"\tSuccess.\n"
#define ERROR_MSG			"\tError.\n"
#define TABLE_STATUS_MSG	"\tTable %d: %s\n"
#define INVALID_COMMAND_MSG	"\tInvalid command.\n\n"
#define CHECK_TABLE_MSG		"Check the table: "
#define OCCUPY_TABLE_MSG	"Occupy the table: "
#define FREE_TABLE_MSG		"Free the table: "
#define RESERVE_TABLE_MSG	"Reserve the table: "

#define HELP	-1

static const char * status_msg[] = {
	"Non-existent",
	"Available",
	"Reserved",
	"Occupied"
};

static const char * help_msg[6] = {
	"\t(0) Help",
	"\t(1) Check table status",
	"\t(2) Check tables status",
	"\t(3) Occupy a table",
	"\t(4) Free a table",
	"\t(5) Reserve a table",
};

static int read_id(void);
static void process_command(int);
static void cli_help(void);
static void cli_check_table(void);
static void cli_check_tables(void);
static void cli_occupy_table(void);
static void cli_reserve_table(void);
static void cli_free_table(void);
static void cli_invalid_command(void);

static int
read_id(void)
{
	int id;

	// Read until the first signed decimal integer
	while (scanf("%d", &id) != 1);

	return id;
}

static void
process_command(int cmd)
{
	// substract 1 to use enum values defined for commands in comm.h
	// and to execute help with 0
	switch (cmd - 1) {
		case HELP:
			cli_help();
			break;

		case CHECK_TABLE:
			cli_check_table();
			break;

		case CHECK_TABLES:
			cli_check_tables();
			break;

		case OCCUPY_TABLE:
			cli_occupy_table();
			break;

		case FREE_TABLE:
			cli_free_table();
			break;

		case RESERVE_TABLE:
			cli_reserve_table();
			break;

		default:
			cli_invalid_command();
			break;
	}
}

static void
cli_help(void)
{
	int n = sizeof(help_msg) / sizeof(char *), i;

	for (i = 0; i < n; i++)
		printf("%s\n", help_msg[i]);

	printf("\n");
}

static void
cli_check_table(void)
{
	int id, s;
	TableStatus status;

	printf(CHECK_TABLE_MSG);
	id = read_id();
	s = check_table(id, &status);

	if (s == -1) {
		printf("There was an error checking the table %d.\n", id);
		return;
	}

	printf(TABLE_STATUS_MSG, id, status_msg[status]);
	printf("\n");
}

static void
cli_check_tables(void)
{
	int i, s;
	TableStatus status[MAX_TABLES];

	s = check_tables(status);
	if (s == -1) {
		printf("There was an error checking the tables.\n");
		return;
	}

	for (i = 0; i < MAX_TABLES; i++)
		printf(TABLE_STATUS_MSG, i, status_msg[status[i]]);

	printf("\n");
}

static void
cli_occupy_table(void)
{
	int id, s;
	bool success;

	printf(OCCUPY_TABLE_MSG);
	id = read_id();
	s = occupy_table(id, &success);
	if (s == -1) {
		printf("There was an error when occupying table %d.\n", id);
		return;
	}

	if (success) printf(SUCCESS_MSG);
	else printf(ERROR_MSG);

	printf("\n");
}

static void
cli_free_table(void)
{
	int id, s;
	bool success;

	printf(FREE_TABLE_MSG);
	id = read_id();
	s = free_table(id, &success);
	if (s == -1) {
		printf("There was an error when freeing table %d.\n", id);
		return;
	}

	if (success) printf(SUCCESS_MSG);
	else printf(ERROR_MSG);

	printf("\n");
}

static void
cli_reserve_table(void)
{
	int id, s;
	bool success;

	printf(RESERVE_TABLE_MSG);
	id = read_id();
	s = reserve_table(id, &success);
	if (s == -1) {
		printf("There was an error when reserving table %d.\n", id);
		return;
	}

	if (success) printf(SUCCESS_MSG);
	else printf(ERROR_MSG);

	printf("\n");
}

static void
cli_invalid_command(void) {
	printf(INVALID_COMMAND_MSG);
}

int
main()
{
	int cmd;

#ifndef FCNTL
	// If client communication can't initialize, end client
	printf("Initializing client... ");
	if (init_client() == -1) {
		printf("\x1B[31m[ERROR]\x1B[0m\nClient couldn't be initialized.\n");
		return 1;
	}
	printf("\x1B[32m[OK]\x1B[0m\n");
#endif

	while (1) {
		cmd = 0; // default to help

		printf(WELCOME_MSG);
		scanf("%d", &cmd);
		process_command(cmd);
	}

	return 0;
}
