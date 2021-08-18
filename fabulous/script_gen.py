import re
from array import *
import fileinput
import sys, getopt
import csv

def main(argv):
    vlog = ''
    top = ''
    try:
        opts, args = getopt.getopt(argv,"hf:t:",["Vlog=","Top="])
    except getopt.GetoptError:
        print ('script_gen.py -r <verilog file> -t <top module name>')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print ('top_wrapper_generator.py -r <NumberOfRows> -c <NumberOfCols> -b <FrameBitsPerRow> -f <MaxFramesPerCol> -d <desync_flag>')
            sys.exit()
        elif opt in ("-f", "--Vlog"):
            vlog = arg
        elif opt in ("-t", "--Top"):
            top = arg
    if not top :
        top = vlog.replace('.v','')
    print ('File       :',vlog)
    print ('Top_module :', top)

    script_str = ''
    try:
        with open("./template_temp.txt", 'r') as file :
            script_str = file.read()
    except IOError:
        print("template_temp.txt not accessible")
        sys.exit(2)
    script_str = script_str.replace("template.v", vlog)
    script_str = script_str.replace("-top template", '-top '+top)
    script_str = script_str.replace("template.json", top+'.json')

    if script_str :
        with open(top+'.ys', 'w') as file:
            file.write(script_str)

if __name__ == "__main__":
    main(sys.argv[1:])

