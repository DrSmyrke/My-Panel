if [ $1 = "--version" ]; then
	if [ -f "src/version.h" ]; then
		cat "src/version.h" | awk '{print $3}'
	else
		echo "N/A"
	fi
	exit 0
fi

cd src
qmake src.pro
make
cd ..
