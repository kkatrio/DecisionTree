import numpy as np
import matplotlib.pyplot as plt

fdata = open('build/data/data.txt')
data = np.loadtxt(fdata)

x = data[:, 0];
y = data[:, 1];
l = data[:, 2];

xmin = np.amin(x) - 0.5;
xmax = np.amax(x) + 0.5;
xm = np.arange(xmin, xmax, 0.1);


fig, ax = plt.subplots()

with open('build/data/lines.txt') as fm:
    for line in fm:
        model = np.fromstring(line, dtype=float, sep=' ')
        b0 = model[0]
        b1 = model[1]
        xmin = model[2]
        xmax = model[3]
        xm = np.arange(xmin, xmax, 0.1);
        ypred = b0 + b1 * xm
        ax.plot(xm, ypred)

ax.scatter(x, y, None, l)
#ax.set_aspect('equal')

plt.show()
