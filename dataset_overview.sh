# Parameter check
if [[ -z "$1" ]]
then
    #if less than three paramaters are received, return the correct usage and stops execution
	echo "USAGE: source database_overview.sh <DATABASE_NAME>"
	return 1
fi

datname=$1

# Database size
echo "Database disk usage:"
sudo -i -u postgres psql -d $datname -c 'SELECT pg_size_pretty( pg_database_size('"'$datname'"') ) AS '$datname''

# Tota tables
echo "Number of tables:"
sudo -i -u postgres psql -d $datname -c 'SELECT count(*) FROM information_schema.tables WHERE table_schema = '"'public'"''

# Tables data
echo "Tables overview:"
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

# If count rows doesn't work and if your database isn't too large:
#echo "Tables rows
#WITH tbl AS
#  (SELECT table_schema,
#          TABLE_NAME
#   FROM information_schema.tables
#   WHERE TABLE_NAME not like 'pg_%'
#     AND table_schema in ('public'))
#
#SELECT table_schema,
#       TABLE_NAME,
#       (xpath('/row/c/text()', query_to_xml(format('select count(*) as c from %I.%I', table_schema, TABLE_NAME), FALSE, TRUE, '')))[1]::text::int AS rows_n
#FROM tbl
#ORDER BY rows_n DESC;
