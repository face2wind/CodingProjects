#!/bin/sh
echo "input your mysql password:"
mysql -uroot -p < tables.sql
