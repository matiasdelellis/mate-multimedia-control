#!/bin/sh

echo "Running Mate Developer Tools..."

mate-autogen $@
if [ $? -ne 0 ]; then
  echo "mate-autogen Failed"
fi
