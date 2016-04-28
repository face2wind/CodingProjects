#!/bin/bash
libPath=./libs/
sourcePath=./src/
outputPath=./../bin_zh_CN/
moduleOutputPath="${outputPath}modules/"
linkReport=""
DEBUG="false"
count=0

#build one module
buildAs()
{
    let count++
    curOutputPath=""
    sourceName=""
    if [ "$1" = "Packer" ] ; then
	curOutputPath=$outputPath
	sourceName="${sourcePath}${1}.as"
	linkReport=""
    elif [ "$1" = "webCombatLoader" ] ; then
	curOutputPath=$outputPath
	sourceName="${sourcePath}${1}.as"
	linkReport="-link-report=webCombatLoader.xml"
    elif [ "$1" = "ReportCombat" ] ; then
	curOutputPath=$moduleOutputPath
	sourceName="${sourcePath}modules/${1}.as"
	linkReport="-load-externs=webCombatLoader.xml"
    elif [ "$1" = "loader" ] ; then
	curOutputPath=$outputPath
	linkReport="-link-report=loaderReport.xml"
	sourceName="${sourcePath}${1}.as"
    elif [ "$1" = "main" ] ; then
	curOutputPath=$outputPath
	linkReport="-link-report=mainReport.xml -load-externs=loaderReport.xml"
	sourceName="${sourcePath}${1}.as"
    #createRole and selectRole module can only optimize with loader
    elif [ "$1" = "CreateRole" ] ; then
	curOutputPath=$moduleOutputPath
	linkReport="-load-externs=loaderReport.xml"
	sourceName="${sourcePath}modules/${1}.as"
    elif [ "$1" = "SelectRole" ] ; then
	curOutputPath=$moduleOutputPath
	linkReport="-load-externs=loaderReport.xml"
	sourceName="${sourcePath}modules/${1}.as"
    #other modlues optimize with loader and main
    else
	curOutputPath=$moduleOutputPath
	linkReport="-load-externs=mainReport.xml -load-externs=loaderReport.xml"
	sourceName="${sourcePath}modules/${1}.as"
    fi
    outputName="${1}.swf"
    echo "Compiling ${1} module  ======================== [ ${2}/${3} ]"
    echo "mxmlc -debug=$DEBUG $linkReport -static-link-runtime-shared-libraries=true -accessible=true -show-actionscript-warnings=true -strict=true -use-network=true -incremental=false -optimize=true -source-path $sourcePath -library-path $libPath -output \"${curOutputPath}${outputName}\" $sourceName"
    mxmlc -debug=$DEBUG $linkReport -static-link-runtime-shared-libraries=true -accessible=true -show-actionscript-warnings=true -strict=true -use-network=true -incremental=false -optimize=true -source-path $sourcePath -library-path $libPath -output "${curOutputPath}${outputName}" $sourceName
    echo "======================== Build $outputName complete !"
    echo ""
}

buildAll()
{
    cd "${outputPath}/modules/"
    rm *.swf -rf
    cd -
	modulesNum=`ls "${sourcePath}modules" |wc -l`
	modulesNum=$((modulesNum+4))
	curNum=1
    buildAs loader $curNum $modulesNum
	curNum=$((curNum+1))
    buildAs main $curNum $modulesNum
	curNum=$((curNum+1))
    buildAs Packer $curNum $modulesNum
	curNum=$((curNum+1))
    buildAs webCombatLoader $curNum $modulesNum
    for i in `ls "${sourcePath}modules" |grep "as$"`
    do
	curNum=$((curNum+1))
	buildAs `echo $i|awk -F\. '{print $1}'` $curNum $modulesNum
    done
}

#start build
doRunScript()
{
    if [[ 0 != $# ]] ; then
	#has parameters, compile module list
	totalNum=`echo $*|awk -F\  '{print NF}'`
	curNum=0
	for i in $*
	do
		curNum=$((curNum+1))
	    buildAs $i $curNum $totalNum
	done
    else
	#no parameters , compile all modules
	buildAll
    fi
}

#show help messages
showHelp()
{
    echo ""
    echo "命令用法："
    echo "./autobuild.sh （编译所有模块）"
    echo "./autobuild.sh 模块名 （编译指定模块）"
    echo ""
    echo "参数："
    echo "-h/-help 显示当前帮助文档"
    echo "-d/-debug 编译成可调试版本（默认为非DEBUG版本）"
    #	echo ""
    #	echo ""
    #	echo ""
}

#first find main parameters
allParam=$*

#-debug param
#====================================================================
hasParamNum=`echo $allParam|grep "\-debug"|wc -l`
if [ "$hasParamNum" = "1" ] ; then
    DEBUG="true"
    allParam=`echo $allParam| sed -e 's/\-debug//g'`
fi
hasParamNum=`echo $allParam|grep "\-d"|wc -l`
if [ "$hasParamNum" = "1" ] ; then
    DEBUG="true"
    allParam=`echo $allParam| sed -e 's/\-d//g'`
fi

#-help param (help will stop all build ,just show help messages )
#====================================================================
hasParamNum=`echo $allParam|grep "\-h"|wc -l`
if [ "$hasParamNum" = "1" ] ; then
    showHelp
    allParam=`echo $allParam| sed -e 's/\-h//g'`
    exit
fi
hasParamNum=`echo $allParam|grep "\-help"|wc -l`
if [ "$hasParamNum" = "1" ] ; then
    showHelp
    allParam=`echo $allParam| sed -e 's/\-help//g'`
    exit
fi


doRunScript $allParam
echo "total ${count} modules compile complete!"
