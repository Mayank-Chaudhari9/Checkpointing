/**
  compile with following command
    gcc -o test $(mysql_config --cflags) simpledb.c $(mysql_config --libs)
*/
#include <unistd.h>
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
    int i=0;
    connection = mysql_init(NULL);
    if(!mysql_real_connect(connection,server,user,password,database,0,NULL,0))
    {
      fprintf(stderr, "%s\n",mysql_error(connection));
      exit(1);
    }
    else
      printf("connection successful\n" );
      for(i=0;i<10;i++)
      {
        printf("%d\n",i);
        sleep(1);
      }
        

      if (mysql_query(connection, "show tables"))
      {
        fprintf(stderr, "%s\n", mysql_error(connection));
        exit(1);
      }
      // uncomment to test test whether the status of above code is 0/1
      //else
        //printf("something is wrong\n");

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
