from builtins import range
import numpy as np
from random import shuffle
from past.builtins import xrange

def svm_loss_naive(W, X, y, reg):
    """
    Structured SVM loss function, naive implementation (with loops).

    Inputs have dimension D, there are C classes, and we operate on minibatches
    of N examples.

    Inputs:
    - W: A numpy array of shape (D, C) containing weights.
    - X: A numpy array of shape (N, D) containing a minibatch of data.
    - y: A numpy array of shape (N,) containing training labels; y[i] = c means
      that X[i] has label c, where 0 <= c < C.
    - reg: (float) regularization strength

    Returns a tuple of:
    - loss as single float
    - gradient with respect to weights W; an array of same shape as W
    """
    dW = np.zeros(W.shape) # initialize the gradient as zero

    # compute the loss and the gradient
    num_classes = W.shape[1]
    num_train = X.shape[0]
    loss = 0.0
    for i in range(num_train):
        scores = X[i].dot(W)
        correct_class_score = scores[y[i]]
        for j in range(num_classes):
            if j == y[i]:
                continue
            margin = scores[j] - correct_class_score + 1 # note delta = 1
            if margin > 0:
                loss += margin
                dW[:,j] += X[i] # dW计算
                dW[:,y[i]] += -X[i] # dW计算

    # Right now the loss is a sum over all training examples, but we want it
    # to be an average instead so we divide by num_train.
    loss /= num_train
    dW /= num_train

    # Add regularization to the loss.
    # loss += reg * np.sum(W * W)
    loss += 0.5 * reg * np.sum(W * W)

    #############################################################################
    # TODO：
    # 计算损失函数的梯度并将其存储为dW。
    # 与其先计算损失再计算梯度，还不如在计算损失的同时计算梯度更简单。
    # 因此，您可能需要修改上面的一些代码来计算梯度。
    #############################################################################
    # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
    
    dW += reg * W

    # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
    
    return loss, dW



def svm_loss_vectorized(W, X, y, reg):
    """
    Structured SVM loss function, vectorized implementation.

    Inputs and outputs are the same as svm_loss_naive.
    """
    loss = 0.0
    dW = np.zeros(W.shape) # initialize the gradient as zero
    C = W.shape[1]
    N = X.shape[0]

    #############################################################################
    # TODO: 
    # 实现一个向量化SVM损失计算方法,并将结果存储到loss中
    #############################################################################
    
    # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****
    
    scores = np.dot(X, W)    # (N, C)
    correct_scores = scores[np.arange(N), y]  # (N, )
    margins = np.maximum(scores - correct_scores.reshape(N, 1) + 1.0, 0)  # (N, C)
    margins[np.arange(N), y] = 0
    loss = np.sum(margins) / N
    loss += 0.5 * reg * np.sum(W * W)

    # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

    #############################################################################
    # TODO:                                                              
    # 实现一个向量化的梯度计算方法,并将结果存储到dW中                                
    #                                                                           
    # 提示:与其从头计算梯度,不如利用一些计算loss时的中间变量                                    
    #############################################################################
    # *****START OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

    dscores = np.zeros_like(scores)  # (N, C)
    dscores[margins > 0] = 1  
    dscores[np.arange(N), y] -= np.sum(dscores, axis=1)   #  (N, 1) = (N, 1)

    dW = np.dot(X.T, dscores) 
    dW /= N
    dW += reg * W 

    # *****END OF YOUR CODE (DO NOT DELETE/MODIFY THIS LINE)*****

    return loss, dW
