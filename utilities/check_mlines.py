import os
import sys
print ("Milu Utility: print out line")

#get the cmd to produce original program
name = sys.argv[1]
mut_path = sys.argv[2]

mutants=mut_path+"/mutants"

ff=open(name,"r")
ori_lines=ff.readlines()
ff.close()

def check_line_diff(ori_lines,mut_lines):
        size = len(ori_lines)
        for i in range(size):
                if  ori_lines[i] != mut_lines[i]:
                        return i+1
        return 0


for file in os.listdir(mutants):
        src_path=mutants+"/"+file+"/src/"+name
        ff=open(src_path,"r")
        mut_lines=ff.readlines()
        ff.close()
        print "M"+file + " " + str(check_line_diff(ori_lines,mut_lines))






