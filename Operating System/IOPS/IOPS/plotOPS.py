
import numpy as np
import matplotlib.pyplot as plt


def plot(block,latencycounts, speedcounts, filepath):
    epoch = list(latencycounts.keys())
    speed = list(speedcounts.values())
    latency = list(latencycounts.values())
    fig, ax1 = plt.subplots()
    ax1.plot(epoch, speed, color='#2A6EA6', label='ops(MB/s)')
    ax2 = ax1.twinx()  # this is the important function

    ax2.plot(epoch, latency, color='#FFA933', label='latency(ms)')
    # ax.set_xlim(1, SIZE[-1])
    # ax.set_xscale('log', basex=2)
    ax1.set_ylim(0, max(speed))
    for a, b in zip(epoch, speed):
        ax1.text(a, b, (a, round(b, 2)), ha='center', va='bottom', fontsize=10)

    ax1.set_xlabel('Forktimes')
    ax1.legend(loc="upper left")
    ax2.legend(loc="upper right")
    ax1.set_title("block ="+str(block))
    # plt.show()
    plt.savefig(filepath+".png")


def ReadData(num):
    latencycounts = {}
    speedcounts = {}
    block = 1
    filepath = "./ram_r_s/" + str(num)
    file = open(filepath+".txt", "r")
    data = file.readlines()
    for each in data:
        a = each.split("\n")[0].split(",")
        block = int(a[0])
        time = int(a[1])
        latency = float(a[2])
        speed = float(a[3])
        latencycounts[time] = latencycounts.setdefault(time, 0)+latency
        speedcounts[time] = speedcounts.setdefault(time, 0)+speed

    plot(block,latencycounts, speedcounts, filepath)


for i in range(6, 13):
    tmp = pow(2, i)
    ReadData(tmp)
