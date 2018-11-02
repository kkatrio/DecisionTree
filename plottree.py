import numpy as np
import matplotlib.pyplot as plt

fdata = open('build/data/data.txt')
data = np.loadtxt(fdata)

x = data[:, 0];
y = data[:, 1];
l = data[:, 2];

fig, ax = plt.subplots()

# quick plot until countourf with prediction
xmins = [np.amin(x) - 0.1 , -0.71, -2.1]
xmaxs = [np.amax(x) + 0.15, -0.2, -0.69]
i = 0

with open('build/data/lines.txt') as fm:
    for line in fm:

        model = np.fromstring(line, dtype=float, sep=' ')
        b0 = model[0]
        b1 = model[1]

        xmin = xmins[i]
        xmax = xmaxs[i]
        xm = np.arange(xmin, xmax, 0.1);

        ypred = b0 + b1 * xm
        ax.plot(xm, ypred)

        i = i + 1

ax.scatter(x, y, None, l)
#ax.set_aspect('equal')

plt.show()
