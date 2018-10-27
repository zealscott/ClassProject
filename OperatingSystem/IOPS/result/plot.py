import numpy as np
import matplotlib.pyplot as plt

SIZE = [64,128, 256, 512, 1024, 2048,4096]
# SIZE = [128, 256, 512, 1024, 2048]


def readdata(filename):
    """ index 0 for ``ram``
        index 1 for ``disk``
    """
    randomwrite = [[], []]
    randomread = [[], []]
    sequentialread = [[], []]
    sequentialwrite = [[], []]
    file = open(filename, 'r')
    data = file.readlines()
    for line in data:
        each = line.split('\t')
        if each[3] == 'ram':
            if each[4] == 'sequential':
                if each[5] == 'read':
                    sequentialread[0].append(float(each[7]))
                else:
                    sequentialwrite[0].append(float(each[7]))
            else:
                if each[5] == 'read':
                    randomread[0].append(float(each[7]))
                else:
                    randomwrite[0].append(float(each[7]))
        else:
            if each[4] == 'sequential':
                if each[5] == 'read':
                    sequentialread[1].append(float(each[7]))
                else:
                    sequentialwrite[1].append(float(each[7]))
            else:
                if each[5] == 'read':
                    randomread[1].append(float(each[7]))
                else:
                    randomwrite[1].append(float(each[7]))
    # global SIZE
    # SIZE = np.logspace(
    #     0, len(sequentialread[0])-1, len(sequentialread[0]), base=2)

    # order_plot(sequentialread[0], randomread[0], 'read', 'ram')
    # order_plot(sequentialread[1], randomread[1], 'read', 'disk')
    # order_plot(sequentialwrite[0], randomwrite[0], 'write', 'ram')
    # order_plot(sequentialwrite[1], randomwrite[1], 'write', 'disk')

    # type_plot(sequentialread[0], sequentialwrite[0], 'sequence', 'ram')
    # type_plot(sequentialread[1], sequentialwrite[1], 'sequence', 'disk')
    # type_plot(randomread[0], randomwrite[0], 'random', 'ram')
    # type_plot(randomread[1], randomwrite[1], 'random', 'disk')

    # device_plot(sequentialread[0], sequentialread[1], 'sequence', 'read')
    # device_plot(sequentialwrite[0], sequentialwrite[1], 'sequence', 'write')
    # device_plot(randomread[0], randomread[1], 'random', 'read')
    # device_plot(randomwrite[0], randomwrite[1], 'random', 'write')

    write_plot(sequentialwrite[0], randomwrite[0],
               sequentialwrite[1], randomwrite[1])
    read_plot(sequentialread[0], randomread[0],
              sequentialread[1], randomread[1])


# type:write/read
# device:ram/disk
# order:sequence,random

# def three_plot():
#     from mpl_toolkits.mplot3d import Axes3D
#     fig = plt.figure()
#     ax = fig.add_subplot(111, projection='3d')
#     X = [1, 1, 2, 2]
#     Y = [3, 4, 4, 3]
#     Z = [1, 2, 1, 1]
#     ax.plot_trisurf(X, Y, Z)
#     plt.show()


def write_plot(Sramspeed, Rramspeed, Sdiskspeed, Rdiskspeed):
    global SIZE
    fig, ax = plt.subplots()
    ax.plot(SIZE, Sramspeed, 'bx--',
            label='ram sequential write speed (MB/s)')
    ax.plot(SIZE, Rramspeed, 'bx--',
            label='ram Random write speed (MB/s)')
    ax.plot(SIZE, Sdiskspeed, 'ro:',
            label='disk sequential write speed (MB/s)')
    ax.plot(SIZE, Rdiskspeed, 'ro:',
            label='disk Random write speed (MB/s)')

    # ax.set_xlim(1, SIZE[-1])
    ax.set_xscale('log', basex=2)
    # ax.set_yscale('log', basey=10)
    # ax.grid(True)
    ax.set_xlabel('Block size (bytes)')
    ax.set_title('write plot')
    plt.legend(loc="upper left")
    plt.show()


def read_plot(Sramspeed, Rramspeed, Sdiskspeed, Rdiskspeed):
    global SIZE
    fig, ax = plt.subplots()
    ax.plot(SIZE, Sramspeed, 'bx--',
            label='ram sequential read speed (MB/s)')
    ax.plot(SIZE, Rramspeed, 'bx--',
            label='ram Random read speed (MB/s)')
    ax.plot(SIZE, Sdiskspeed, 'ro:',
            label='disk sequential read speed (MB/s)')
    ax.plot(SIZE, Rdiskspeed, 'ro:',
            label='disk Random read speed (MB/s)')

    # ax.set_xlim(1, SIZE[-1])
    ax.set_xscale('log', basex=2)
    # ax.set_yscale('log', basey=10)
    # ax.grid(True)
    ax.set_xlabel('Block size (bytes)')
    ax.set_title('read plot')
    plt.legend(loc="upper left")
    plt.show()


def device_plot(ramspeed, diskspeed, order, type):
    global SIZE
    fig, ax = plt.subplots()
    ax.plot(SIZE, ramspeed, color='#2A6EA6',
            label='ram ' + order + type + ' speed (MB/s)')
    ax.plot(SIZE, diskspeed, color='#FFA933',
            label='disk ' + order + type + ' speed (MB/s)')
    # ax.set_xlim(1, SIZE[-1])
    ax.set_xscale('log', basex=2)
    ax.set_yscale('log', basey=2)
    # ax.grid(True)
    ax.set_xlabel('Block size (bytes)')
    ax.set_title('device plot')
    plt.legend(loc="upper left")
    plt.show()


def type_plot(readspeed, writespeed, order, device):
    global SIZE
    fig, ax = plt.subplots()
    ax.plot(SIZE, readspeed, color='#2A6EA6',
            label=device + ' ' + order + ' read speed (MB/s)')
    ax.plot(SIZE, writespeed, color='#FFA933',
            label=device + ' ' + order + ' write speed (MB/s)')
    # ax.set_xlim(1, SIZE[-1])
    ax.set_xscale('log', basex=2)
    ax.set_yscale('log', basey=2)
    # ax.grid(True)
    ax.set_xlabel('Block size (bytes)')
    ax.set_title('type plot')
    plt.legend(loc="upper left")
    plt.show()


def order_plot(squence, random, type, device):
    global SIZE
    fig, ax = plt.subplots()
    ax.plot(SIZE, squence, color='#2A6EA6',
            label=device + ' sequential ' + type + ' speed (MB/s)')
    ax.plot(SIZE, random, color='#FFA933',
            label=device + ' random ' + type + ' speed (MB/s)')
    # ax.set_xlim(1, SIZE[-1])
    ax.set_xscale('log', basex=2)
    ax.set_yscale('log', basey=2)
    # ax.grid(True)
    ax.set_xlabel('Block size (bytes)')
    ax.set_title('order plot')
    plt.legend(loc="upper left")
    plt.show()


readdata("readspeed_new.txt")
