#!/bin/bash

# CREATES A DATABASE FOR CITUS

if [[ -z "$1" ]]
then
	# Wrong use of the parameters
	printf "USAGE: source create-database.sh <DATABASE_NAME>\n"
	return 1
else
  NUM_WORKERS=4
  DATABASE=$1
  
  #Create the database in the workers
  for (( i=1; i<=NUM_WORKERS ;i++))
  do
    psql -p 970$i -c "CREATE DATABASE $DATABASE"
    psql -d $DATABASE -p 970$i -c "CREATE EXTENSION citus"
  done
  
  #Create the database in coordinator
  psql -p 9700 -c "CREATE DATABASE $DATABASE"
  psql -d $DATABASE -p 9700 -c "CREATE EXTENSION citus"
  
  #Add the workers in the coordinator database
  for (( i=1; i<=NUM_WORKERS ;i++))
  do
    psql -d $DATABASE -p 9700 -c "SELECT * FROM citus_add_node('localhost', 970$i)"
  done
  
  #Verify the workers
  psql -d $DATABASE -p 9700 -c "SELECT * FROM citus_get_active_worker_nodes();"

fi 
