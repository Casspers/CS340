#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: \n", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}


int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("test.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stdout, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "CREATE TABLE PASSWORDS("  \
         "USER TEXT PRIMARY KEY     NOT NULL," \
         "PASS           TEXT    NOT NULL );" ;

   

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }


   sql = "INSERT INTO PASSWORDS (USER, PASS) "  \
         "VALUES ('User1', 'Pass1'); " \
         "INSERT INTO PASSWORDS (USER, PASS)"  \
         "VALUES ('User2', 'Pass2'); "     \
         "INSERT INTO PASSWORDS (USER, PASS)" \
         "VALUES ('User3', 'Pass3');" \
         "INSERT INTO PASSWORDS (USER, PASS)" \
         "VALUES ('User4', 'Pass4');";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Records created successfully\n");
   }

   sql = "SELECT * FROM PASSWORDS";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Operation done successfully\n");
  }


   sqlite3_close(db);
   return 0;
}