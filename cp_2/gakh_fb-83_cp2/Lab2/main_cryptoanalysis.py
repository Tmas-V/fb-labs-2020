from Caesar_cipher_op import *
from Vigenere_cipher_op import *

cipher_text = codecs.open( ".\\var6_text\\ciphertext_var6.txt", "r", "utf-8" ).read()


lens_indexes=codecs.open( "indexes.txt", "r", "utf-8" ).read().split('\n')[:-1]

cur_INDEX=calculate_INDEX(cipher_text)
print("Calculated ciphertext INDEX = {}".format(cur_INDEX))
print(cipher_text[:60])
r=int(lens_indexes[0].split('\t')[0])
temp_delta=abs(float(lens_indexes[0].split('\t')[1])-cur_INDEX)
suggested_r=[]
for i in lens_indexes:
       new_delta=abs(float(i.split('\t')[1])-cur_INDEX)
       if abs(new_delta) <= 0.002:
              r = int(i.split('\t')[0])
              temp_delta = new_delta
              suggested_r.append(r)

for r in range(20, 60): 
       divided_text=divide_text(cipher_text, r)
       key=''
       for txt in divided_text:
              num=c_deduce_key(txt)
              key+=char(num%alphabet_len)
       
       dt=v_decode(cipher_text, key)
       i=calculate_INDEX(dt)
       if i >=0.04:
              print("Suggested key lenght(r) = {}".format(r))
              print("Suggested key: {}".format(key))
              print(dt[:80])
              print("INDEX = {}".format(i))
              print('-------------------------------------------------')

print('\n')
key='ВОЗВРАЩЕНИЕДЖИННА'
print(v_decode(cipher_text, key)[:60])
