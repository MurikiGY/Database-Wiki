
if [ ! -d /run/postgresql ]
then
	sudo mkdir /run/postgresql
	sudo chown postgres /run/postgresql
fi

NUM_WORKERS=4

#Start the coordinator
pg_ctl -D citus/coordinator -o "-p 9700" -l citus/logfiles/coordinator_logfile start

#Start the workers
for (( i=1; i<=NUM_WORKERS ;i++))
do
  pg_ctl -D citus/worker$i -o "-p 970$i" -l citus/logfiles/worker$i-logfile start
done
