#!/bin/bash


help() {
  printf "\n=== HELP PAGE ===\n\n"

  printf "=> size: script.sh ; size <database>\n"
  printf "The amount of disk usage by the database\n\n"

  printf "=> table_count: script.sh ; table_count <database>\n"
  printf "Number of tables in schema public\n\n"

  printf "=> table_name: script.sh ; table_name <database>\n"
  printf "List of tables in schema public\n\n"

  printf "=> overview: script.sh ; overview <database>\n"
  printf "Number of atributes and rows for each table\n\n"
}


#Disk usage of the database
database_size() {
  datname=$1
  printf "\n=== DATABASE DISK USAGE ===\n\n"
  printf "SEND: SELECT pg_size_pretty( pg_database_size( $datname ) ) AS $datname;\n\n"
  psql -d $datname -c "SELECT pg_size_pretty( pg_database_size( '$datname' ) ) AS $datname"
}


#Count the number of tables in public schema
database_table_count() {
  datname=$1
  printf "\n=== DATABASE TABLE COUNT ===\n\n"
  printf "SEND: 
  SELECT 
    COUNT(*) 
  FROM information_schema.tables 
  WHERE table_schema = 'public';\n\n"
  psql -d $datname -c "
  SELECT 
    COUNT(*) 
  FROM information_schema.tables 
  WHERE table_schema = 'public'"
}


#List all tables in public schema
database_table_names(){
  datname=$1
  printf "\n=== DATABASE TABLES ===\n\n"
  printf "SEND: 
  SELECT 
    tab.table_name
  FROM information_schema.tables 
  WHERE table_schema = 'public';\n\n"
  psql -d $datname -c "
  SELECT
    tab.table_name
	FROM information_schema.tables AS tab
	WHERE tab.table_schema = 'public'"
}


#Count number of atributes and rows for all tables
database_overview() {
  datname=$1
  printf "\n=== DATABASE ATRIBUTES AND TUPLES COUNT ===\n\n"
  echo "SEND:
  SELECT
    tab.table_name,
    pg_size_pretty( pg_relation_size(quote_ident(tab.table_name)) ) AS table_size,
    aux.column_count AS column_count,
    aux2.rows_n AS row_count
  FROM information_schema.tables AS tab
  
  INNER JOIN
    (SELECT
      col.table_name,
      COUNT(*) AS column_count
    FROM information_schema.columns AS col
    WHERE col.table_schema = 'public'
    GROUP BY col.table_name
    ORDER BY column_count desc) AS aux
  ON tab.table_name = aux.table_name
  
  INNER JOIN
    (WITH tbl AS
      (SELECT
        table_schema,
        table_name
      FROM information_schema.tables
      WHERE table_name not like 'pg_%' AND table_schema in ('public'))
    
    SELECT
      table_schema,
      table_name,
      (xpath('/row/c/text()', query_to_xml(format('select count(*) as c from %I.%I', table_schema, table_name), FALSE, TRUE, '')))[1]::text::int AS rows_n
    FROM tbl
    ORDER BY rows_n DESC) AS aux2
  ON aux2.table_name = tab.table_name
  
  WHERE tab.table_schema = 'public'
  ORDER BY pg_relation_size(quote_ident(tab.table_name)) desc, aux.column_count desc;"
  printf "\n"

  psql -d $datname -c "
  SELECT
	  tab.table_name,
		pg_size_pretty( pg_relation_size(quote_ident(tab.table_name)) ) AS table_size,
		aux.column_count AS column_count,
		aux2.rows_n AS row_count
  FROM information_schema.tables AS tab
  
  INNER JOIN
	  (SELECT
	    col.table_name,
	    COUNT(*) AS column_count
	  FROM information_schema.columns AS col
	  WHERE col.table_schema = 'public'
	  GROUP BY col.table_name
	  ORDER BY column_count desc) AS aux
  ON tab.table_name = aux.table_name

  INNER JOIN
    (WITH tbl AS
      (SELECT
        table_schema,
        table_name
      FROM information_schema.tables
      WHERE table_name not like 'pg_%' AND table_schema in ('public'))
    
    SELECT
    	table_schema,
      table_name,
      (xpath('/row/c/text()', query_to_xml(format('select count(*) as c from %I.%I', table_schema, table_name), FALSE, TRUE, '')))[1]::text::int AS rows_n
    FROM tbl
    ORDER BY rows_n DESC) AS aux2
  ON aux2.table_name = tab.table_name

  WHERE tab.table_schema = 'public'
	ORDER BY pg_relation_size(quote_ident(tab.table_name)) desc, aux.column_count desc;"
}


#For each atribute: bytes, n_distincts, null_fraction
table_statistics(){
  datname=$1
  tabname=$2
  printf "\n=== TABLE STATISTICS ===\n\n"
  echo "SEND:
  WITH tbl AS
    (SELECT
      table_name,
      column_name,
      data_type
    FROM information_schema.columns
    WHERE table_name = '$tabname')

  SELECT
    table_name,
    column_name,
    data_type,
    (xpath('/row/a/text()', query_to_xml(format('SELECT pg_size_pretty(SUM(pg_column_size(%I))) AS a FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS total_size,
    (xpath('/row/d/text()', query_to_xml(format('SELECT pg_size_pretty(TRUNC(AVG(pg_column_size(%I)), 4)) AS d FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS average_size,
    (xpath('/row/b/text()', query_to_xml(format('SELECT COUNT(DISTINCT(%I)) AS b FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS n_distinct,
    (xpath('/row/c/text()', query_to_xml(format('SELECT COUNT(%I) AS c FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS count_not_null
  FROM tbl"
  printf "\n"

  psql -d $datname -c "
  WITH tbl AS
    (SELECT
      table_name,
      column_name,
      data_type
    FROM information_schema.columns
    WHERE table_name = '$tabname')

  SELECT
    table_name,
    column_name,
    data_type,
    (xpath('/row/a/text()', query_to_xml(format('SELECT pg_size_pretty(SUM(pg_column_size(%I))) AS a FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS total_size,
    (xpath('/row/d/text()', query_to_xml(format('SELECT pg_size_pretty(TRUNC(AVG(pg_column_size(%I)), 4)) AS d FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS average_size,
    (xpath('/row/b/text()', query_to_xml(format('SELECT COUNT(DISTINCT(%I)) AS b FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS n_distinct,
    (xpath('/row/c/text()', query_to_xml(format('SELECT COUNT(%I) AS c FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS count_not_null
  FROM tbl"
}


#Display byte usage, Max, Min of the table from numeric types
table_numeric_types() {
  datname=$1
  tabname=$2
  printf "\n=== TABLE NUMERIC TYPES ===\n\n"
  echo "SEND:
  WITH tbl AS
    (SELECT
      table_name,
      column_name,
      data_type
     FROM information_schema.columns
     WHERE table_name = '$tabname'
     AND (data_type = 'smallint' 
          OR data_type = 'integer' 
          OR data_type = 'bigint' 
          OR data_type = 'decimal'
          OR data_type = 'numeric'
          OR data_type = 'real'
          OR data_type = 'double precision'
          OR data_type = 'smallserial'
          OR data_type = 'serial'
          OR data_type = 'bigserial'
         )
    )

  SELECT
    table_name,
    column_name,
    data_type,
    (xpath('/row/a/text()', query_to_xml(format('SELECT pg_size_pretty(SUM(pg_column_size(%I))) AS a FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS total_size,
    (xpath('/row/d/text()', query_to_xml(format('SELECT pg_size_pretty(TRUNC(AVG(pg_column_size(%I)), 4)) AS d FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS average_size,
    (xpath('/row/b/text()', query_to_xml(format('SELECT COUNT(DISTINCT(%I)) AS b FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS n_distinct,
    (xpath('/row/c/text()', query_to_xml(format('SELECT COUNT(%I) AS c FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS count_not_null,
    (xpath('/row/e/text()', query_to_xml(format('SELECT MAX(%I) AS e FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS max_value,
    (xpath('/row/f/text()', query_to_xml(format('SELECT MIN(%I) AS f FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS min_value
  FROM tbl"
  printf "\n"

  psql -d $datname -c "
  WITH tbl AS
    (SELECT
      table_name,
      column_name,
      data_type
     FROM information_schema.columns
     WHERE table_name = '$tabname'
     AND (data_type = 'smallint' 
          OR data_type = 'integer' 
          OR data_type = 'bigint' 
          OR data_type = 'decimal'
          OR data_type = 'numeric'
          OR data_type = 'real'
          OR data_type = 'double precision'
          OR data_type = 'smallserial'
          OR data_type = 'serial'
          OR data_type = 'bigserial'
         )
    )

  SELECT
    table_name,
    column_name,
    data_type,
    (xpath('/row/a/text()', query_to_xml(format('SELECT pg_size_pretty(SUM(pg_column_size(%I))) AS a FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS total_size,
    (xpath('/row/d/text()', query_to_xml(format('SELECT pg_size_pretty(TRUNC(AVG(pg_column_size(%I)), 4)) AS d FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS average_size,
    (xpath('/row/b/text()', query_to_xml(format('SELECT COUNT(DISTINCT(%I)) AS b FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS n_distinct,
    (xpath('/row/c/text()', query_to_xml(format('SELECT COUNT(%I) AS c FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS count_not_null,
    (xpath('/row/e/text()', query_to_xml(format('SELECT MAX(%I) AS e FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS max_value,
    (xpath('/row/f/text()', query_to_xml(format('SELECT MIN(%I) AS f FROM %I', column_name, table_name), FALSE, TRUE, '')))[1] AS min_value
  FROM tbl"

}


null_statistics() {
  datname=$1
  tabname=$2
  printf "Database $datname, Table $tabname will be analyzed for NULL values\n"

  # Get the table columns and write in aux.txt
  psql -d $datname -c "select distinct(column_name) from information_schema.columns where table_name = '$tabname'" | sed -n '3,$p' | sed '$d' | sed '$d' > aux.txt

  # ----- CREATE THE CSV FILE -----
  # Add the header
  echo colName\;total\;2007\;2008\;2009\;2010\;2011\;2012\;2013\;2014\;2015\;2016\;2017\;2018\;2019\;2020\;2021 > ${tabname}_statistics.txt
  # Iterate each column and find the avg nulls
  while read column; do
    echo -n $column\; >> ${tabname}_null_statistics.txt
    psql -U postgres -d $datname -c "select AVG(CASE WHEN $column is null THEN 1.0 ELSE 0 END) as \"$column NULL\" from $tabname" | sed -n 3p | xargs| tr -d '\n' >> ${tabname}_null_statistics.txt
    for year in {2007..2021}
    do
      echo -n \; >> ${tabname}_null_statistics.txt
      psql -U postgres -d $datname -c "select AVG(CASE WHEN $column is null THEN 1.0 ELSE 0 END) as \"$column NULL\" from $tabname where ano_censo = $year" | sed -n 3p | xargs | tr -d '\n' >> ${tabname}_null_statistics.txt
    done;
    echo >> ${tabname}_null_statistics.txt
    printf "$column analyzed!\n"
  done < aux.txt
}

true_statistics() {
  datname=$1
  tabname=$2
  printf "Database $datname, Table $tabname will be analyzed for TRUE values\n"

  # Get the table columns and write in aux.txt
  psql -d $datname -c "select distinct(column_name) from information_schema.columns where table_name = '$tabname' and data_type = 'boolean'" | sed -n '3,$p' | sed '$d' | sed '$d' > aux.txt

  # ----- CREATE THE CSV FILE -----
  # Add the header
  echo colName\;total\;2007\;2008\;2009\;2010\;2011\;2012\;2013\;2014\;2015\;2016\;2017\;2018\;2019\;2020\;2021 > ${tabname}_true_statistics.txt
  # Iterate each column and find the avg nulls
  while read column; do
    echo -n $column\; >> ${tabname}_true_statistics.txt
    psql -U postgres -d $datname -c "select AVG(CASE WHEN $column is true THEN 1.0 ELSE 0 END) as \"$column TRUE\" from $tabname where $column is not NULL" | sed -n 3p | xargs| tr -d '\n' >> ${tabname}_true_statistics.txt
    for year in {2007..2021}
    do
      echo -n \; >> ${tabname}_true_statistics.txt
      psql -U postgres -d $datname -c "select AVG(CASE WHEN $column is true THEN 1.0 ELSE 0 END) as \"$column TRUE\" from $tabname where ano_censo = $year and $column is not null" | sed -n 3p | xargs | tr -d '\n' >> ${tabname}_true_statistics.txt
    done;
    echo >> ${tabname}_true_statistics.txt
    printf "$column analyzed!\n"
  done < aux.txt
}
