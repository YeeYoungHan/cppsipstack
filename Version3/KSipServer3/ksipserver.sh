#!/bin/sh
#------------------------------------------------------------------------------
# KSipServer start/stop script 
#------------------------------------------------------------------------------
# programmer : yee young han ( websearch@naver.com )
#------------------------------------------------------------------------------
# start date : 2012/09/14
#------------------------------------------------------------------------------

# program directory
root_dir=/home/websearch/OpenProject/CppSipStack/KSipServer
program="KSipServer.exe"
program_list="$program"
program_arg="$root_dir/KSipServer.xml"
program_name="KSipServer"
logfile=$root_dir/ksipserverdog.log
this_script=ksipserver.sh

# get process pid
getpids(){
	pids=`ps -Af | grep "$1" | grep -v grep | sed -e 's/^........ *//' -e 's/ .*//'`
	echo "$pids"
}

# start function
start() {
	# Start daemons.
	if [ -n "`getpids $program`" ]; then
		echo "$program_name: already running"
  else
    echo "$program_name: Starting..."
    ulimit -c unlimited
    ulimit -n 65535
    $root_dir/$program $program_arg
	fi
}

# stop function
stop() {
	# Stop daemons.
  echo "$program_name : Stopping... "
  killall $program 
}

# status function
status() {
	pids=`getpids $program`
	if [ "$pids" != "" ]; then
		echo "$program_name: running"
  else
		echo "$program_name: not running"
	fi
	
	pids=`getpids "$this_script watchdog"`
	if [ "$pids" != "" ]; then
		echo "watchdog: running"
  else
		echo "watchdog: not running"
	fi
}

start_program(){
	d=`date +%Y/%m/%d_%H:%M:%S`

  echo "[$d] $1: Starting..." >> $logfile
  ulimit -c unlimited
  ulimit -n 65535
  $root_dir/$1 $program_arg
}

watchdog(){
	# 최소 실행시 프로세스가 정상적으로 동작할 수 있도록 일정시간 대기한다.
	echo "watchdog: Starting..."
	sleep 60
	while [ 1 ]; do
	
		# 프로세스가 존재하지 않는 경우, 프로세스를 다시 시작하여 준다.
		for program in $program_list
		do
			if [ -f "$root_dir/$program" ]; then
				if [ "`getpids $program`" = "" ]; then
			    start_program $program
			  fi
			else
				echo "[error] $root_dir$program is not exist."
			fi
		done
		
		sleep 10
	done
}

# watchdog start
watchdog_start(){
	$root_dir/$this_script watchdog &
}

# watchdog stop
watchdog_stop(){
	pids=`getpids "$this_script watchdog"`
	for p in $pids
	do
		kill $p
	done
	echo "watchdog : Stopped"
}

# main function.
case "$1" in
	start)
		start
		watchdog_start
		;;
	stop)
		watchdog_stop
		stop
		;;
	status)
		status
		;;
	restart)
		stop
		sleep 1
		start
		;;
	watchdog)
		watchdog
		;;
	*)
		echo $"Usage: $0 {start|stop|status|restart}"
		exit 1
		;;
esac

