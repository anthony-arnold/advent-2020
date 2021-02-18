#!/usr/bin/env python
import sys
from lark import Lark
import re

"""
Make sure Lark is installed (pip install lark).

Pass the input to stdin; the program will print the valid
lines. To determine the number of valid lines, pipe to wc -l

e.g.

    $ ./day19.py < day19.txt | wc -l
      350
"""

grammar = ['start: n_0']

parser = None
for line in sys.stdin.readlines():
    line = line.strip()

    if parser:
        try:
            # Test validity
            parser.parse(line)
            print(line)
        except:
            # Discard invalid lines
            pass

    elif line == '':
        # Grammar file built
        parser = Lark('\n'.join(grammar))
        continue

    elif '"' in line:
        c = line.split('"')[1]
        n = line.split(':')[0]
        grammar += ['n_{0}: {1}'.format(n, c.upper())]
        grammar += ['{0}: "{1}"'.format(c.upper(), c)]
    else:
        grammar += [re.sub(r'([0-9]+)', r'n_\1', line)]
