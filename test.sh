if [ $# == 0 ]; then
	echo "Starting new terminal, stderr to `tty`"
	gnome-terminal -e "`dirname $0`/test.sh `tty` $DEBUG";
else
	`dirname $0`/Binaries/Consor 2> $1
fi
