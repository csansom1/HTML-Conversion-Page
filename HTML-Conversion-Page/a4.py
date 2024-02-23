#!/usr/bin/python

# Filename: a4.py
# Author: Connor Sansom
# Last Update: 07/10/2016
# Purpose: a4.py is called from listio.c and adds the final HTML tags to the strings it recieves.

import os, sys

q1 = open("q1", "r")
name = q1.readline() [:-1]
sizeString = q1.readline() [:-1]
size = int(sizeString)
strings = []
i = 0
while True:
  str = q1.readline() [:-1]
  if not str:
    break
  strings.append(str)
  i += 1
os.remove("q1")

strings[0] = "<HTML><HEAD><TITLE>%s</TITLE></HEAD><BODY>%s" % (name, strings[0])
strings[i-1] = "%s</BODY></HTML>" % strings[i-1]

if os.path.exists("./files/%s.info" % name):
  info = open("./files/%s.info" % name, "r")
  while True:
    line = info.readline()
    if not line or line == "":
      break
    line = line.replace("\n", "")
    for j in range(i):
      strings[j] = strings[j].replace(line[2:], "<%s>%s</%s>" % (line[0], line[2:], line[0]))
elif os.path.exists("./files/%s.info" % name[:name.rfind(".")]):
  info = open("./files/%s.info" % name[:name.rfind(".")], "r")
  while True:
    line = info.readline()
    if not line:
      break
    line = line.replace("\n", "")
    if not line == "":
      for k in range(i):
        strings[k] = strings[k].replace(line[2:], "<%s>%s</%s>" % (line[0], line[2:], line[0]))


q2 = os.open("./q2", os.O_WRONLY)
for k in range(i):
  os.write(q2, "%d\n%s\n" % (len(strings[k]), strings[k]))
os.close(q2)
