#!/bin/bash

DATABASE="tpch"
QUERY_FILE="queries"

clickhouse-client -d $DATABASE -t --queries-file $QUERY_FILE/ch18.sql
