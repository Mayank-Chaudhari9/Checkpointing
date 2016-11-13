/**
  compile with following command
    gcc -o test $(mysql_config --cflags) simpledb.c $(mysql_config --libs)
*/

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

int  main(int argc, char const *argv[]) {
    /* code */
    MYSQL *connection;
    MYSQL_RES *result;
    MYSQL_ROW row;
    int state;
    char *server= "localhost";
    char *user="root";
    char *password="toor";
    char *database="test";

    connection = mysql_init(NULL);
    if(!mysql_real_connect(connection,server,user,password,database,0,NULL,0))
    {
      fprintf(stderr, "%s\n",mysql_error(connection));
      exit(1);
    }
    else
      printf("connection successful\n" );

    state =mysql_query(connection,"select * from users");
      if (state!=0)
       {
         printf("\n",mysql_error(connection));
         return 1;
        }

      printf("MvSQL table data \n");

      //store result before printing it

      result = mysql_store_result(connection);
      printf("Rows : %d\n",mysql_num_rows(result));

      //process each row in the result
      while ((row=mysql_fetch_row(result))!=NULL) {
        printf("Name : %s\t, ID : %d\t, Remark : %s\n",row[0],row[1],row[2]);
      }
      mysql_free_result(result);
      mysql_close(connection);


    return 0;
  }
