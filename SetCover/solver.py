#!/usr/bin/python
# -*- coding: utf-8 -*-

# The MIT License (MIT)
#
# Copyright (c) 2014 Carleton Coffrin
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.


from collections import namedtuple
from subprocess import Popen, PIPE

Set = namedtuple("Set", ['index', 'cost', 'items'])

def solve_it(input_data):
    # Modify this code to run your optimization algorithm

    # parse the input

    file = open("tmp.data", "w")
    file.write(input_data)
    file.close()

    process = Popen(["./solve.out"], stdout=PIPE, universal_newlines=True)
    (stdout, _) = process.communicate()

    splitted_input = input_data.split()
    k = (splitted_input[0], splitted_input[1])

    max_iter = {
        ("29", "157"): 10000,
        ("200", "1000"): 15000,
        ("500", "5000"): 10000,
        ("1000", "10000"): 1000,
        ("1023", "330"): 500
    }

    output_data = stdout.strip()

    file = open("init_sol.data", "w")
    file.write(output_data)
    file.close()

    process = Popen(["./a.out", str(max_iter[k])], stdout=PIPE, universal_newlines=True)
    (stdout, _) = process.communicate()

    #output_data = stdout.strip()   
    #os.remove("tmp.data")

    return stdout.strip()


import sys

if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1:
        file_location = sys.argv[1].strip()
        with open(file_location, 'r') as input_data_file:
            input_data = input_data_file.read()
        print(solve_it(input_data))
    else:
        print('This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/sc_6_1)')

