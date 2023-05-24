#!bin/bash

# Search the statistical data from table
table_statistics () {
    datname=$1
    tabname=$2
    dirname=$3

    #The directory exists
    if [ ! -d $dirname ]
    then
        mkdir $dirname
    fi

    #Table general data
    printf "Searching data from $tabname...\n"
    sudo -i -u postgres psql -d $datname -c 'SELECT
        attname AS column,
        null_frac,
        avg_width,
        n_distinct,
        most_common_vals
    FROM pg_stats
    WHERE tablename = '"'$tabname'"' ' > $dirname/$tabname.datatable
    printf "Generated file: $table.datatable in $directory\n"

    #Table statistics, ADD STATISTICAL SEARCH HERE
    head -n -2 $dirname/$tabname.datatable | tail -n +3 > $tabname.tmp
    printf "< $tabname >\n" > $dirname/$tabname.statistics
    #NULL percentage
    awk -F '|' '{sum+=$2} END { print "NULL %: ",sum/NR}' $tabname.tmp >> $dirname/$tabname.statistics
    #Distinct== 0 or 1
    printf "                column               |   null_frac   | byte_width |  n_distinct   |  most_common_vals\n" >> $dirname/$tabname.statistics
    printf "=====================================+===============+============+===============+==================\n" >> $dirname/$tabname.statistics
    awk -F '|' '$4 == 0 || $4 == 1' $tabname.tmp >> $dirname/$tabname.statistics
    printf "\n\n" >> $dirname/$tabname.statistics
    #Graphic
    #cut -d'|' -f1,2 $tabname.tmp | LC_ALL=C sort -t'|' -k2 --numeric-sort > data/$tabname

    #Remove temp files
    rm $tabname.tmp
    printf "Generated file: $tabname.statistics in $dirname\n"
}

# ====================== Main ======================

# If there is no first parameter
if [[ -z "$1" ]]
then
	# Wrong use of the parameters
	printf "USAGE: source database_overview.sh <DATABASE_NAME> or source database_overview.sh <DATABASE_NAME> <TABLE_TABLE>\n"
	return 1
else
	# If there is no second parameter
	if [[ -z "$2" ]]
	then
		datname=$1
		
		#General data
		printf "Searching general data from $datname\n"
		printf "Database disk usage:\n" > $datname.statistics
		sudo -i -u postgres psql -d $datname -c 'SELECT pg_size_pretty( pg_database_size( '"'$datname'"' ) ) AS '$datname'' >> $datname.statistics
		printf "Number of tables:\n" >> $datname.statistics
		sudo -i -u postgres psql -d $datname -c 'SELECT count(*) FROM information_schema.tables WHERE table_schema = '"'public'"'' >> $datname.statistics
		
		#All Tables data
		printf "Calculating data over all database, this might lead some thime...\n"
		printf "Tables overview:\n" >> $datname.statistics
		sudo -i -u postgres psql -d $datname -c 'SELECT
		    tab.table_name,
		    pg_size_pretty( pg_relation_size(quote_ident(tab.table_name)) ) AS table_size,
		    aux.column_count AS column_count,
			aux2.rows_n AS row_count
		FROM information_schema.tables AS tab
		
		INNER JOIN
		    (SELECT
		        col.table_name,
		        count(*) AS column_count
		    FROM information_schema.columns AS col
		    WHERE col.table_schema = '"'public'"'
		    GROUP BY col.table_name
		    ORDER BY column_count desc) AS aux
		ON tab.table_name = aux.table_name
		
		INNER JOIN
			(WITH tbl AS
		   		(SELECT
					table_schema,
		           	table_name
		    	FROM information_schema.tables
		    	WHERE table_name not like '"'pg_%'"' AND table_schema in ('"'public'"'))
			
		 	SELECT
				table_schema,
		        table_name,
		        (xpath('"'/row/c/text()'"', query_to_xml(format('"'select count(*) as c from %I.%I'"', table_schema, table_name), FALSE, TRUE, '"''"')))[1]::text::int AS rows_n
		 	FROM tbl
		 	ORDER BY rows_n DESC) AS aux2
		ON aux2.table_name = tab.table_name
		
		WHERE tab.table_schema = '"'public'"'
		ORDER BY pg_relation_size(quote_ident(tab.table_name)) desc, aux.column_count desc;' >> $datname.statistics

		#Search statistical data from all tables
		sudo -i -u postgres psql -d $datname -c 'SELECT
			tab.table_name
		FROM information_schema.tables AS tab
		WHERE tab.table_schema = '"'public'"';' | head -n -2 | tail -n +3 > TableList.tmp
		printf "\n ----- DATABASE CRITICAL INFORMATION ----- \n" >> $datname.statistics
		for table in `cat TableList.tmp`
		do
			table_statistics $datname $table TablesData
			cat TablesData/$tabname.statistics >> $datname.statistics
		done
		rm TableList.tmp
		printf "Generated database statistics file $datname.statistics\n"

	# Specific table statistic
	else
		datname=$1
		table=$2
		directory="TablesData"
		table_statistics $datname $table $directory
	fi
fi

