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

      if (mysql_query(connection, "show tables"))
      {
        fprintf(stderr, "%s\n", mysql_error(connection));
        exit(1);
      }

      result = mysql_use_result(connection);

      printf("MvSQL tables in database \n");
      while ((row=mysql_fetch_row(result))!=NULL) {
        /* code */
          printf("%s\n",row[0] );
      }

      mysql_free_result(result);
      mysql_close(connection);


    return 0;
  }
