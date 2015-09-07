
#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdbool.h>

#include "include/comm.h"

#define DB_PATH     "db.csv"

/**
 *	Check the status of the table
 *
 *	@param	id	ID of the table
 *	@param	status	Out parameter; Table status
 *	@return 0 if operation was successful, -1 if there was an error	
 */
int check_table(int id, TableStatus *status);

/**
 *	Get the status of all the tables and store them in `status`
 *
 *	@param	status	Out parameter; An array of status
 *	@return 0 if operation was successful, -1 if there was an error	
 */
int check_tables(TableStatus *status);

/**
 *	Occupy a table if it's not occupied
 *
 *	@param	id	ID of the table
 *	@param	success	Out parameter; true if the table is occupied 
 *			after the operation, else false
 *	@return 0 if operation was successful, -1 if there was an error	
 */
int occupy_table(int id, bool *success);

/**
 *	Free a table already occupied
 *
 *	@param	id	ID of the table
 *	@param	success	Out parameter; true if the table is free
 *			after the operation, else false
 *	@return 0 if operation was successful, -1 if there was an error	
 */
int free_table(int id, bool *success);

/**
 *	Reserve a table whenever it's not already reserved
 *
 *	@param	id 	ID of the table
 *	@param success	Out parameter; false if the table is not reserved
 *			after operation, else true
 *	@return 0 if operation was successful, -1 if there was an error	
 */
int reserve_table(int id, bool *success);

#endif
