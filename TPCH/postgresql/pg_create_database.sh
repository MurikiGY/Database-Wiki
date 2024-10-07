#!/bin/bash

DATABASE="tpch"
SCHEMA_FILE="pg_tpch_schema.sql"


echo "CREATE DATABASE"
echo "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-"
psql -c "DROP DATABASE $DATABASE"
psql -c "CREATE DATABASE $DATABASE"

echo "CREATE TPCH TABLES, INDEXES AND CONSTRAINTS"
echo "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-"
psql -d $DATABASE -f $SCHEMA_FILE

echo "POPULATE DATABASE"
echo "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-"
psql -d $DATABASE -c "\COPY region   FROM '../dbgen/data/region.csv' WITH (FORMAT csv, DELIMITER '|')";
psql -d $DATABASE -c "\COPY nation   FROM '../dbgen/data/nation.csv' WITH (FORMAT csv, DELIMITER '|')";
psql -d $DATABASE -c "\COPY customer FROM '../dbgen/data/customer.csv' WITH (FORMAT csv, DELIMITER '|')";
psql -d $DATABASE -c "\COPY supplier FROM '../dbgen/data/supplier.csv' WITH (FORMAT csv, DELIMITER '|')";
psql -d $DATABASE -c "\COPY part     FROM '../dbgen/data/part.csv' WITH (FORMAT csv, DELIMITER '|')";
psql -d $DATABASE -c "\COPY partsupp FROM '../dbgen/data/partsupp.csv' WITH (FORMAT csv, DELIMITER '|')";
psql -d $DATABASE -c "\COPY orders   FROM '../dbgen/data/orders.csv' WITH (FORMAT csv, DELIMITER '|')";
psql -d $DATABASE -c "\COPY lineitem FROM '../dbgen/data/lineitem.csv' WITH (FORMAT csv, DELIMITER '|')";


