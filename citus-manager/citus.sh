#!/bin/bash

NUM_WORKERS=4

#Create coordinator and worker folders
mkdir -p citus/coordinator
mkdir -p citus/logfiles
for (( i=1; i<=NUM_WORKERS ;i++))
do
  mkdir -p citus/worker$i
done


#Create normal postgres instances
initdb -D citus/coordinator
for (( i=1; i<=NUM_WORKERS ;i++))
do
  initdb -D citus/worker$i
done


#Load Citus for earch one of them
echo "shared_preload_libraries = 'citus'" >> citus/coordinator/postgresql.conf
for (( i=1; i<=NUM_WORKERS ;i++))
do
  echo "shared_preload_libraries = 'citus'" >> citus/worker$i/postgresql.conf
done


#Start coordinator and workers
pg_ctl -D citus/coordinator -o "-p 9700" -l citus/logfiles/coordinator_logfile start
for (( i=1; i<=NUM_WORKERS ;i++))
do
  pg_ctl -D citus/worker$i -o "-p 970$i" -l citus/logfiles/worker$i-logfile start
done


#Create citus for each one of them
psql -p 9700 -c "CREATE EXTENSION citus;"
for (( i=1; i<=NUM_WORKERS ;i++))
do
  psql -p 970$i -c "CREATE EXTENSION citus;"
done


#Sets the coordinator
psql -p 9700 -c "SELECT citus_set_coordinator_host('coordinator', 9700)"


#Tell the coordinator where to find workers
for (( i=1; i<=NUM_WORKERS ;i++))
do
  psql -p 9700 -c "SELECT * from citus_add_node('localhost', 970$i);"
done


#Verify the instalations of the workers
psql -p 9700 -c "SELECT * FROM citus_get_active_worker_nodes();"


#Delete the config file from the workers
cd citus
for (( i=1; i<=NUM_WORKERS ;i++))
do
  cd worker$i
  rm postgresql.conf
  ln -s ../coordinator/postgresql.conf postgresql.conf
  cd ..
done
cd ..


# ====================================================================

# CREATING THE DATABASE

# It will be necessary the database in the coordinator and the workers
# First create the database in each one of the workers
# > CREATE DATABASE example;
# > \c example
# > CREATE EXTENSION citus;

# This will replicate the query to all recognized nodes
# > SELECT run_command_on_workers('CREATE DATABASE example;');

# Then in the coordinator
# > CREATE DATABASE example;
# > \c example
# > CREATE EXTENSION citus;
# > SELECT * FROM citus_add_node('localhost', 9701);
# > SELECT * FROM citus_add_node('localhost', 9702);
# ...

# Verify if the workers were added
# > SELECT * FROM citus_get_active_worker_nodes();
# ====================================================================

# CREATING THE TABLES AND REFERENCE KEYS

# Crie as tabelas normalmente

# Insira chaves primarias uma por vez

# Distribua a tabela
# > SELECT create_distributed_table('table', 'column');

# Insira chaves estrangeiras uma por vez

# Para referenciar colunas com chaves estrangeiras, as tabelas podem ser
# copiadas em todos os workers, cortando o overhead
# !!referenciar em tabelas pequenas!!
# > SELECT create_reference_tables('table_name');

# Insira os dados

# ====================================================================
