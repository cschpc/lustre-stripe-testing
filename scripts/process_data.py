import numpy as np
from pathlib import Path

filePaths = Path(".").glob("*.out")

# Original column numbers
colMB = 0

colTime = 1
colBandwidth = 2

data = []

for file in filePaths:

    dataPerRun = np.genfromtxt(file, dtype=float, skip_header=2)
    data.append(dataPerRun)

data = np.asanyarray(data)

# Handle case of just one datafile
if len(data.shape) < 3:
    data = data[np.newaxis, :]

# Now data[i, :] gives results of the ith run

fname = "results.txt"
print(f"Writing to {fname}")

header = "FileSizeMB AvgTime MinTime MaxTime MedianTime AvgBandwidth MinBandwidth MaxBandwidth MedianBandwidth\n"
# Analyze each 'fileSize' row separately, computing avg, min, max bandwidth
with open("results.txt", 'w') as outFile:

    outFile.write(header)
    for j in range(data.shape[1+colMB]):
        d = data[:,j,:]

        avgTime = np.average(d[:, colTime])
        minTime = np.min(d[:, colTime])
        maxTime = np.max(d[:, colTime])
        medianTime = np.median(d[:, colTime])

        avgBandwidth = np.average(d[:, colBandwidth])
        minBandwidth = np.min(d[:, colBandwidth])
        maxBandwidth = np.max(d[:, colBandwidth])
        medianBandwidth = np.median(d[:, colBandwidth])

        sizeMB = int(d[0,0])

        outFile.write(f"{sizeMB:d} {avgTime:g} {minTime:g} {maxTime:g} {medianTime:g} " \
                      f"{avgBandwidth:g} {minBandwidth:g} {maxBandwidth:g} {medianBandwidth:g}\n")
    #
#
