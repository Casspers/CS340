#	build.sh
# 	Build Script for final project
#	To initilize the database for manipulation
#
#	Running this script will purge the database 
#	Starting over from the initila 4 users.

rm database.db

gcc database.c -l sqlite3

./a.out
sleep 1

gcc -o Server Server.c -pthread -l sqlite3

sleep 1

gcc -o Client Client.c

sleep 1

echo "---Successful Build---"

