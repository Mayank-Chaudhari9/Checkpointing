#!/bin/bash

set -m # turn on job control

#This script launches all the restarts in the background.
#Suggestions for editing:
#  1. For those processes executing on the localhost, remove
#     'ssh <hostname> from the start of the line. 
#  2. If using ssh, verify that ssh does not require passwords or other
#     prompts.
#  3. Verify that the dmtcp_restart command is in your path on all hosts,
#     otherwise set the remote_prefix appropriately.
#  4. Verify DMTCP_HOST and DMTCP_PORT match the location of the
#     dmtcp_coordinator. If necessary, add
#     'DMTCP_PORT=<dmtcp_coordinator port>' after 'DMTCP_HOST=<...>'.
#  5. Remove the '&' from a line if that process reads STDIN.
#     If multiple processes read STDIN then prefix the line with
#     'xterm -hold -e' and put '&' at the end of the line.
#  6. Processes on same host can be restarted with single dmtcp_restart
#     command.


check_local()
{
  worker_host=$1
  unset is_local_node
  worker_ip=$(gethostip -d $worker_host 2> /dev/null)
  if [ -z "$worker_ip" ]; then
    worker_ip=$(nslookup $worker_host | grep -A1 'Name:' | grep 'Address:' | sed -e 's/Address://' -e 's/ //' -e 's/	//')
  fi
  if [ -z "$worker_ip" ]; then
    worker_ip=$(getent ahosts $worker_host |grep "^[0-9]\+\.[0-9]\+\.[0-9]\+\.[0-9]\+ *STREAM" | cut -d' ' -f1)
  fi
  if [ -z "$worker_ip" ]; then
    echo Could not find ip-address for $worker_host. Exiting...
    exit 1
  fi
  ifconfig_path=$(which ifconfig)
  if [ -z "$ifconfig_path" ]; then
    ifconfig_path="/sbin/ifconfig"
  fi
  output=$($ifconfig_path -a | grep "inet addr:.*${worker_ip} .*Bcast")
  if [ -n "$output" ]; then
    is_local_node=1
  else
    is_local_node=0
  fi
}


usage_str='USAGE:
  dmtcp_restart_script.sh [OPTIONS]

OPTIONS:
  --host, -h, (environment variable DMTCP_HOST):
      Hostname where dmtcp_coordinator is running
  --port, -p, (environment variable DMTCP_PORT):
      Port where dmtcp_coordinator is running
  --hostfile <arg0> :
      Provide a hostfile (One host per line, "#" indicates comments)
  --ckptdir, -d, (environment variable DMTCP_CHECKPOINT_DIR):
      Directory to store checkpoint images
      (default: use the same directory used in previous checkpoint)
  --restartdir, -d, (environment variable DMTCP_RESTART_DIR):
      Directory to read checkpoint images from
  --tmpdir, -t, (environment variable DMTCP_TMPDIR):
      Directory to store temporary files 
        (default: $TMDPIR/dmtcp-$USER@$HOST or /tmp/dmtcp-$USER@$HOST)
  --no-strict-uid-checking:
      Disable uid checking for the checkpoint image.  This allows the
        checkpoint image to be restarted by a different user than the one
        that create it. (environment variable DMTCP_DISABLE_UID_CHECKING)
  --interval, -i, (environment variable DMTCP_CHECKPOINT_INTERVAL):
      Time in seconds between automatic checkpoints
      (Default: Use pre-checkpoint value)
  --help:
      Print this message and exit.'


ckpt_timestamp="Mon Nov 14 19:02:48 2016"

coord_host=$DMTCP_HOST
if test -z "$DMTCP_HOST"; then
  coord_host=reaper
fi

coord_port=$DMTCP_PORT
if test -z "$DMTCP_PORT"; then
  coord_port=7779
fi

checkpoint_interval=$DMTCP_CHECKPOINT_INTERVAL
if test -z "$DMTCP_CHECKPOINT_INTERVAL"; then
  checkpoint_interval=0
fi
export DMTCP_CHECKPOINT_INTERVAL=${checkpoint_interval}

if [ $# -gt 0 ]; then
  while [ $# -gt 0 ]
  do
    if [ $1 = "--help" ]; then
      echo "$usage_str"
      exit
    elif [ $# -ge 1 ]; then
      case "$1" in 
        --host|-h)
          coord_host="$2"
          shift; shift;;
        --port|-p)
          coord_port="$2"
          shift; shift;;
        --hostfile)
          hostfile="$2"
          if [ ! -f "$hostfile" ]; then
            echo "ERROR: hostfile $hostfile not found"
            exit
          fi
          shift; shift;;
        --restartdir|-d)
          DMTCP_RESTART_DIR=$2
          shift; shift;;
        --ckptdir|-d)
          DMTCP_CKPT_DIR=$2
          shift; shift;;
        --tmpdir|-t)
          DMTCP_TMPDIR=$2
          shift; shift;;
        --no-strict-uid-checking)
          noStrictUidChecking="--no-strict-uid-checking"
          shift;;
        --interval|-i)
          checkpoint_interval=$2
          shift; shift;;
        *)
          echo "$0: unrecognized option '$1'. See correct usage below"
          echo "$usage_str"
          exit;;
      esac
    elif [ $1 = "--help" ]; then
      echo "$usage_str"
      exit
    else
      echo "$0: Incorrect usage.  See correct usage below"
      echo
      echo "$usage_str"
      exit
    fi
  done
fi

dmt_rstr_cmd=/usr/bin/dmtcp_restart
which $dmt_rstr_cmd > /dev/null 2>&1 || dmt_rstr_cmd=dmtcp_restart
which $dmt_rstr_cmd > /dev/null 2>&1 || echo "$0: $dmt_rstr_cmd not found"
which $dmt_rstr_cmd > /dev/null 2>&1 || exit 1

local_prefix=
remote_prefix=
remote_dmt_rstr_cmd=dmtcp_restart
if ! test -z "$remote_prefix"; then
  remote_dmt_rstr_cmd="$remote_prefix/bin/dmtcp_restart"
fi

# Number of hosts in the computation = 1
# Number of processes in the computation = 1

given_ckpt_files=" /home/devil/Desktop/test/test/ckpt_dmtcp1_19d0e6a6c07e7-40000-5829bcf9.dmtcp"

ckpt_files=""
if [ ! -z "$DMTCP_RESTART_DIR" ]; then
  for tmp in $given_ckpt_files; do
    ckpt_files="$DMTCP_RESTART_DIR/$(basename $tmp) $ckpt_files"
  done
else
  ckpt_files=$given_ckpt_files
fi

coordinator_info="--host $coord_host --port $coord_port"
tmpdir=
if [ ! -z "$DMTCP_TMPDIR" ]; then
  tmpdir="--tmpdir $DMTCP_TMPDIR"
fi

ckpt_dir=
if [ ! -z "$DMTCP_CKPT_DIR" ]; then
  ckpt_dir="--ckptdir $DMTCP_CKPT_DIR"
fi

exec $dmt_rstr_cmd $coordinator_info $ckpt_dir \
  $maybejoin --interval "$checkpoint_interval" $tmpdir $noStrictUidChecking \
  $ckpt_files
