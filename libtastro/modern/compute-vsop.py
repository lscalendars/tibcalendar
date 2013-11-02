#!/usr/bin/python
# coding=utf-8
#
# Python vsop optimized file generator.
#
# Copyright (c) 2013 Elie Roux <elie.roux@telecom-bretagne.eu>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#
# This script converts files from ftp://ftp.imcce.fr/pub/ephem/planets/vsop87/
# into useable and configurable C files. It's been used to compile the files
# in tibastro, but won't be useful for average users. It is only provided for
# people who might need it for other purposes.
#

# This script should be called with following arguments:
#   compute-vsop.py infile
# with infile the file you want to convert, coming from the official
# dataset (ftp://ftp.imcce.fr/pub/ephem/planets/vsop87/)
# it outputs the result on stdout, so you should redirect it.

import re
import sys

# regexp to match a normal line, to be searched or matched starting at 48th char
vsoplineregex = re.compile("\s?-?\d+\.\d+\s+-?\d+\.\d+\s+(-?\d+\.\d+)\s+(-?\d+\.\d+)\s+(-?\d+\.\d+)")

# correspondance between file extensions and planet:
fecor = {"ear":"earth", 
  "jup":"jupiter",
  "mar":"mars",
  "mer":"mercury",
  "nep":"neptune",
  "sat":"saturn",
  "ura":"uranus",
  "ven":"venus",
  "emb":"emb"}

def main():
  if len(sys.argv) != 2:
    print """This script should be used with exactly one argument which is the name of the
  file you want to convert.
  """
    exit(1)
  try:
    f = open(sys.argv[1])
    # we get the planet it is for
    planet = fecor[sys.argv[1].split(".")[-1]]
    vsop_letter = sys.argv[1][6].lower()
    fout = open("vsop87-%c-%s.c" % (vsop_letter, planet), "w")
    lines = f.readlines()
    previous_numbers = None
    functionnumber = 0
    fout.write("#include \"vsop.h\"\n")
    for line in lines:
      # First the case of a "normal" line
      # we don't care about things before column 48
      m = vsoplineregex.match(line,48)
      if m:
        if (previous_numbers):
          fout.write("  twoops(_(%s), _(%s), _(%s),\n         _(%s), _(%s), _(%s));\n" % (previous_numbers[0], previous_numbers[1], previous_numbers[2], m.group(1), m.group(2), m.group(3)))
          previous_numbers = None
        else:
          previous_numbers = [m.group(1), m.group(2), m.group(3)]
      #else, it should be a head line, the only interesting thing is the 42nd and 60th character:
      elif len(line) > 60:
        previousfunctionnumber = functionnumber
        functionnumber = int(line[59])
        variablenumber = int(line[41])
        print "fun : %d, var : %d" % (functionnumber, variablenumber)
        if (previous_numbers):
          fout.write("  oneop( _(%s), _(%s), _(%s));\n" % (previous_numbers[0], previous_numbers[1], previous_numbers[2]))
        if (functionnumber != 0 or variablenumber != 1):
          fout.write("  end();\n}\n")
        if (variablenumber != 1 and functionnumber == 0):
          print_sum_function(previousfunctionnumber, variablenumber -1, planet, vsop_letter, fout)
        fout.write("\ncoord_t vsop87_%c_%s_%d_%d (time_t t) {\n  initialize();\n" % (vsop_letter, planet, variablenumber, functionnumber))
      # if it's none of the two, we just ignore the line  
    if (previous_numbers):
      fout.write("  oneop( _(%s), _(%s), _(%s));\n" % (previous_numbers[0], previous_numbers[1], previous_numbers[2]))
    fout.write("  end();\n}\n")
    print_sum_function(functionnumber, 3, planet, vsop_letter, fout)
    f.close()
    fout.close()
  except IOError:
    print "Error: cannot open file %s" % sys.argv[1]

def print_sum_function(factor, variable_number, planet, vsop_letter, fout):
  common_str = "vsop87_%c_%s_%d" % (vsop_letter, planet, variable_number)
  if factor == 5:
    fout.write("""
coord_t %s (time_t t) {
  return ((((%s_5(t) *t + %s_4(t)) * t + %s_3(t)) *t + %s_2(t)) * t + %s_1(t)) *t + %s_0(t);
}
""" % (common_str, common_str, common_str, common_str, common_str, common_str, common_str))
  elif factor == 4:
    fout.write("""
coord_t %s (time_t t) {
  return (((%s_4(t) * t + %s_3(t)) *t + %s_2(t)) * t + %s_1(t)) *t + %s_0(t);
}
""" % (common_str, common_str, common_str, common_str, common_str, common_str))
  elif factor == 3:
    fout.write("""
coord_t %s (time_t t) {
  return ((%s_3(t) *t + %s_2(t)) * t + %s_1(t)) *t + %s_0(t);
}
""" % (common_str, common_str, common_str, common_str, common_str))
  else:
    print "error!!! Factor %d not taken into account" % factor
    exit(1)

main()
