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

    MYSQL_ROW row;
    //MYSQL_RES result;
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


        printf("%s\n",connection->passwd );
        printf("%s\n",connection->host);

        //printf("%d\n",*connection );
     state =mysql_query(connection,"select * from user");
      if (state)
       {
         printf("%s\n",mysql_error(connection));
         return 1;
        }

      printf("MvSQL table data \n");

      //store result before printing it



    MYSQL_RES *result1 = mysql_store_result(connection);
      printf("Rows : %llu\n",mysql_num_rows(result1));

      //process each row in the result
      while ((row=mysql_fetch_row(result1))) {
        printf("Name : %s\t, ID : %s\n",row[0],row[1]);
      }
      mysql_free_result(result1);
      printf("connection before checkpointing%d\n",connection );
      for(int i=0;i<10;i++)
        {
          printf("%d\n",i);
          sleep(1);
        }

      state =mysql_query(connection,"select * from user");
        if (state)
         {
           printf("%s\n",mysql_error(connection));
           return 1;
          }
        //uncomment to test whether the connection structure is being populated correctly or not.
        /*
          printf("database name in test file %s\n", connection->db);
          printf("MvSQL table data \n");
          printf("connection after checkpointing %d\n",connection );
        */


      MYSQL_RES *result = mysql_store_result(connection);
        printf("Rows : %llu\n",mysql_num_rows(result));

        //process each row in the result
        while ((row=mysql_fetch_row(result))) {
          printf("Name : %s\t, ID : %s\n",row[0],row[1]);
        }
        mysql_free_result(result);




      mysql_close(connection);


    return 0;
  }
