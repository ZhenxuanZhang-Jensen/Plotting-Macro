# -*- coding: utf-8 -*-
# @Author: Ram Krishna Sharma
# @Author Email: ram.krishna.sharma@cern.ch
# @Date:   2021-06-03
# @Last Modified by:   Ram Krishna Sharma
# @Last Modified time: 2021-07-06
import uproot
import argparse
import matplotlib.pyplot as plt
import os
import numpy
import matplotlib.colors as mcolors

pick_color = []
# print mcolors
# print "===="
# print mcolors.TABLEAU_COLORS
# print "===="
names = list(mcolors.TABLEAU_COLORS)
# print "===="
for i, name in enumerate(names):
    # print "***> ",mcolors.TABLEAU_COLORS[name]
    pick_color.append(mcolors.TABLEAU_COLORS[name])

parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
    description='''
    Example Command:
    ----------------
    python ReadTree.py -i InputRootFile.root  -t TreeName  --dir temp -var_file variableListToPlot.py

    ----------------
    '''
    )
parser.add_argument('-i', '--input_file',
    default='TnP_ntuple.root',
    type=str,
    help='Input root file'
    )
parser.add_argument('-t', '--tree_name',
    default='Ana/passedEvents',
    type=str,
    help='tree name of input root file'
    )
parser.add_argument('-var_file', '--var_file',
    default="Variables_Hist_Overlap.py",
    type=str,
    help='text file having list of all variables.'
    )
parser.add_argument('-d', '--debug',
    default=False,
    type=bool,
    help='debug true or false'
    )
parser.add_argument('-v', '--var_set_to_plot',
    default="twoD_var_list",
    type=str,
    help='''
    Which variable set to plot.
    This should be defined as dictionary in the args.var_file.'''
    # choices=["nJettiness", "mela", "others", "nJettinessMELA","all"]
    )
parser.add_argument('-logY', '--logY',
    default=True,
    type=bool,
    help='make y-axis as log: true/false'
    )
parser.add_argument('-dir', '--dir_to_save_plots',
    default="tmp_plots",
    type=str,
    help='Directory name to keep plots'
    )
args = parser.parse_args()
# print(args.accumulate(args.integers))

if not os.path.isdir(args.dir_to_save_plots):
    os.makedirs(args.dir_to_save_plots)

file = uproot.open(args.input_file)
if (args.debug): print(file)
if (args.debug): print(file.keys())

tree = file[args.tree_name]
if (args.debug):
#     print(tree.keys())
    print(tree)

if (args.debug):
    print("|{count:5} | {branch_name:46} |".format(count="count", branch_name="Branch Name"))
    print("|{count:5} | {branch_name:46} |".format(count="---", branch_name="---"))
    for i,name in enumerate(tree.arrays()):
        print("|{count:5} | {branch_name:46} |".format(count=i, branch_name=name))
        if i>11: break

branches = tree.arrays()
number_of_branches = len(branches)


import sys
cwd = os.getcwd()
sys.path.insert(0, cwd)  # mypath = path of module to be imported
variableListToPlot = __import__((args.var_file).replace(".py","")) # __import__ accepts string

# branchesToPlot  = variableListToPlot.nJettinessMELA
branchesToPlot  = getattr(variableListToPlot, args.var_set_to_plot)

total_number_of_plots = len(branchesToPlot)

# print total_number_of_plots
for count, var_plots in enumerate(branchesToPlot):
    # print count,var_plots
    # print "\t",len(var_plots)/4.0
    color_count = 0
    for VarCount in range(0,len(var_plots),5):
        # print VarCount
        plt.ioff() # to turn off the displaying plots.
        print("===> Plotting branch: {0:3}/{1:<3}, {2:31}, {3:3}, {4:3}, {5:3}".format(
            count+1,total_number_of_plots,
            var_plots[VarCount],
            var_plots[VarCount+1], var_plots[VarCount+2],var_plots[VarCount+3]
            # branchesToPlot[var_plots]
            )
        )
        # print var_plots[VarCount],var_plots[VarCount+1], var_plots[VarCount+2],var_plots[VarCount+3]
        # label = var_plots[VarCount+4]
        n, bins, patches = plt.hist(branches[var_plots[VarCount]],
            # bins=branchesToPlot[var_plots][0],
            bins=var_plots[VarCount+1],
            range=(var_plots[VarCount+2], var_plots[VarCount+3]),
            label=var_plots[VarCount+4],
            # alpha=0.6,
            fill=False,
            linewidth=2,
            # facecolor='c',
            # hatch='/',
            # edgecolor='red',
            edgecolor=pick_color[color_count],
            histtype='step',
            normed=True,
            )
        color_count = color_count+1
        # print("n = {}, \nbins = {}, \npatches = {}".format(n,bins,patches))
        # print "type(n): ",type(n)
        # print("type(n): {}".format(str(type(n))))
        # converting list to array
        # arr = numpy.array(n)
        print "\t",var_plots[VarCount],"Total Entries: ",numpy.sum(n),"\tarea: ",sum(numpy.diff(bins)*n)
        # plt.xlabel(var_plots[VarCount],fontsize=15)
        # plt.text(30,400,"Entries: "+str(numpy.sum(n)))
        # plt.show()
    plt.xlabel("")
    plt.ylabel('Number of events',fontsize=15)
    plt.tight_layout()
    plt.legend(loc='best')
    plt.savefig(args.dir_to_save_plots+os.sep+var_plots[VarCount]+'_'+str(count)+'.png')
    plt.savefig(args.dir_to_save_plots+os.sep+var_plots[VarCount]+'_'+str(count)+'.pdf')
    if (args.logY):
        plt.yscale('log')
        plt.savefig(args.dir_to_save_plots+os.sep+var_plots[VarCount]+'_'+str(count)+'_log.png')
        plt.savefig(args.dir_to_save_plots+os.sep+var_plots[VarCount]+'_'+str(count)+'_log.pdf')
        plt.yscale('linear')
    plt.close()
