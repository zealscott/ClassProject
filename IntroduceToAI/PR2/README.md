

















# 预处理

- 注意`read_csv`的时候要加`quoting = 3`
  - [pandas read_csv](http://pandas.pydata.org/pandas-docs/stable/generated/pandas.read_csv.html)
- 不使用`stopwords`
- 不移除数字



# Usage

1. `run text Pre Process`

   ```python
   import PreText
   Data = TextPreProcess(DataPath, OutPath)
   Data.process()
   ```

2. ​

   ​

# toturial

- 利用Doc2Vec的改进

  - [Sentiment Analysis Using Doc2Vec](http://linanqiu.github.io/2015/10/07/word2vec-sentiment/)

- Kaggle针对Word2vector

  - [kaggle tutorial](https://www.kaggle.com/c/word2vec-nlp-tutorial#part-2-word-vectors)

- Gensim发明者写的

  - [Doc2vec tutorial](https://rare-technologies.com/doc2vec-tutorial)

- Github上的一篇，但没太看懂

  - [Gensim Doc2vec Tutorial on the IMDB Sentiment Dataset](https://github.com/RaRe-Technologies/gensim/blob/develop/docs/notebooks/doc2vec-IMDB.ipynb)

- Kaggle上的讨论

  - [Using Doc2Vec from gensim.](https://www.kaggle.com/c/word2vec-nlp-tutorial/discussion/12287)

- gensim官方参数文档

  - [Deep learning with paragraph2vec](https://radimrehurek.com/gensim/models/doc2vec.html)

- 关于参数`negative sampling`

  - [Negative Sampling](http://mccormickml.com/2017/01/11/word2vec-tutorial-part-2-negative-sampling/)

  ​



# Reference

1. [正则提取出HTML正文](https://blog.csdn.net/pingzi1990/article/details/41698331)
2. [replacer](https://github.com/PacktPublishing/Natural-Language-Processing-Python-and-NLTK/blob/master/Module%203/__pycache__/replacers.py)
3. [RegexReplacer](https://groups.google.com/forum/#!topic/nltk-users/BVelLz2UNww)
4. [词干提取与词性还原](https://blog.csdn.net/march_on/article/details/8935462)
5. [pos tag type](https://stackoverflow.com/questions/15388831/what-are-all-possible-pos-tags-of-nltk?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa)
6. [Stemming and Lemmatization](https://www.jianshu.com/p/22be6550c18b)
7. [IMDB电影评论集](http://ai.stanford.edu/~amaas/data/sentiment/)
8. ​