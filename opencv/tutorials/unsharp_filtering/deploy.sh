#! /usr/bin/env python

import sys, os

def main():
    
    # check for arguments
    if len(sys.argv) < 2:
        print 'usage: ./deploy.sh  <opencv directory>'
        return 0
    
    # set opencv directory
    opencv_dir = sys.argv[1]

    # start moving files over
    print 'deploying to ' + opencv_dir
    
    #load file moving list
    f = open('deployment_table.txt', 'r')
    table = f.readlines()  #.strip().split(' ')
    print table.strip('\n')
    f.close()

    items = filter(lambda a: a != '', table)
    print items
       

if __name__ == '__main__':
    main()
