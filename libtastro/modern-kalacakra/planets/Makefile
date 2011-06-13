wc = wcl386
flags = /c /cc /ms /W3 /3 /fpi /omax /zp1 /bt=NT

vsop.exe: vsop.obj merc87.obj venu87.obj earth87.obj mars87.obj jupi87.obj satu87.obj bcd.obj jd.obj tests.obj
   wlink system nt name vsop file vsop, merc87, venu87, earth87, mars87, jupi87, satu87, bcd, jd, tests

vsop.obj: vsop.c jd.h
  $(wc) $(flags) vsop.c

merc87.obj: merc87.c mercc.dat mercd.dat
  $(wc) $(flags) merc87.c

venu87.obj: venu87.c venuc.dat venud.dat
  $(wc) $(flags) venu87.c

mars87.obj: mars87.c marsc.dat marsd.dat
  $(wc) $(flags) mars87.c

earth87.obj: earth87.c earthc.dat earthd.dat
  $(wc) $(flags) earth87.c

jupi87.obj: jupi87.c jupic.dat jupid.dat
  $(wc) $(flags) jupi87.c

satu87.obj: satu87.c saturnc.dat saturnd.dat
  $(wc) $(flags) satu87.c

jd.obj: jd.c jd.h bcd.ext
  $(wc) $(flags) jd.c

bcd.obj: bcd.c bcd.h
  $(wc) $(flags) bcd.c

tests.obj: tests.c
  $(wc) $(flags) tests.c
