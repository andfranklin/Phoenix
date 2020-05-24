import numpy as np
import matplotlib.pyplot as plt

center_data = np.genfromtxt('center_nhg_plate.csv', delimiter=',', skip_header=1)
center_Ts = center_data[:,0][::4]
center_Zs = center_data[:,2][::4]

edge_data = np.genfromtxt('edge_nhg_plate.csv', delimiter=',', skip_header=1)
edge_Ts = edge_data[:,0][::4]
edge_Zs = edge_data[:,2][::4]

corner_data = np.genfromtxt('corner_nhg_plate.csv', delimiter=',', skip_header=1)
corner_Ts = corner_data[:,0][::4]
corner_Zs = corner_data[:,2][::4]

print("center diff :", center_Ts[0] - center_Ts[-1])
print("edge diff   :", edge_Ts[0] - edge_Ts[-1])
print("corner diff :", corner_Ts[0] - corner_Ts[-1])
print()
print("front, center - corner :", center_Ts[0] - corner_Ts[0])
print("back, center - corner  :", center_Ts[-1] - corner_Ts[-1])

plt.plot(center_Zs, center_Ts, lw=1, marker='o', ms=6, label="Center Line", zorder=1)
plt.plot(edge_Zs, edge_Ts, lw=1, marker='o', ms=6, label="Edge Line", zorder=1)
plt.plot(corner_Zs, corner_Ts, lw=1, marker='o', ms=6, label="Corner Line", zorder=1)
plt.ylabel("$T(z)$ [K]")
plt.xlabel("$z$ [m]")
plt.legend()
plt.savefig("plate_com_line_samples_nhg.pdf")
plt.show()

