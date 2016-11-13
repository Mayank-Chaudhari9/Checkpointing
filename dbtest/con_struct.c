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

      printf("host : %s\n",connection->host );
      printf("user : %s\n",connection->user );
      printf("passwd : %s\n",connection->passwd );
      printf("database : %s\n",connection->db );
      printf("port : %u\n",connection->port );
      printf("unix_socket : %s\n",connection->unix_socket );
      printf("client_flag : %lu\n",connection->client_flag );

      mysql_close(connection);


    return 0;
  }
