import numpy as np
import matplotlib.pyplot as plt

fdata = open('../build/data/data.txt')
data = np.loadtxt(fdata)

fmodel = open('../build/data/model.txt')
model = np.loadtxt(fmodel)

x = data[:, 0];
y = data[:, 1];
l = data[:, 2];

xmin = np.amin(x) - 0.5;
xmax = np.amax(x) + 0.5;
xm = np.arange(xmin, xmax, 0.1);
b0 = model[0]
b1 = model[1]
ypred = b0 + b1 * xm


fig, ax = plt.subplots()
ax.scatter(x, y, None, l)
ax.plot(xm, ypred)

plt.show()

