#include <stdio.h>
#include <cliapi.h>

#define SUCCESS_MSG			"Success.\n"
#define ERROR_MSG			"Error.\n"
#define INVALID_COMMAND_MSG	"Invalid command.\n"
#define CHECK_TABLE_MSG		"Check the table: "
#define OCCUPY_TABLE_MSG	"Occupy the table: "
#define FREE_TABLE_MSG		"Free the table: "
#define RESERVE_TABLE_MSG	"Reserver the table: "
#define TABLE_STATUS_MSG	"Table %d: %s\n"

static const char * status_msg[4];
status_msg[NON_EXISTENT] = "Non-existent";
status_msg[AVAILABLE] = "Available";
status_msg[RESERVED] = "Reserved";
status_msg[OCCUPIED] = "Occupied";

void
process_command(int cmd)
{
	switch (cmd) {
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
cli_check_table(void)
{
	int id;

	printf(CHECK_TABLE_MSG);
	// TODO get id from stdin
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
	// TODO get id from stdin
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
	// TODO get id from stdin
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
	// TODO get id from stdin
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

}
