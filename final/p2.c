#include <stdio.h>
#include <sys/time.h>
#include <mysql/mysql.h>
#include "dmtcp.h"

mysql_close(MYSQL *mysql)
{
  printf("control reached in close_plugin \n");
  printf("value of mysql free_me : %d\n",mysql->free_me);
  if (mysql)

    if (mysql->net.vio != 0)
    {

      mysql->status=MYSQL_STATUS_READY;

      mysql->reconnect=0;

    }

    if (mysql->free_me)
        mysql->free_me=0;

  }





DmtcpPluginDescriptor_t close_plugin = {
  "3",
  "3",
  "example",
  "DMTCP",
  "dmtcp@ccs.neu.edu",
  "Example plugin",
  NULL,
  NULL
};

DMTCP_DECL_PLUGIN(close_plugin);
