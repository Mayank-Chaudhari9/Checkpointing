#include <stdio.h>
#include <sys/time.h>
#include <mysql/mysql.h>
#include "dmtcp.h"

MYSQL *pconnection,*connection;
char *pserver;
char *puser;
char *ppassword;
char *pdatabase;

// function to trap the call from actual progarm and get the passed paramerter to initialize the global varibles used next time
MYSQL *mysql_real_connect(MYSQL *mysql,const char *host, const char *user, const char *passwd,const char *db,unsigned int port,const char *unix_socket,unsigned long client_flag )
{
  printf("present here\n");
  connection=mysql;
  pconnection=mysql;
  pserver=host;
  puser=user;
  ppassword=passwd;
  pdatabase=db;
  //totest whether the values are being trapped or not
  //printf("%s\n",passwd );
  //NEXT_FNC to transfer control to the actual function
  NEXT_FNC(mysql_real_connect)(connection,host,user,passwd,db,0,NULL,0);
  //uncomment to check whether the connection structure is being populated or not
  //printf(" passwd : %s\n",connection->passwd );
  return connection;
}



static void checkpoint()
{
  //mysql_close(connection);
  printf("---------#####  the %s plugin is being called before checkpinting. ##### ------------\n",__FILE__);
}

static void resume()
{
  pconnection=NULL;
  mysql_real_connect(pconnection,pserver,puser,ppassword,pdatabase,0,NULL,0);
  printf("------#### The %s plugin has no been checkpointed. #### ------------\n", __FILE__);
}

static DmtcpBarrier barriers[]={
  {DMTCP_GLOBAL_BARRIER_PRE_CKPT,checkpoint,
  "checkpoint"},
  {DMTCP_GLOBAL_BARRIER_RESUME,resume,"resume"}
};

DmtcpPluginDescriptor_t plugindb={
  "1.0",
  "1.0",
  "plugindb",
  "DMTCP",
  "Database C Plugin",
  DMTCP_DECL_BARRIERS(barriers),
  NULL
};

DMTCP_DECL_PLUGIN(plugindb);
