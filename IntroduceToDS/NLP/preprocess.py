
# coding: utf-8

# In[1]:

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import nltk 
from nltk.tokenize import WordPunctTokenizer




def process(text):
    # ## 分割成词语，去除标点

    # In[3]:

    from nltk.tokenize import RegexpTokenizer
    tokenizer = RegexpTokenizer(r'\w+')
    words = tokenizer.tokenize(text)



    # ## 打标记

    # In[4]:

    tags = nltk.pos_tag(words)



    # ## stemming

    from nltk.stem import WordNetLemmatizer
    wordnet_lemmatizer = WordNetLemmatizer()
    WORD = []

    for word, tag in tags:
         wntag = tag[0].lower()
         wntag = wntag if wntag in ['a', 'r', 'n', 'v','n','s'] else None
         if not wntag:
                 lemma = word
         else:
                 lemma = wordnet_lemmatizer.lemmatize(word, wntag)
         WORD.append(lemma)

    # ## stop word

    # In[7]:

    from nltk.corpus import stopwords
    word_after_stop = []
    for w in WORD:
        if not w in stopwords.words('english'):
            word_after_stop.append(w)
    word_after_stop

    # ## 去除过短的词和spell check（conti.）

    # In[14]:
    clean=""
    for w in word_after_stop:
        if len(w)>=3 and w.isalpha():
            clean = clean + w.lower() + " "
    clean = clean[:-1]
    return clean


# fReview = open("Reviews.txt")
# fProcess = open("process.txt","w")

fReview = open("testReviews.txt")
fProcess = open("testprocess.txt","w")

raw = fReview.readlines()

for text in raw:
    word = process(text) + "\n"
    fProcess.write(word)

fProcess.close()
fReview.close()





