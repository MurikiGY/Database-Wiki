# With a dump file, generate a file with only create tables and primary keys statements
# then, again with the dump file, generate a file with only insert foreign keys statements

# Generate dumpfile with msqldump -D <datname> > dumpfile.sql

# ====================== Main ======================

# If there is no first parameter
if [[ -z "$1" ]]
then
    # Wrong use of the parameters
    printf "USAGE: monet_post_translate.sh <dump_file>.sql\n"
    return 1
else
	file=$1

	#Remove sys, set schema and sequences and coments
	sed 's/"sys".//g' $file | grep -v -E 'SET SCHEMA|CREATE SEQUENCE|ALTER TABLE|--' > tmp

	#Remove primary and foreign keys from dump file and adjust types
	grep -v -E 'FOREIGN KEY|PRIMARY KEY' tmp  | sed ':a;N;$!ba;s/,\n);/\n);/g'    | sed 's/"//g' |
	sed 's/TINYINT/SMALLINT/g'                | sed 's/DOUBLE/DOUBLE PRECISION/g' |
	sed 's/CHARACTER LARGE OBJECT/TEXT/g'     | sed 's/HUGEINT/BIGINT/g' > create_tables.sql

	#Generate the file of restrictions
	grep -E 'CREATE TABLE|CONSTRAINT|);' tmp   | sed -z 's/\n);/;/g' | sed 's/CREATE/ALTER/g' |
	sed -z 's/\n\tCONSTRAINT/\tCONSTRAINT/g'   | grep 'CONSTRAINT'   |
	sed -z 's/\tCONSTRAINT/\n\tCONSTRAINT/g'   | sed -z 's/(\n/\n/g' |
	sed 's/CONSTRAINT/ADD CONSTRAINT/g' > restrictions.sql

	#Generate temp file to export from monet and import to postgres
	data_path='\/home\/banco\/dbteam\/simcaq_database\/'
	mkdir -p /home/banco/dbteam/simcaq_database/
	grep -v -E 'CONSTRAINT|START TRANSACTION|COMMIT' create_tables.sql  |
	sed ':a;N;$!ba;s/,\n);/\n);/g' 	| sed 's/NOT NULL//g'               |
	sed 's/INTEGER//g'              | sed 's/DOUBLE PRECISION//g'       |
	sed 's/SMALLINT//g'             | sed 's/BOOLEAN//g'                |
	sed 's/BIGINT//g'               | sed 's/TEXT//g'                   |
	sed 's/VARCHAR(.*)//g'          | sed 's/DECIMAL(.*)//g' > tmp2

	#Generate export file
	sed -z 's/\n//g' tmp2       | sed -z 's/CREATE/\nCREATE/g'      |
	tail -n +2                  | sed 's/CREATE TABLE /COPY /g'     |
	sed -e "s/\\COPY \(.*\) (\(.*\));/\\COPY \1 (\2) FROM \1 INTO '$data_path\1.csv' USING DELIMITERS '|' NULL AS '';/g" |
	sed 's/ [^ ]*//'            | sed -z 's/(/(\n/g'                |
	sed -z 's/,/,\n/g'          | sed -z 's/)/\n)/g'                |
	sed 's/ .*,/,/g'            | sed 's/(//g'                      |
	sed 's/) //g'               | sed 's/COPY /COPY SELECT /g' > export_data.sql

	#Generate import file
	sed 's/ .*,/,/g' tmp2           | sed -z 's/\n//g'                           |
	sed -z 's/CREATE/\nCREATE/g'    | tail -n +2                                 |
	sed 's/CREATE TABLE /\\COPY /g' |
	sed -e "s/\\COPY \(.*\) (\(.*\));/\\COPY \1 (\2) FROM '$data_path\1.csv';/g" |
	sed -z 's/\t//g'                | sed 's/ \{1,\}/ /g'                        |
	sed 's/ )/)/g' > import_data.sql

	rm tmp
	rm tmp2
fi
