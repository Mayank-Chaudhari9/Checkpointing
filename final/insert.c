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
    int state,id=3015;
    char query[100],name[5]="test",remark[8]="student";
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

      printf("inserting into users \n");
      /*
      sprintf(query,"INSERT INTO USERS VALUES('%s',%d,'%s')",name,id,remark);
      printf("%s\n",query);
      //if(mysql_query(connection,query)==0)
        //printf("values updated successfully\n");
        */
      mysql_query(connection, "INSERT INTO users VALUES('mayank1',12345,'remark')");


      mysql_free_result(result);
      mysql_close(connection);


    return 0;
  }
