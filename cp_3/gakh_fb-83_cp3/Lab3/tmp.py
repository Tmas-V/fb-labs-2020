from Affine_cipher import *


text = filter_raw_text(".\\raw_text.txt").upper()
cur_counts=get_counts_of_bigramms(text[:-1])

cur_counts = sorted(cur_counts.items(), key=lambda item: item[1])[::-1]


with codecs.open('.\\bigramms.txt', "w","utf-8") as file:
       for c in cur_counts:
              file.write("'{}', ".format(c[0]))
file.close()
