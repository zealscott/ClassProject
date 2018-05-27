# gensim modules
import warnings
warnings.filterwarnings(action='ignore', category=UserWarning, module='gensim')

from gensim import utils
from gensim.models.doc2vec import TaggedDocument
from gensim.models import Doc2Vec
# numpy
import numpy as np
# random
from random import shuffle
# classifier
from sklearn.linear_model import LogisticRegression


class LabeledLineSentence(object):
    """
    tag each line sentence from multi-files
    the document should be one line with space
    """

    def __init__(self, sources):
        """
        the source should be ``{filename:tag_prefix}``
        """
        self.sources = sources

        flipped = {}

        # make sure that keys are unique
        for key, value in sources.items():
            if value not in flipped:
                flipped[value] = [key]
            else:
                raise Exception('Non-unique prefix encountered')

    def __iter__(self):
        for source, prefix in self.sources.items():
            with utils.smart_open(source) as fin:
                for item_no, line in enumerate(fin):
                    yield TaggedDocument(utils.to_unicode(line).split(), [prefix + '_%s' % item_no])

    def to_array(self):
        """
        convert sentence to taggedDocument for train
        """
        self.sentences = []
        for source, prefix in self.sources.items():
            with utils.smart_open(source) as fin:
                for item_no, line in enumerate(fin):
                    self.sentences.append(TaggedDocument(
                        utils.to_unicode(line).split(), [prefix + '_%s' % item_no]))
        return self.sentences

    def shuffle(self):
        """
        random shuffle sentences
        """
        shuffle(self.sentences)
        return self.sentences


class Train(object):
    def __init__(self, sources):
        """
        the source should be dict: ``{filename:tag_prefix}``
        """
        self.sources = sources
        self.sentences = LabeledLineSentence(self.sources)

    def process(self, vector_size=100, min_count=1, window=10, epoch=50):
        """
        Train Doc2Vec model  
        """
        self.model = Doc2Vec(min_count=1, window=10, size=100, sample=1e-4, negative=5, workers=7)

        self.model.build_vocab(self.sentences.to_array())

        for i in range(epoch):
            self.model.train(self.sentences.shuffle(),
                             total_examples=self.model.corpus_count, epochs=self.model.iter)
        
        print("Finish Trainning")

    def save(self, filename):
        """
        save model to disk  
        """
        self.model.save(filename)

        self.model = Doc2Vec.load(filename)

        print("save model to" + filename)

    def similar(self, word):
        """
        return the similar words  
        """
        return self.model.wv.most_similar(word)



sources = {'CleanData\\test.txt': 'TEST', 'CleanData\\train-neg.txt': 'TRAIN_NEG',
           'CleanData\\train-pos.txt': 'TRAIN_POS', 'CleanData\\train-unsup.txt': 'TRAIN_UNS'}

test = Train(sources)
test.process()
test.save('.\\Model\\imdb_noW2V.d2v')

