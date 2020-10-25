#!/bin/bash
if [ $# != 3 ]
then
    echo "Usage:"
    echo "$0 inputfile outputfile fps"
    exit 1
fi

if [ ! -f ./moviecompile ];
then
    echo "moviecompile program not found!"
    exit 1
fi

mkdir -p vidtmp
ffmpeg -s qvga -an -r $3 vidtmp\/frame-%05d.jpg -i $1 || rm -rf ./vidtmp
if [ $? -eq 0 ]
then
  echo "Successfully created image sequence"
  ./moviecompile vidtmp $2 $3
else
  echo "ffmpeg failed" >&2
fi
