#include <stdio.h>
#include <sys/time.h>
#include <mysql/mysql.h>
#include "dmtcp.h"
//#include "config.h"

MYSQL_RES *result;
MYSQL_ROW row;
MYSQL *pconnection=NULL,*connection;
char *pserver=NULL;
char *puser=NULL;
char *ppassword=NULL;
char *pdatabase;
unsigned int pport;
const char *punix_socket="/var/run/mysqld/mysqld.sock";
unsigned long pclient_flag;

// function to trap the call from actual progarm and get the passed paramerter to initialize the global varibles used next time
MYSQL *mysql_real_connect(MYSQL *mysql,const char *host, const char *user, const char *passwd,const char *db,unsigned int port,const char *unix_socket,unsigned long client_flag )
{

  printf("value of mysql free_me : %d\n",mysql->free_me);

  printf("present here\n");
  connection=mysql;


  pconnection=mysql;

  //uncomment to check whether the connection structure is being populated or not
  printf("control reached here \n");
  pserver=host;
  ppassword=passwd;
  pdatabase=db;
  pport=port;
  punix_socket=unix_socket;
  pclient_flag=client_flag;
  puser=user;
  //to test whether the values are being trapped or not
  printf("%d\n",connection );
  printf(" passwd : %s\n",ppassword );
  printf(" passwd : %s\n",passwd );
  printf(" database : %s\n",pdatabase );
  printf(" server : %s\n",pserver );
  printf(" user : %s\n",puser);

  printf("control reached here \n");

  NEXT_FNC(mysql_real_connect)(mysql,host,user,passwd,db,pport,NULL,0);
  printf("value of mysql free_me : %d\n",mysql->free_me);
  return connection;
}

static void example_event_hook(DmtcpEvent_t event, DmtcpEventData_t *data)
{
  /* NOTE:  See warning in plugin/README about calls to printf here. */
  switch (event) {
  case DMTCP_EVENT_INIT:
    printf("The plugin containing %s has been initialized.\n", __FILE__);
    break;
  case DMTCP_EVENT_EXIT:
    printf("The plugin is being called before exiting.\n");
    break;

  default:
    break;
  }
}

static void checkpoint()
{
  printf("\n*** The plugin is being called before checkpointing. ***\n");
  printf("###---------- application closing database connection --------------##########\n");
  //connection->free_me=0;
  mysql_close(connection);
  printf("Connection closed \n");
}

static void resume()
{
  printf("*** The application has now been checkpointed. ***\n");
  printf("###---------- application creating database connection --------------##########\n" );
  connection = mysql_init(pconnection);

  printf("%d\n",connection );

  mysql_real_connect(connection,pserver, puser, ppassword,pdatabase, pport, NULL, 0);

}

static void restart()
{
  printf("The application is now restarting from a checkpoint.\n");
  printf("###---------- application creating database connection --------------##########\n" );
  connection = mysql_init(pconnection);
  mysql_real_connect(connection,pserver, puser, ppassword,pdatabase,pport,NULL,0);
}

static DmtcpBarrier barriers[] = {
  {DMTCP_GLOBAL_BARRIER_PRE_CKPT, checkpoint, "checkpoint"},
  {DMTCP_GLOBAL_BARRIER_RESUME, resume, "resume"},
  {DMTCP_GLOBAL_BARRIER_RESTART, restart, "restart"}
};

DmtcpPluginDescriptor_t example_plugin = {
  "3",
  "3",
  "example",
  "DMTCP",
  "dmtcp@ccs.neu.edu",
  "Example plugin",
  DMTCP_DECL_BARRIERS(barriers),
  example_event_hook
};

DMTCP_DECL_PLUGIN(example_plugin);
