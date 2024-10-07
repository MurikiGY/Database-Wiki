#!/bin/bash

DIR="./queries"

for file in "$DIR"/*; do
    if [ -f "$file" ]; then
    	echo "Running query file: $file"
	    echo "$file" >> results.dat
	    clickhouse-client -d tpch --password='heregoesyourpassword' --queries-file $file --time 2>> results.dat > /dev/null
	    echo "" >> results.dat
    fi
done
