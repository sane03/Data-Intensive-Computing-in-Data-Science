#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 11 15:55:27 2019

@author: 
"""

from mrjob.job import MRJob
from collections import Counter
import string
from operator import itemgetter
import time
import re

WORD_RE = re.compile(r"[\w']+")
punctuations = '''""!()-[]{};:'"\,<>./?@#$%^&*_~'''

c = Counter()

class MRWordFreqCount(MRJob):
 
    def mapper(self, _, line):
        # yield each word in the line
        for word in WORD_RE.findall(line):
            if word in punctuations: 
                word = word.replace(word,"") 
            c[word.lower()] += 1
        sorted(c.items(), key=itemgetter(0))
        yield c,None
            
    def combiner(self, key, value):
        # optimization: sum the words we've seen so far
        for key,value in  c.most_common(10):
            yield key,value      
   
    def reducer(self, key, value):
        for key,value in  c.most_common(10):
            yield key,value


if __name__ == '__main__':
   starttime = time.time()
   MRWordFreqCount.run()
   endtime = time.time()
   duration = endtime-starttime
   print ("Time: ", duration) 
  
    
