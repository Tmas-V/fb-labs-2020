def fast_power(x,a,m):
       if x[1]!='b' or a[1]!='b' or m[1]!='b':
              print("-----------------------------\nError in fast_power(): operands not binary.\n-----------------------------")
              return None
       x_i=int(x[2:],2)
       a_b=a[2:][::-1]
       m_i=int(m[2:],2)
       x_ans=1
       x_temp=x_i%m_i
       for i in range(0,len(a_b)):
              if a_b[i] == '1':
                     x_ans=x_ans*x_temp%m_i
              x_temp=x_temp*x_temp%m_i
       return x_ans
def generate_row(B,m):
       r=B%m
       ans=[1]
       while r!=1:
              ans.append(r)
              if r==0:
                     return ans
              r=r*B%m
       return ans
def get_numerals(x,B):
       if B==2:
              x_bin=bin(x)[2:][::-1]
              ans=[]
              for i in x_bin:
                     ans.append(int(i))
              return ans
       if B==16:
              x_hex=hex(x)[2:][::-1]
              ans=[]
              for i in x_hex:
                     ans.append(int('0x'+i))
              return ans
       if B==10:
              ans=[x%B]
              p=1
              while ans[-1]!=0:
                     p*=B
                     ans.append(x//p%B)
              return ans[:-1]
       else:
              return None
def check_mod(r_row,a_row,m):
       ans=0
       if r_row[-1]==0:
              for i in range(0,len(r_row)):
                     ans+=a_row[i]*r_row[i]%m
       else:
              l=len(r_row)
              for i in range(0,len(a_row)):
                     ans+=a_row[i]*r_row[i%l]%m
       return not bool(ans%m)
#x=123
#a=456
#m=789
#print("{}".format(fast_power(bin(x),bin(a),bin(m))))
r_10_11=generate_row(2,11)
a=get_numerals(1441,2)
print(a)
print(check_mod(r_10_11, a,11))
