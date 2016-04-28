#!/bin/sh

rm -rf ../server_cpp/src/proto/* ../client_cpp/src/proto/*

protoc -I. --cpp_out=../server_cpp/src/proto ./*.proto
protoc -I. --cpp_out=../client_cpp/src/proto ./*.proto
