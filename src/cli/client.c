#include <stdio.h>
#include <cliapi.h>

#define WELCOME_MSG			"Insert your command (0 for help): "
#define SUCCESS_MSG			"Success.\n"
#define ERROR_MSG			"Error.\n"
#define TABLE_STATUS_MSG	"Table %d: %s\n"
#define INVALID_COMMAND_MSG	"Invalid command.\n"
#define CHECK_TABLE_MSG		"Check the table: "
#define OCCUPY_TABLE_MSG	"Occupy the table: "
#define FREE_TABLE_MSG		"Free the table: "
#define RESERVE_TABLE_MSG	"Reserver the table: "

#define HELP	-1

static const char * status_msg[4];
status_msg[NON_EXISTENT] = "Non-existent";
status_msg[AVAILABLE] = "Available";
status_msg[RESERVED] = "Reserved";
status_msg[OCCUPIED] = "Occupied";

static const char * help_msg[] = {
	"(0) Help\n",
	"(1) Check table status\n",
	"(2) Check tables status\n",
	"(3) Occupy a table\n",
	"(4) Free a table\n",
	"(5) Reserve a table\n",
};

static int
read_id(void)
{
	int id;

	// Read until the first signed decimal integer
	while (scanf("%d", &id) == 1);

	return id;
}

void
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

void
cli_help(void)
{
	int n = sizeof help_msg, i;

	for (i = 0; i < n; i++)
		printf(help_msg[i]);
}

void
cli_check_table(void)
{
	int id;

	printf(CHECK_TABLE_MSG);
	id = read_id();
	TableStatus status = check_table(id);
	printf(TABLE_STATUS_MSG, id, status_msg[status]);
}

void
cli_check_tables(void)
{
	int i;
	TableStatus tables[MAX_TABLES];

	tables = check_tables();
	for (i = 0; i < MAX_TABLES; i++)
		printf(TABLE_STATUS_MSG, id, status_msg[status]);
}

void
cli_occupy_table(void)
{
	int id;
	bool result;

	printf(OCCUPY_TABLE_MSG);
	id = read_id();
	result = occupy_table(id);

	if (result)
		printf(SUCCESS_MSG);
	else
		printf(ERROR_MSG);
}

void
cli_free_table(void)
{
	int id;
	bool result;

	printf(FREE_TABLE_MSG);
	id = read_id();
	result = free_table(id);

	if (result)
		printf(SUCCESS_MSG);
	else
		printf(ERROR_MSG);
}

void
cli_reserve_table(void)
{
	int id;
	bool result;

	printf(RESERVE_TABLE_MSG);
	id = read_id();
	result = reserve_table(id);

	if (result)
		printf(SUCCESS_MSG);
	else
		printf(ERROR_MSG);
}

void
cli_invalid_command(void) {
	printf(INVALID_COMMAND_MSG);
}

int
main()
{
	int cmd;

	while (1) {
		cmd = 0; // default to help

		printf(WELCOME_MSG);
		scanf("%d\n", &cmd);
		process_command(cmd);
	}
}
