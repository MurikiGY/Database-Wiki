#!/bin/bash

DATABASE="tpch"
SCHEMA_FILE="ch_tpch_schema.sql"


echo "CREATE DATABASE"
clickhouse-client -q "CREATE DATABASE $DATABASE"

echo "CREATE TPCH TABLES"
clickhouse-client -d $DATABASE --queries-file $SCHEMA_FILE

echo "POPULATE DATABASE"
clickhouse-client -d $DATABASE -q "INSERT INTO region   SETTINGS format_csv_delimiter='|' FORMAT CSV" < ../dbgen/data/region.csv
clickhouse-client -d $DATABASE -q "INSERT INTO nation   SETTINGS format_csv_delimiter='|' FORMAT CSV" < ../dbgen/data/nation.csv
clickhouse-client -d $DATABASE -q "INSERT INTO customer SETTINGS format_csv_delimiter='|' FORMAT CSV" < ../dbgen/data/customer.csv
clickhouse-client -d $DATABASE -q "INSERT INTO supplier SETTINGS format_csv_delimiter='|' FORMAT CSV" < ../dbgen/data/supplier.csv
clickhouse-client -d $DATABASE -q "INSERT INTO part     SETTINGS format_csv_delimiter='|' FORMAT CSV" < ../dbgen/data/part.csv
clickhouse-client -d $DATABASE -q "INSERT INTO partsupp SETTINGS format_csv_delimiter='|' FORMAT CSV" < ../dbgen/data/partsupp.csv
clickhouse-client -d $DATABASE -q "INSERT INTO orders   SETTINGS format_csv_delimiter='|' FORMAT CSV" < ../dbgen/data/orders.csv
clickhouse-client -d $DATABASE -q "INSERT INTO lineitem SETTINGS format_csv_delimiter='|' FORMAT CSV" < ../dbgen/data/lineitem.csv


