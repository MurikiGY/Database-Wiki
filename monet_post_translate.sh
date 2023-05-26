
# ====================== Main ======================

# If there is no first parameter
if [[ -z "$1" ]]
then
    # Wrong use of the parameters
    printf "USAGE: monet_post_translate.sh <dump_file>.sql\n"
    return 1
else
	file=$1

	#Remove sequences and set schema
	grep -v -E 'CREATE SEQUENCE|ALTER TABLE|SET SCHEMA' $file > output.sql

	#Remove sys
	sed -i 's/"sys".//g' output.sql

	#Adjust types
	sed -i 's/TINYINT/SMALLINT/g' output.sql
	sed -i 's/DOUBLE/DOUBLE PRECISION/g' output.sql
	sed -i 's/CHARACTER LARGE OBJECT/TEXT/g' output.sql
fi
