#include <stdio.h>
#include <sys/time.h>
#include "dmtcp.h"
#include "config.h"

void print_time() {
  struct timeval val;
  gettimeofday(&val, NULL);
  printf("%ld %ld", (long)val.tv_sec, (long)val.tv_usec);
}

unsigned int sleep(unsigned int seconds) {
  printf("sleep1: "); print_time(); printf(" ... ");
  unsigned int result = NEXT_FNC(sleep)(seconds);
  print_time(); printf("\n");

  return result;
}


void dmtcp_event_hook(DmtcpEvent_t event, DmtcpEventData_t *data)
{
  switch(event)
  {
    case DMTCP_EVENT_WRITE_CKPT:
      printf(" The plugin %s is being called before checkpointing\n",__FILE__ );
      break;

    case DMTCP_EVENT_RESUME:
      printf("The plugin %s has been checkpointed\n",__FILE__ );
      break;

    default:
    ;

    }
}
DMTCP_NEXT_EVENT_HOOK(event,data);
