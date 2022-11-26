#!/bin/sh

mkdir -p files
echo "Test" > files/no_permission_file
chmod a-w files/no_permission_file