import matplotlib.pyplot as plt

plt.figure(1, figsize=(5,5))
x = [1e3, 1e4, 1e5, 1e6, 1e7]
uproot = [     0.01, 0.01, 0.02, 0.05, 0.44, ]
root_numpy = [ 0.06, 0.06, 0.07, 0.13, 0.84, ]
rdf = [        0.72, 0.72, 0.76, 1.12, 4.91, ]
for y, l in zip([rdf, uproot, root_numpy], ["RDataFrame", "uproot", "root_numpy"]):
    plt.loglog(x, y, "o-", label=l)
plt.legend()
plt.xlabel("Entries")
plt.ylabel("Time in seconds")
plt.savefig("matrix.png", bbox_inches="tight")

plt.figure(2, figsize=(5,5))
x = [1e3, 1e4, 1e5, 1e6, 1e7]
uproot = [     0.01, 0.01, 0.02, 0.05, 0.41, ]
root_numpy = [ 0.06, 0.06, 0.07, 0.13, 0.82, ]
rdf = [        0.90, 0.93, 1.27, 4.61, 38.4, ]
for y, l in zip([rdf, uproot, root_numpy], ["RDataFrame", "uproot", "root_numpy"]):
    plt.loglog(x, y, "o-", label=l)
plt.legend()
plt.xlabel("Entries")
plt.ylabel("Time in seconds")
plt.savefig("dict.png", bbox_inches="tight")
