
#ifndef _SERVER_H_
#define _SERVER_H_

#define DB_PATH     "db.csv"

/**
 *	Check the status of the table
 *
 *	@param	id ID of the table
 *	@return	The status of the specified table
 */
TableStatus check_table(int id);

/**
 *	Get the status of all the tables
 *
 *	@return	array of status
 */
TableStatus *tables_status(void);

/**
 *	Occupy a table if it's not occupied
 *
 *	@param	id ID of the table
 *	@return	False if the table is already occupied, else true
 */
bool occupy_table(int id);

/**
 *	Free a table already occupied
 *
 *	@param	id ID of the table
 *	@return	TODO
 */
bool free_table(int id);

/**
 *	Reserve a table whenever it's not already reserved
 *
 *	@param	id ID of the table
 *	@return	False whenever the specified table is already reserved
 */
bool reserve_table(int id);

#endif