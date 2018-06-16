import math

def is_even(x):
	return x % 2 == 0

def is_odd(x):
	return not is_even(x)

def norm(x): 
    return math.sqrt(sum(i**2 for i in x))

def is_neighbor(X, V):
	return any([abs(X[0] - Y[0]) < 2 and abs(X[1] - Y[1]) < 2 and (X != Y) for Y in V])