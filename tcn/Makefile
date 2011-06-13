wc = wcl386
flags = /c /cc /ms /W3 /3 /fpi /omax /zp1 /bt=NT

tcn.exe: tcn.obj tc_pcalc.obj t3.obj utils.obj bcd.obj
    wlink system nt name tcn file tcn, tc_pcalc, t3, utils, bcd

tcn.obj: tcn.c tcn.h tc.def bcd.h
        $(wc) $(flags) tcn.c

tc_pcalc.obj: tc_pcalc.c tcn.h tc.def tc.ext bcd.h
        $(wc) $(flags) tc_pcalc.c

t3.obj: t3.c tcn.h tc.def tc.ext bcd.h
        $(wc) $(flags) t3.c

utils.obj: utils.c tcn.h tc.def tc.ext bcd.h
        $(wc) $(flags) utils.c

bcd.obj: bcd.c bcd.h
        $(wc) $(flags) bcd.c
