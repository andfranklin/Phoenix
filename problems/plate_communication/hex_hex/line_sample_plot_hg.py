import numpy as np
import matplotlib.pyplot as plt

q_density = 10_000     # W / m^3
sigma = 5.670374419E-8 # W / m^2 K^4
T_surf = np.power(q_density / sigma, 0.25)

def T(z):
	return (0.5 * q_density * (1.0 - (z * z))) + T_surf

center_data = np.genfromtxt('center_hg_plate.csv', delimiter=',', skip_header=1)
center_Ts = center_data[:,0][::4]
center_Zs = center_data[:,2][::4]

edge_data = np.genfromtxt('edge_hg_plate.csv', delimiter=',', skip_header=1)
edge_Ts = edge_data[:,0][::4]
edge_Zs = edge_data[:,2][::4]

corner_data = np.genfromtxt('corner_hg_plate.csv', delimiter=',', skip_header=1)
corner_Ts = corner_data[:,0][::4]
corner_Zs = corner_data[:,2][::4]


plt.plot(center_Zs, center_Ts, lw=1, marker='o', ms=6, label="Center Line", zorder=1)
plt.plot(edge_Zs, edge_Ts, lw=1, marker='o', ms=6, label="Edge Line", zorder=1)
plt.plot(corner_Zs, corner_Ts, lw=1, marker='o', ms=6, label="Corner Line", zorder=1)
zs = np.linspace(0.0, 1.0, 100)
Ts = T(zs)
plt.plot(zs, Ts, lw=1, label="Emission Analytic Solution", zorder=0)
plt.ylabel("$T(z)$ [K]")
plt.xlabel("$z$ [m]")
plt.legend()
plt.savefig("plate_com_line_samples_hg.pdf")
plt.show()

# Ts = T(center_Zs)
# plt.plot(center_Zs, center_Ts-Ts, lw=1, marker='o', ms=6, label="Center Line", zorder=1)
# plt.plot(center_Zs, edge_Ts-Ts, lw=1, marker='o', ms=6, label="Edge Line", zorder=1)
# plt.plot(center_Zs, corner_Ts-Ts, lw=1, marker='o', ms=6, label="Corner Line", zorder=1)
# plt.ylabel("$T(z) - T_{analy.}(z)$ [K]")
# plt.xlabel("$z$ [m]")
# plt.legend()
# plt.savefig("plate_com_line_sample_diff_hg.pdf")
# plt.show()