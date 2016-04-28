#!/bin/sh

allLine=0
getFileLine()
{
    tmp=`cat $1 | wc -l`
    echo "[${tmp}] - $1"
    allLine=$((allLine+tmp))
}

getPathFileLine()
{
    for i in `find $1|grep "pp$"|grep -v build`
    do
	getFileLine ${i}
    done
}

for i in $* ; do getPathFileLine $i ; done

echo ""
echo "total line : ${allLine}"
