if [ $# == 0 ]; then
	echo "Starting new terminal"
	gnome-terminal -e "`dirname $0`/test-debug.sh `tty`";
	sleep 0.25
	gdb "`dirname $0`/Binaries/Consor" -ev "run > `cat /tmp/dbgterm`"
else
	echo "`tty`" > /tmp/dbgterm
	cat /dev/zero
fi
