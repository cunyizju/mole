#!/usr/bin/python3
#
# oofem2part.py       (c) 2013 Borek Patzak, www.oofem.org
#
# Description
#    
#     oofem2part reads serial oofem input file and performs 
#     the node-cut partitioning of the problem domain 
#     into a set of subdomains (using metis),
#     automatically identifying shared nodes.
#     On output, it produces a set of oofem input files for each 
#     subproblem, suitable for parallel oofem analysis.
#
# Dependencies
#
#     oofem2part requires "Metis to python" wrapper module
#     (see http://metis.readthedocs.org/en/latest/)
#     the METIS_DLL environment variable should point to 
#     Metis shared library location.
#
# Usage
#
#     oofem2part -f file -n #
#     where -f option allows to set path to oofem input input file, 
#           -n option sets the number of desired target partitions
#
# Note
#
#     Some adjustment of created parallel input files may be necessary,
#     typically the solver has to be changed into parallel one.
#

import sys
import re
from sets import Set   # Python 2 specific, replaced with set in Python 3
import getopt

#debug flag
debug = 0

#some constants defining nodal status
UNDEF = 0
LOCAL = 1
SHARED= 2


#----do not change------
# number of partitions
nparts = 0
# input File Name 
inputFileName = ''

def print_usage():
    print("\nUsage:\noofem2part -f file -n #")
    print("where -f file sets path to input oofem (serial) file to be partitioned")
    print("      -n #    allows to set the required number of target partitions")


# returns the value corresponding to given keyword and record
def getKeywordValue (record, kwd, optional = None):
    match = re.search (kwd+'\s+\"*([\\\.\+\-,:\w]+)\"*', record, re.IGNORECASE)
    if match:
        return match.group(1)
    else:
        #issue an error if argument compulsory
        if optional is None:
            print("\nMissing keyword \"", kwd, "\" in\n", record)
            exit(1)
        else:
            return optional


def readRecord (file):
    line = file.readline()
    while re.search(r"^#", line):
        line = file.readline()
    return line


# returns a list of node masters, empty list if node is not slave
def giveNodeMasters(inode):
    global nodes
    global nodemap
    global elemnodes
    global elemmap

    answer = []
    # this pattern covers simple slave relations and rigid arm nodes
    match = mastermask_re.search(nodes[inode])
    if match:
        nm = int(match.group(1))
        en = match.group(2).split()
        for i in range(nm):
            inode = int(en[i])  # global number
            answer.append(nodemap[inode])  # local numbering
        return answer

    # match slave nodes
    match = slavenodemasters_re.search(nodes[inode])
    if match:
        nm = int(match.group(1))
        en = match.group(2).split()
        for i in range(nm):
            inode = int(en[i])  # global number
            answer.append(nodemap[inode])  # local numbering
        return answer

    match = hangingnode_re.search(nodes[inode])
    if match:
        pattern = r'masterElement\s+(\d+)\s+'
        match = re.search(pattern, nodes[inode], re.IGNORECASE)
        if match:
            e = int(match.group(1))
            # add all elem nodes to master list
            answer.append(elemnodes[elemmap[e]])
            return answer
        else:
            # hanging node without masterElement
            print("Warning: Support for hanging nodes without masterElement not available")
            exit(1)

    return answer


# returns a list containing for each node following tuple:
# (statuses, partitions), where status is local or shared and
# partitions is a list containing node partitions
def classifyNodes (elem_part):
    global nodes, nelem
    global ndofman

    nodalstatuses = [0] * ndofman
    nodalpartitions = []
    # loop over dofmans
    for i in range(ndofman):
        nodalpartitions.append(set())  # replaced Set with set in Python 3
        # loop over elements sharing node
        for j in nodalconnectivity[i]:
            nodalpartitions[i].add(elem_part[j])

        if len(nodalpartitions[i]) == 1:
            nodalstatuses[i] = LOCAL
        else:
            nodalstatuses[i] = SHARED

    # account for master->slave connections (here only simple slave dofs handled)
    # loop over all nodes
    for inode in range(ndofman):
        masters = giveNodeMasters(inode)  # masters empty if none
        for m in masters:
            # add inode partition to master partition
            if nodalstatuses[inode] == LOCAL:
                if nodalpartitions[inode].issubset(nodalpartitions[m]):
                    pass  # dependence already satisfied
                else:
                    # make sure that master is accessible from inode partition
                    nodalstatuses[m] = SHARED
                    nodalpartitions[m].update(nodalpartitions[inode])
            else:
                # inode is shared => master has to be shared as well on the same partitions
                nodalstatuses[m] = SHARED
                nodalpartitions[m].update(nodalpartitions[inode])

    return nodalstatuses, nodalpartitions


# returns (loc_nodes, shared_nodes) tuple containing
# loc_nodes dict of local nodes on partition i
# shared_nodes dict of shared nodes 
def getPartitionNodeList(i, elem_part):
    global nodes

    loc_nodes = set()  # replaced Set with set in Python 3
    shared_nodes = {}
    for j in range(nelem):
        ep = elem_part[j]
        if ep == i:
            # element on i-th partition found
            # loop over element nodal list
            for inode in elemnodes[j]:
                # now check if all elements shared by node are assigned to the same i-th partition => local node, shared otherwise
                local = True
                for k in nodalconnectivity[inode]:
                    if elem_part[k] != i:
                        local = False
                        shared_nodes.setdefault(inode, set()).add(elem_part[k])  # replaced Set with set

                if local:
                    loc_nodes.add(inode)

    # account for master->slave connections (here only simple slave dofs handled)
    # loop over all nodes
    for inode in nodes:
        masters = giveNodeMasters(inode)
        for m in masters:
            # mark masters as shared to make sure that master is accessible in i-th partition
            if inode in loc_nodes and not (m in loc_nodes):  # master is on remote partition
                shared_nodes.setdefault(inode, set()).add(elem_part[k])  # replaced Set with set

            if not (inode in loc_nodes and m in loc_nodes):
                # add master into a list of shared nodes
                shared_nodes.setdefault(m, set()).add(elem_part[k])  # replaced Set with set

    return loc_nodes, shared_nodes


# open input file to partition
infile = open(inputFileName, 'r')

# parse input file
parseInput(infile)

# now create adjacency list
# nodecut assumed - elements graph vertices, nodes represent edges
adj = [set() for i in range(nelem)]  # replaced Set with set in Python 3
for ie in range(nelem):
    # loop over element nodes (local numbering)
    for i in elemnodes[ie]:
        for k in nodalconnectivity[i]:
            if k != ie:
                adj[ie].add(k)
                adj[k].add(ie)

# done; call metis now
try:
    from metis import part_graph
    cuts, part_vert = part_graph(adj, nparts)
except:
    print("metis module not installed or internal metis error encountered")
    exit(0)

# write partitioned mesh on output
print("Partition  local_nodes shared_nodes   elements")
print("----------------------------------------------")
nodalstatuses, nodalpart = classifyNodes(part_vert)
for i in range(nparts):
    writePartition(i, part_vert, nodalstatuses, nodalpart)

print("Done")
