#обчисленням оберненого елементу за модулем із використанням розширеного алгоритму
#Евкліда, розв’язуванням лінійних порівнянь. При розв’язуванні порівнянь потрібно
#коректно обробляти випадок із декількома розв’язками, повертаючи їх усі

def GCD_recursive(a,b):
           if (a*a+b*b==0):
                  return None
           elif (a*b==0):
                  return max(a,b)
           if (a%b == 0):
                  return b
           else:
                  return GCD_recursive(b,a%b)
def GCD(a,b):
       d=GCD_recursive(a,b)
       if d is None:
              return None
       else:
              return abs(d)
def reverse_mod(a,m):
       if a>m:
              tmp=a
              a=m
              m=tmp
       if  GCD(a,m)!=1:
              return None
       #print("a={}, m={}".format(a,m))
       if a==1:
              return 1
       x=[m//a]
       r=[m, a, m%a]
       if r[0]!=1:
              while r[len(r)-1]!=1:
                     x.append(r[len(r)-2]//r[len(r)-1])
                     r.append(r[len(r)-2]%r[len(r)-1])
       #print("x: {}".format(x))
       #print("r: {}".format(r))
       i=len(x)-1
       counter=True
       b=1
       c=-x[len(x)-1]
       while i>0:
              i-=1
              b_copy=b
              b=c
              if counter:
                     c=b_copy-c*x[i]
                     counter=False
              else:
                     c=-b_copy+c*x[i]
                     counter=True
       #print("{} * {} + {} * {} = 1".format(r[0], b, r[1], c))
       return c


class LINEAR_EQUATION:
       def __init__(this, a, b, n):
              this.a = a
              this.b = b
              this.n = n
       def print(this):
              print("Equation: {} * x = {} mod ( {} )".format(this.a%this.n, this.b%this.n, this.n))
       def solve(this):
              if this.a==0 or this.n==0 or this.n==1:
                     return None              
              d=GCD(this.a,this.n)
              if d>1:
                     if this.b%d!=0:
                            return None
                     else:
                            a=this.a//d
                            b=this.b//d
                            n=this.n//d
                            x0=reverse_mod(a,n)*b%n
                            x = []
                            for i in range(1,d):
                                   x.append(x0+i*n)
                            return x
              else:
                     return [reverse_mod(this.a,this.n)*this.b%this.n]

                     
       
       
