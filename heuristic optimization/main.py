# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import ppAlg,CBS,CBSwP,PBS
import dataStructure
import datetime,time

def main():
    text = open("result.txt","w")
    timecost = []
    flowtime = []
    # 运行100次
    for i in range(100):
        pro = dataStructure.create_pro(0)
        pro.printPro()
        pp = ppAlg.PPAlg(pro)
        cbs = CBS.CBSAlg(pro)
        cbswp = CBSwP.CBSwP(pro)
        pbs = PBS.PBSAlg(pro)
        curtime = []
        curflow = []


        ppstart = time.clock()
        ppflow = pp.main()
        ppend = time.clock()
        pptime = ppend-ppstart
        if len(ppflow) == 0:
            curflow.append("NoSolution")
            curtime.append("Notime")
        curflow.append(ppflow)
        curtime.append(pptime)

        cbsstart = time.clock()
        cbsflow = cbs.main()
        cbsend = time.clock()
        cbstime = cbsend-cbsstart
        if len(cbsflow) == 0:
            curflow.append("NoSolution")
            curtime.append("Notime")
        curflow.append(cbsflow)
        curtime.append(cbstime)

        cbswpstart = time.clock()
        cbswpflow = cbswp.main()
        cbswpend = time.clock()
        cbswptime = cbswpend - cbswpstart
        if len(cbswpflow) == 0:
            curflow.append("NoSolution")
            curtime.append("Notime")
        curflow.append(cbswpflow)
        curtime.append(cbswptime)

        pbsstart = time.clock()
        pbsflow = pbs.main()
        pbsend = time.clock()
        pbstime = pbsend - pbsstart
        if len(pbsflow) == 0:
            curflow.append("NoSolution")
            curtime.append("Notime")
        curflow.append(pbsflow)
        curtime.append(pbstime)

        timecost.append(curtime)
        flowtime.append(curflow)

    for i in range(100):
        text.write(str(timecost[i])+"\n")
        text.write(str(flowtime[i])+"\n")




if __name__ == '__main__':
    main()

