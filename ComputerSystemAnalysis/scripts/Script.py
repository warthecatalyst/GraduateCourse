import matplotlib.pyplot as plt

if __name__ == '__main__':
    with open('testOut.txt') as f:
        lines = f.readlines()
        x_line = [500 * i for i in range(1, 11)]
        y_lines1 = []
        y_lines2 = []
        n = []
        d = []
        colors = ["#F08080", "#DB7093", "C19090"]
        for out in range(7):
            y_line1 = []
            y_line2 = []
            flag = 0
            for i in range(out * 10, out * 10 + 10):
                line = lines[i].rstrip()
                data = line.split("\t")
                if flag == 0:
                    flag = 1
                    n.append(int(data[0]))
                    d.append(int(data[1]))
                y_line1.append(float(data[3]))
                y_line2.append(float(data[4]))
            # drawing
            y_lines1.append(y_line1)
            y_lines2.append(y_line2)
        # plt.title("n = "+str(n)+" d = "+str(d))
        print(x_line)
        print(y_lines1)
        print(y_lines2)
        print(n)
        print(d)
        plt.title("fixed n and d,max execution time and total tasks")
        plt.xlabel("total tasks")
        plt.ylabel("execution time")
        for i in range(len(n)):
            cur = "n = "+str(n[i])+" d = "+str(d[i])
            print(cur)
            plt.plot(x_line, y_lines1[i], label=cur)
        plt.legend()
        plt.show()
        plt.title("average execution time for different n and d")
        plt.xlabel("total tasks")
        plt.ylabel("execution time")
        for i in range(len(n)):
            if n[i] == 8:
                cur = "n = "+str(n[i])+" d = "+str(d[i])
                print(cur)
                plt.plot(x_line, y_lines2[i], label=cur)
        plt.legend()
        plt.show()
