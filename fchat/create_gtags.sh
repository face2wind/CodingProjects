#!/bin/sh

script_path=$(dirname $0)
script_path="$(pwd)/${script_path}"
#echo $script_path

echo "do gtags and htags on :"

cd "${script_path}/server_cpp"
pwd
gtags && htags

cd "${script_path}/client_cpp"
pwd
gtags && htags
