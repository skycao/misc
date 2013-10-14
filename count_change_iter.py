def count_change_iter(amount, denoms):
    """
    >>> amount, denoms = 100, [1, 5, 10, 25, 50, 100]
    >>> count_change_iter(amount, denoms)
    293
    """
    result = [1] #identity polynomial
    for denom in denoms:
        result = mult_poly(result, make_poly(amount, denom))
    return result[amount]

#multiplies two polynomials together, returns the list of coefficients
def mult_poly(c1, c2): #c1 and c2 are lists of coefficients of polynomials
    c = []
    for i in range(len(c1) + len(c2) - 1):
        c.append(coeff_of_pow_n(c1, c2, i))
    return c
    
#returns the coefficient of the power n term of the product of the 
#polynomials corresponding to c1 and c2
def coeff_of_pow_n(c1, c2, n):
    #return sum([c1[i] * c2[j] for i in range(len(c1)) for j in range(len(c2)) if i + j == n])
    end = min(n, len(c1) - 1) #highest power of c1 in the sum
    if n <= len(c2) - 1:
        #the sum is a_0 * b_n + a_1 * b_{n-1} + ... + a_end * b_{n - end}
        #notice 0 + n == n, and every next terms adds 1 to zero and subtracts 1 from n
        return sum([c1[i] * c2[n - i] for i in range(end + 1)])
    else:
        start = n - len(c2) + 1 #lowest power of c1 in the sum
        #the sum is a_start * b_k + ... + a_end * b_{k - end + start} where k == len(c2) - 1 is the power of polynomial b
        #notice that start + k == n, and every next term adds 1 to start and subtracts 1 from k
        return sum([c1[start + i] * c2[len(c2) - 1 - i] for i in range(end - start + 1)])

#given an amount and a denom from denoms, makes the corresponding polynomial,
#i.e. list of coefficients
def make_poly(amount, denom):
    poly = [1 if i == 0 else 0 for i in range(denom)]
    return trim_trail_zeroes(poly * (amount // denom + 1))

#trims the trailing zeroes of a polynomial
def trim_trail_zeroes(poly):
    """
    >>> a = [1, 1, 1, 0, 0, 0, 0]
    >>> b = [2, 0, 5, 0, 1, 0, 0]
    >>> trim_trail_zeroes(a)
    [1, 1, 1]
    >>> trim_trail_zeroes(b)
    [2, 0, 5, 0, 1]
    """
    index = len(poly) - 1
    while poly[index] == 0:
        poly.pop()
        index -= 1
    return poly
    
