#!/usr/bin/env python

import pandas as pd
import math
import os 
dir_path = os.path.dirname(os.path.realpath(__file__))

data = pd.read_csv(os.path.join(dir_path, "UnicodeData.txt"), sep=";", header=None)

with open(os.path.join(dir_path, "..", "core", "vca", "case_mappings.cpp"), "w") as f:
    f.write('// Auto-generated via data/create_case_mappings.py. Do not edit by hand!\n')
    f.write('#include "case_mappings.h"\n\n')
    f.write('namespace vca\n')
    f.write('{\n\n')
    f.write('const std::unordered_map<Char, Char>&\n')
    f.write('lower_to_upper_map()\n')
    f.write('{\n')
    f.write('    static const std::unordered_map<Char, Char> map{\n')
    for _, row in data.iterrows():
        l = row[0]
        u = row[13]
        if type(u) == str:
            f.write("        {0x%s, 0x%s},\n" % (l, u))
    f.write('    };\n')
    f.write('    return map;\n')
    f.write('}\n\n')
    f.write('const std::unordered_map<Char, Char>&\n')
    f.write('upper_to_lower_map()\n')
    f.write('{\n')
    f.write('    static const std::unordered_map<Char, Char> map{\n')
    for _, row in data.iterrows():
        u = row[0]
        l = row[12]
        if type(l) == str:
            f.write("        {0x%s, 0x%s},\n" % (u, l))
    f.write('    };\n')
    f.write('    return map;\n')
    f.write('}\n')
    f.write('\n} // namespace vca\n')
