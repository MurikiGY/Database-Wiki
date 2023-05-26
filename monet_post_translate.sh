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

	#Remove foreign keys from dump file and adjust types
	grep -v -E 'FOREIGN KEY' temp | sed ':a;N;$!ba;s/,\n);/\n);/g' 	|
	sed 's/TINYINT/SMALLINT/g' 					|
	sed 's/DOUBLE/DOUBLE PRECISION/g' 				|
	sed 's/CHARACTER LARGE OBJECT/TEXT/g' > create_tables.sql

	#Generate the file of foreign keys creation
	grep -E 'CREATE TABLE|FOREIGN KEY|);' temp | sed -z 's/\n);/;/g' | sed 's/CREATE/ALTER/g' | \
	sed -z 's/\n\tCONSTRAINT/\tCONSTRAINT/g'   | grep 'CONSTRAINT'   | \
	sed -z 's/\tCONSTRAINT/\n\tCONSTRAINT/g'   | sed -z 's/(\n/\n/g' | \
	sed 's/CONSTRAINT/ADD CONSTRAINT/g' > foreign_key.sql

	rm temp
fi
