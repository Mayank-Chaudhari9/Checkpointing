#include <stdio.h>
#include <sys/time.h>
#include <mysql/mysql.h>
#include "dmtcp.h"

MYSQL *mysql_real_connect()



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
