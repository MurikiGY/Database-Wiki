#!/bin/bash

# DROP A DATABASE FOR CITUS

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
    psql -p 970$i -c "DROP DATABASE $DATABASE"
  done
  
  #Create the database in coordinator
  psql -p 9700 -c "DROP DATABASE $DATABASE"
fi 
