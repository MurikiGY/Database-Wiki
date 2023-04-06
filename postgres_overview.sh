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
		
		printf "Searching general data from $datname\n"

		# Database size
		printf "Database disk usage:\n"
		sudo -i -u postgres psql -d $datname -c 'SELECT pg_size_pretty( pg_database_size( '"'$datname'"' ) ) AS '$datname''

		# Tota tables
		printf "Number of tables:\n"
		sudo -i -u postgres psql -d $datname -c 'SELECT count(*) FROM information_schema.tables WHERE table_schema = '"'public'"''
		
		# Tables data
		printf "Tables overview:\n"
		sudo -i -u postgres psql -d $datname -c 'SELECT 
			tab.table_name, 
			pg_size_pretty( pg_relation_size(quote_ident(tab.table_name)) ) AS table_size, 
			aux.column_count, 
			aux2.rows_count 
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
			(SELECT 
				rows.relname, 
				rows.n_live_tup AS rows_count 
			FROM pg_stat_user_tables AS rows) AS aux2 
		ON tab.table_name = aux2.relname 
		WHERE tab.table_schema = '"'public'"' 
		ORDER BY pg_relation_size(quote_ident(tab.table_name)) desc, aux.column_count desc, aux2.rows_count desc;'
		
		## If count rows doesn't work and if your database isn't too large:
		#printf "Tables rows"
		#sudo -i -u postgres psql -d $datname -c 'WITH tbl AS 
		#  (SELECT table_schema, 
		#          TABLE_NAME 
		#   FROM information_schema.tables 
		#   WHERE TABLE_NAME not like '"'pg_%'"' 
		#     AND table_schema in ('"'public'"')) 
		#
		#SELECT table_schema, 
		#       TABLE_NAME, 
		#       (xpath('"'/row/c/text()'"', query_to_xml(format('"'select count(*) as c from %I.%I'"', table_schema, TABLE_NAME), FALSE, TRUE, '"''"')))[1]::text::int AS rows_n 
		#FROM tbl 
		#ORDER BY rows_n DESC;'

	# Table overview
	else

		datname=$1
		table=$2
		printf "Searching data from $table in $datname...\n\n"

		# Null percentage
		printf "Null relation:\n"
		sudo -i -u postgres psql -d $datname -c 'SELECT
		    attname AS column,
		    null_frac,
		    avg_width,
		    n_distinct,
		    most_common_vals
		FROM pg_stats
		WHERE tablename = '"'$table'"' '

	fi
fi

