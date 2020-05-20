#!/bin/bash

now=($( date '+%F %H:%M'))
DATE=${now[0]}
TIME=${now[1]}
TIME_STAMP=${DATE}_${TIME}
HOME=.

$HOME/ranmaze2w.x $1 $2 $HOME/_TEMPMAZE

if [ -f $HOME/_TEMPMAZE.1 ] ; then
    $HOME/solvemaze.x $HOME/_TEMPMAZE.1
    $HOME/printmaze.x -h10 -w15 < $HOME/_TEMPMAZE.1 > $HOME/_TEMPMAZE.ps
else
    echo "File $HOME/_TEMPMAZE.1 does not exist";
    exit 1;
fi

if [ -f  $HOME/_TEMPMAZE.1.dfs ] ; then
    echo "Printmaze: Writing encapsulated postscript"
    $HOME/printmaze.x -h10 -w15 < $HOME/_TEMPMAZE.1.dfs > $HOME/_TEMPMAZE.dfs.ps
else
    echo "File $HOME/_TEMPMAZE.1.dfs does not exist";
    exit 1;
fi

if [ -f  $HOME/_TEMPMAZE.ps ] ; then
    pstopnm -stdout -yborder=0 -xborder=0 -portrait -xsize=1000 $HOME/_TEMPMAZE.ps | pnmtopng > $HOME/maze.png
    chmod 644 $HOME/maze.png
else
    echo "File $HOME/_TEMPMAZE.ps does not exist";
    exit 1;
fi

if [ -f  $HOME/_TEMPMAZE.dfs.ps ] ; then
    pstopnm -stdout -yborder=0 -xborder=0 -portrait -xsize=1000 $HOME/_TEMPMAZE.dfs.ps | pnmtopng > $HOME/maze.dfs.png
    chmod 644 $HOME/maze.dfs.png
else
    echo "File $HOME/_TEMPMAZE.dfs.ps does not exist";
    exit 1;
fi

echo "maze graphic in maze.png"
echo "solved maze graphic in maze.dfs.png"
