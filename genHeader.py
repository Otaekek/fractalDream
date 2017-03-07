import os;

print("#ifndef HEADERLIST")
print("#define HEADERLIST")
print("char headerList[65536] = \"", end='')
with open('.tmpheader', 'r') as fin:
    print (fin.read(), end='')
print("\";")
print("#endif")
