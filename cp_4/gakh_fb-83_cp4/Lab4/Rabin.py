from GCD import *
from Math_operations import *
import random

arr_small_n_prime = [3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]
small_primes_rows = {}
for s_p in arr_small_n_prime:
       small_primes_rows[s_p] = generate_row(2,s_p)


def small_prime_test(p):
       p_bin = get_numerals(p,2)
       #print(p_bin, end=" ")
       for sp in small_primes_rows.keys():
              if  check_mod(small_primes_rows[sp], p_bin, sp):
                     #print(sp, end = " ")
                     return False
       return True

def Miller_Rabin_test(p):
       k = 30
       s = 0
       tmp = p - 1
       while(tmp%2!=0):
              s+=1
              tmp = tmp // 2
       d = tmp
       #check = False
       for i in range(1,k):
              x = random.randint(3,p)
              #print("\nk = {}, x = {}\n".format(i, x))
              g = GCD(x,p)
              if (g!=1):
                     #print("{} had gcd with {} so not prime.".format(p, x))
                     return False
              fp = fast_power(x,d,p)
              if (fp != 1) and (fp != -1):
                     x_r = fp
                     for j in range(1,s):
                            x_r = fast_power(x_r,2,p)
                            if x_r == 1:
                                   #print("{} is not pseudo prime with {} so not prime.".format(p,x))
                                   return False
              #else:
                     #print("{} had +-1 after fast_power on {} so it`s pseudo prime with {}".format(p,x,x))
       return True
def get_prime_between(n0, n1):
       x0 = random.randint(n0,n1)
       m0 = 0
       if x0%2 == 0:
              m0 = x0+1
       else:
              m0 = x0
       #print("M0 = {}".format(m0))
       #print("-----------------------------------------------------------------------")
       for p in range(m0, n1, 2):
              #print("\nInspecting for primeness {}".format(p))
              #print("-----------------------------------------------------------------------")
              if small_prime_test(p):
                     #print("{} passed small test(can be prime by Miller-Rabin).".format(p))
                     if Miller_Rabin_test(p):
                            return p
              #else:
                     #print("{} DIDN`T pass small test SO NOT PRIME.".format(p))
       return None
