#	build.sh
# 	Build Script for final project


gcc database.c -l sqlite3

./a.out

gcc -o Server Server.c -pthread -l sqlite3

#sleep .5

gcc -o Client Client.c

#rm database.db