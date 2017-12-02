#	build.sh
# 	Build Script for final project

clear 

#gcc database.c -l sqlite3

#./a.out

gcc -o Server Server.c -pthread

#sleep .5

#gcc -o Client Client.c

#rm database.db