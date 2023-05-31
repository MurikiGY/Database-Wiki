# With a dump file, generate a file with only create tables and primary keys statements
# then, again with the dump file, generate a file with only insert foreign keys statements


# ====================== Main ======================

# If there is no first parameter
if [[ -z "$1" ]]
then
    # Wrong use of the parameters
    printf "USAGE: monet_post_translate.sh <dump_file>.sql\n"
    return 1
else
	file=$1

	#Remove sys, set schema and sequences
	sed 's/"sys".//g' $file | grep -v -E 'SET SCHEMA|CREATE SEQUENCE|ALTER TABLE' > temp

	#Remove coments, foreign keys from dump file and adjust types
	grep -v -E 'FOREIGN KEY|--' temp		| sed ':a;N;$!ba;s/,\n);/\n);/g' 	| sed 's/"//g' |
	sed 's/TINYINT/SMALLINT/g' 			| sed 's/DOUBLE/DOUBLE PRECISION/g' 	|
	sed 's/CHARACTER LARGE OBJECT/TEXT/g' > create_tables.sql

	#Generate the file of foreign keys creation
	grep -E 'CREATE TABLE|FOREIGN KEY|);' temp | sed -z 's/\n);/;/g' | sed 's/CREATE/ALTER/g' |
	sed -z 's/\n\tCONSTRAINT/\tCONSTRAINT/g'   | grep 'CONSTRAINT'   |
	sed -z 's/\tCONSTRAINT/\n\tCONSTRAINT/g'   | sed -z 's/(\n/\n/g' |
	sed 's/CONSTRAINT/ADD CONSTRAINT/g' > foreign_key.sql

	#Generate file to export data
	mkdir -p /tmp/database_data
	grep -v -E 'CONSTRAINT|START TRANSACTION|COMMIT' create_tables.sql				        |
	sed ':a;N;$!ba;s/,\n);/\n);/g' 	| sed 's/NOT NULL//g'							|
	sed 's/INTEGER//g'		| sed 's/DOUBLE PRECISION//g'			                	|
	sed 's/SMALLINT//g'		| sed 's/BOOLEAN//g'							|
	sed 's/BIGINT//g'		| sed 's/TEXT//g' 							|
	sed 's/VARCHAR(.*)//g'		| sed 's/DECIMAL(.*)//g'  						|
	tac				| sed -z 's/);\n\t/SELECT /g'					        |
	sed '/^SELECT/s/$/,/'		| sed ':a;N;$!ba;s/,\nCREATE TABLE/\nFROM/g'	    			|
	sed 's/(//g' 			| sed 's/SELECT /COPY SELECT\n\t/g' 			        	|
	sed 's/ .*,/,/g'		| sed -E 's/(FROM) ([a-zA-Z0-9_-]+)/\1 \2 \2/g'	  			|
	sed -E "s/FROM ([a-zA-Z0-9_-]+) ([a-zA-Z0-9_-]+)/FROM \1 INTO '\/tmp\/database_data\/\1.csv'/g"	|
	sed "/^.*FROM/s/$/USING DELIMITERS '|' NULL AS '';/g" > export_data.csv
	
	rm temp
fi
