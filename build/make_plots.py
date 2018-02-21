#!/usr/bin/env python

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

size = "709MB"
threads = [1,2,3,4]
tdf = [
        [12.39, 12.39, 11.89, 11.66, 11.95],
        [7.11, 7.22, 7.62, 7.43, 7.28],
        [6.61, 6.81, 6.69, 6.76, 6.65],
        [6.65, 6.79, 6.55, 6.66, 6.65],
        ]
root_pandas = [11.56, 11.63, 11.58, 11.59, 11.77]

plt.figure(1, figsize=(3,3))
plt.xticks([1, 2, 3, 4])
plt.errorbar(threads, np.mean(tdf, axis=-1), np.std(tdf, axis=-1), fmt="+", label="TDataFrame")
plt.axhline(y=np.mean(root_pandas), color="r", label="root_numpy")
plt.axhline(y=np.mean(root_pandas)-np.std(root_pandas), color="r", alpha=0.3)
plt.axhline(y=np.mean(root_pandas)+np.std(root_pandas), color="r", alpha=0.3)
plt.xlabel("Number of threads")
plt.ylabel("Elapsed time in seconds")
plt.title("Loading 709MB of data from disk to memory.\nArray of random floats with shape (50000000, 4)")
plt.legend()
plt.savefig("threads.pdf", bbox_inches="tight")

size = [0.7, 1.4, 2.1, 2.8]
tdf_1 = [12.7, 23.3, 38.0, 50.5]
tdf_2 = [7.9, 14.4, 23.9, 29.9]
tdf_3 = [7.1, 13.3, 19.8, 26.0]
tdf_4 = [6.5, 12.7, 17.7, 24.3]
root_pandas = [11.8, 25.8, 37.0, 51.4]

plt.figure(2, figsize=(3,3))
for x, label in zip([tdf_1, tdf_2, tdf_3, tdf_4, root_pandas], ["TDF with 1 thread", "TDF with 2 threads", "TDF with 3 threads", "TDF with 4 threads", "root_numpy"]):
    plt.plot(size, x, "+--", label=label, alpha=0.7)
plt.xticks(size)
plt.xlabel("Size of data in MB")
plt.ylabel("Elapsed time in seconds")
plt.title("Performance subject to input data size and number of threads")
plt.legend(loc=1, bbox_to_anchor=(1.9, 1.00))
plt.savefig("size.pdf", bbox_inches="tight")

plt.figure(3, figsize=(3,3))
threads = [1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20]
tdf = [
        [57.7, 57.5, 59.4, 54.9, 58.6],
        [50.5, 54.5, 51.8, 48.3, 48.4],
        [47.7, 39.9, 32.7, 40.4, 39.8],
        [37.8, 38.6, 41.3, 36.4, 31.9],
        [37.1, 31.4, 34.8, 29.8, 32.4],
        [33.2, 33.3, 41.5, 33.8, 31.2],
        [27.2, 27.4, 23.5, 31.5, 18.5],
        [15.1, 15.8, 15.5, 13.0, 16.9],
        [20.2, 26.7, 25.8, 28.4, 24.9],
        [19.9, 18.7, 18.0, 20.2, 17.2],
        [17.0, 19.7, 19.2, 20.2, 18.1],
        ]
plt.errorbar(threads, np.mean(tdf, axis=1), np.std(tdf, axis=1), fmt="+")
plt.xlabel("Number of threads")
plt.ylabel("Elapsed time in seconds")
plt.title("Loading 2.8GB of data from disk to memory.\nArray of random floats with shape (2e8, 4)")
