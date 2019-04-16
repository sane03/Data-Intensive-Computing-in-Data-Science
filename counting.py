#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 11 15:03:16 2019

@author: 
"""

from mrjob.job import MRJob
import time
import re

WORD_RE = re.compile(r"[\w']+")


class MRWordFreqCount(MRJob):

    def mapper(self, _, line):
        for word in WORD_RE.findall(line):
            yield word.lower(), 1

    def combiner(self, word, counts):
        yield word, sum(counts)

    def reducer(self, word, counts):
        yield word, sum(counts)


if __name__ == '__main__':

   starttime = time.time()
   MRWordFreqCount.run()
   endtime = time.time()
   duration = endtime-starttime
   print ("Time: ", duration) 
