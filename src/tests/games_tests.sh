#!/usr/bin/env bash

OPTIONS="--noconfigfile --quickexit --nowelcomemessage --enable-zxdesktop"

echo "Test Jupiter Ace"
./zesarux $OPTIONS --machine ace extras/media/jupiterace/acevaders/acevaders.ACE

echo "Test Coleco"
./zesarux $OPTIONS --machine coleco "extras/media/colecovision/H.E.R.O (USA).col"

echo "Test CPC"
./zesarux $OPTIONS --machine cpc464 --zoom 1 --zxdesktop-width 512 extras/media/cpc/zynaps.cdt

echo "Test CPC 6128"
./zesarux $OPTIONS --machine cpc6128 --zoom 1 --zxdesktop-width 512 "../../../soft_retro/cpc/Prince of Persia (1990)(Broderbund Software).dsk"

echo "Test PCW"
./zesarux $OPTIONS --machine pcw8256 --zoom 1 --zxdesktop-width 512 extras/media/pcw/Livingstone_Supongo.dsk

echo "Test MSX"
./zesarux $OPTIONS --machine msx1 ../../../soft_retro/msx/Road\ Fighter\ \(1985\)\(Konami\)\[a\]\[RC-730\].rom

echo "Test SG1000"
./zesarux $OPTIONS --machine sg1000 "extras/media/sg1000/Girl's Garden (Japan).sg"

echo "Test Master System"
./zesarux $OPTIONS --machine sms ../../../soft_retro/mastersystem/Sonic\ The\ Hedgehog\ \(USA\,\ Europe\).sms

echo "Test SVI"
./zesarux $OPTIONS --machine svi328 extras/media/spectravideo/force.rom

echo "Test ZX81"
./zesarux $OPTIONS --machine zx81 extras/media/zx81/MAZOGS.P

echo "Test TSConf"
./zesarux $OPTIONS --machine tsconf --zoom 1 --zxdesktop-width 512 extras/media/spectrum/tsconf/fast.spg

echo "Test Inves"
./zesarux $OPTIONS --machine inves ../../../soft_retro/spectrum/LEMMIN48.TAP

echo "Test QL"
./zesarux $OPTIONS --machine ql --zoom 1 --zxdesktop-width 512 --ql-mdv1-root-dir "/Users/cesarhernandez/Documents/ZEsarUX/zesarux/src/my_soft/microbyt/ql/mago_folder/" --enable-ql-mdv-flp --ql-mdv1-read-only --ql-mdv1-enable
