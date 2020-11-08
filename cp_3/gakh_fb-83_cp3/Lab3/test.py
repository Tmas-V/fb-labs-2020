from Affine_cipher import *

def deduce_key(X1_str,Y1_str,X2_str,Y2_str):
       print("{} -> {}, {} -> {}".format(X1_str,Y1_str,X2_str,Y2_str))
       X1=encode_bigramm(X1_str)
       Y1=encode_bigramm(Y1_str)
       print("{} = a * {} + b ( mod {} )".format(Y1,X1,m))
       X2=encode_bigramm(X2_str)
       Y2=encode_bigramm(Y2_str)
       print("{} = a * {} + b ( mod {} )".format(Y2,X2,m))
       L1 = LINEAR_EQUATION((X1-X2)%m, (Y1-Y2)%m, m)
       L1.print()
       a=L1.solve()
       if a is None:
              return None
       print(a)
       b=[]
       for i in a:
              b.append((Y1-i*X1)%m)
       result = [(a[0],b[0])]
       for i in range(1,len(a)):
              result.append((a[i],b[i]))
       return result
       
       

s="ПРИНЕСИМНЕВЕЩЬ"

print(s)
c=encipher_affine(s,939, 253)
print(c)

all_keys=deduce_key(s[0]+s[1],c[0]+c[1], s[2]+s[3],c[2]+c[3])
if all_keys is not None:
       for key in all_keys:
              plain_text=decipher_affine(c, key[0], key[1])
              if plain_text is not None:
                     print(key, end=" ")
                     print(plain_text, end="  ")



