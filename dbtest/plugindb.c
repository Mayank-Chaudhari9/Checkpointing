#include <stdio.h>
#include <sys/time.h>
#include <mysql/mysql.h>
#include "dmtcp.h"

MYSQL *connection;
char *server;
char *user;
char *password;
char *database;


MYSQL *mysql_real_connect(MYSQL *mysql,const char *host, const char *user, const char *passwd,const char *db,unsigned int port,const char *unix_socket,unsigned long client_flag )
{
  
}



static void checkpint()
{
  printf("---------#####  the %s plugin is being called before checkpinting. ##### ------------\n",__FILE__);
}

static void resume()
{
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
