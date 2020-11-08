from Affine_cipher import *


def deduce_key(X1_str,Y1_str,X2_str,Y2_str):
       #print("{} -> {}, {} -> {}".format(X1_str,Y1_str,X2_str,Y2_str))
       X1=encode_bigramm(X1_str)
       Y1=encode_bigramm(Y1_str)
       X2=encode_bigramm(X2_str)
       Y2=encode_bigramm(Y2_str)
       
       XY_LQ=XY_LINEAR_EQUATION(X1,Y1,X2,Y2, m)
       #XY_LQ.print()
       return XY_LQ.solve()
       
       

cipher_text = filter_raw_text(".\\V11").upper()
cur_counts=get_counts_of_bigramms(cipher_text)

cur_counts = sorted(cur_counts.items(), key=lambda item: item[1])[::-1]
#print(cur_counts)
print(freq_rating)
print('')
for i in range(0,5):
       for j in range(i+1,5):
              all_keys=deduce_key(freq_rating[i], cur_counts[i][0], freq_rating[j], cur_counts[j][0])
              if all_keys is not None:
                     #print(all_keys)
                     for key in all_keys:
                            plain_text=decipher_affine(cipher_text, key[0], key[1])
                            if plain_text is not None:
                                   index=calculate_INDEX(plain_text)
                                   if index > 0.04:
                                          print("{}  :  {}".format(key, plain_text[:200]))
                                   #print(key, end=" ")
                                   #print(plain_text[:30], end="  ")
                                   #c_counts=get_counts_of_bigramms(plain_text)
                                   #c_counts=sorted(c_counts.items(), key=lambda item: item[1])[::-1][:5]
                                   #print(c_counts)
                            #else:
                                   #print("deciphered text is None!!!!")
              print('--------------------------------------------------------------------------------\n')
                                   




#with codecs.open('.\\my_text\\plain_text.txt', "w","utf-8") as file:
#       file.write(filter_raw_text('.\\my_text\\raw_plaintext.txt'))
#file.close()

