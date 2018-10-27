

def choose(a,b,c,d):
    f = open("Myfinal.txt","w")
    totle = len(a)
    for i in range(totle):
        trueCount = 0
        trueCount += a[i]
        trueCount += b[i]
        trueCount += c[i]
        trueCount += d[i]
        if trueCount > 0:
            f.write("positive\n")
        else:
            f.write("negative\n")
    f.close()

def change(str):
    a = open(str,"r")
    data = a.readlines()
    real = []
    for line in data:
        line = line.split("\n")[0]
        if line == 'positive':
            real.append(1)
        else:
            real.append(-1)
    return real


fname = ["MyPredict1.txt","MyPredict2.txt","MyPredict3.txt","MyPredict4.txt"]
a = change(fname[0])
b = change(fname[1])
c = change(fname[2])
d = change(fname[3])
choose(a,b,c,d)