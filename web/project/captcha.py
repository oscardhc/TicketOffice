from sympy import *
from random import *
import operator

init_printing(use_latex='mathjax')
x = Symbol('x')
opSet = [operator.add, operator.sub, operator.mul, operator.truediv]

def generate():
    okSet = [x, (x + randint(1,3)) ** 2, sqrt(x + randint(1,3)), x + 2, x ** 2, sin(x), cos(x), sqrt(x**2 + randint(1,3))]
    i = okSet[randint(0, len(okSet) - 1)]
    j = okSet[randint(0, len(okSet) - 1)]
    k = okSet[randint(0, len(okSet) - 1)]
    if randint(0, 3) == 0 :
        i = exp(i)
    cu = i * randint(1, 3)
    cu = opSet[randint(0, 2)](cu, j) * randint(1, 3)
    # cu = opSet[randint(0, 3)](cu, k) * randint(1, 3)
    return [latex(cu), latex(simplify(diff(cu,x))), cu.evalf(subs={x:1})]
