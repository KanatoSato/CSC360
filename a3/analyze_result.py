import os
import random
import re
import matplotlib.pyplot as plt

def main():
    seeds = [None]*20                 # 20 random seeds stored here
    quantums = [50,100,250,500]       # list of quantum lengths to use
    dispatches = [0,5,10,15,20,25]    # list of dispatch lengths to use

    archive = {"q50": [[],[]], "q100": [[],[]], "q250": [[],[]], "q500": [[],[]]} # The first list stores average waiting for each dispatch length. The second list stores average turnaround for each dispatch length.


    try:      # If text file exists, overwrites the file with blank file
        file = open("result.txt","r")
        file.close()
        file = open("result.txt","w")

    except:   # If not, creates a file
        file = open("result.txt","w")

    for i in range (0,len(seeds)):  ## Seed generator
        seed = random.randint(1000, 9999)
        seeds[i] = seed

    for q in quantums:    ## for all combinations of quantum lengths and dispatch length, run command bellow.
        for d in dispatches:
            for seed in seeds:
                os.system("./simgen 1000 %d | ./rrsim --quantum %d --dispatch %d >> result.txt" %(seed,q,d))
        
    file.close()

    file = open("result.txt","r")

    i = 1
    total_wait = 0
    total_ta = 0
    for line in file:
        data = re.findall(r'\d*\.\d+|\d+', line)  # Extract quantum, dispatch length, average waiting and average turnaround from result of simulator.
        if i < 20:
            quantum = int(data[0])
            total_wait = total_wait + float(data[2])
            total_ta = total_ta + float(data[3])
            i = i+1
        else:
            total_wait = total_wait + float(data[2])
            total_ta = total_ta + float(data[3])
            calc_avg(quantum,total_wait,total_ta,archive)
            total_wait = 0
            total_ta = 0
            i = 1

    plot_avg_wait(archive,dispatches,seeds) # plot dispatch VS avg_wait graph
    plot_avg_ta(archive,dispatches,seeds)   # plot dispatch VS avg_ta graph

# Calculate average waiting and average turnaround time
def calc_avg(quantum,total_wait,total_ta,archive):

    if quantum == 50:
        avg_wait = total_wait / 20
        avg_ta = total_ta / 20

        archive["q50"][0].append(avg_wait)
        archive["q50"][1].append(avg_ta)

    elif quantum == 100:
        avg_wait = total_wait / 20
        avg_ta = total_ta / 20

        archive["q100"][0].append(avg_wait)
        archive["q100"][1].append(avg_ta)

    elif quantum == 250:
        avg_wait = total_wait / 20
        avg_ta = total_ta / 20

        archive["q250"][0].append(avg_wait)
        archive["q250"][1].append(avg_ta)

    else:
        avg_wait = total_wait / 20
        avg_ta = total_ta / 20

        archive["q500"][0].append(avg_wait)
        archive["q500"][1].append(avg_ta)

# Plot dispatch VS avg_wait graph graph and save it as pdf
def plot_avg_wait(archive,dispatches,seeds):
    f = plt.figure()
    plt.plot(dispatches,archive["q50"][0])
    plt.plot(dispatches,archive["q100"][0])
    plt.plot(dispatches,archive["q250"][0])
    plt.plot(dispatches,archive["q500"][0])

    plt.title("Round Robin scheduler -- # tasks: 1000; \nseed values: %s" %(str(seeds)))
    plt.xlabel("Dispatch overhead")
    plt.ylabel("Average waiting time")
    plt.legend(["q=50","q=100","q=250","q=500"])
    
    f.savefig("graph_waiting.pdf", bbox_inches='tight')

# Plot dispatch VS avg_ta graph graph and save it as pdf
def plot_avg_ta(archive,dispatches,seeds):
    f = plt.figure()
    plt.plot(dispatches,archive["q50"][1])
    plt.plot(dispatches,archive["q100"][1])
    plt.plot(dispatches,archive["q250"][1])
    plt.plot(dispatches,archive["q500"][1])

    plt.title("Round Robin scheduler -- # tasks: 1000; \nseed values: %s" %(str(seeds)))
    plt.xlabel("Dispatch overhead")
    plt.ylabel("Average turnaround time")
    plt.legend(["q=50","q=100","q=250","q=500"])
    
    f.savefig("graph_turnaround.pdf", bbox_inches='tight')

if __name__ == "__main__":
    main()
