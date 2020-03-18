
#para_temp=[40, 85, 30, 75, -5, 70,
#            0, 58,-10, 45,-50,  0,
#           10, 45, -5, 25,  5, 20]
#para_get=[0]*18
path="./para.txt"

def para_read(para,n):
    count=0
    with open(path,"r") as file:
        for i in range(0,n):
            l=file.readline()
            para[count]=(int)(l)
            count=count+1

def para_write(para,n):
    with open(path,"w") as f:
        for i in range(0,n):
            f.write(str(para[i]))
            f.write("\n")

#while(True):
#    para_write(para_temp,18)
#    para_read(para_get,18)
#    print(para_get)
#    break

