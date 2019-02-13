#!/bin/bash

GAMESS_PATH="/home/mateusz/gamessSTO"

HERE=$(dirname $(readlink -f $0))
TMP="$HERE/tmp_data"

cp inps/h2_R0.inp $GAMESS_PATH
cd $GAMESS_PATH
./rungms h2_R0.inp > log.out

mv c.dat $TMP
mv e.dat $TMP
mv ci.dat $TMP
mv log.out $TMP

cd $TMP

grep 'STATE   1  ENERGY=' log.out > en.dat
