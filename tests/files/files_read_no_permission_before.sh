#!/bin/bash

mkdir -p files
echo "Test" > files/no_permission_file
chmod -r files/no_permission_file