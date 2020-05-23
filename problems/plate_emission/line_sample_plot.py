import numpy as np
import matplotlib.pyplot as plt

q_density = 10_000     # W / m^3
sigma = 5.670374419E-8 # W / m^2 K^4
T_surf = np.power(q_density / sigma, 0.25)

print("T_surf =", T_surf)

def T(z):
	return (0.5 * q_density * (1.0 - (z * z))) + T_surf

zs = np.linspace(0.0, 1.0, 100)
Ts = T(zs)

plt.plot(zs, Ts, lw=1, label="Analytic", zorder=0)


num_Zs = [0.0, 0.0833333358169, 0.166666671634, 0.25, 0.333333343267, 0.416666656733,
			0.5, 0.583333313465, 0.666666686535, 0.75, 0.833333313465, 0.916666686535, 1.0]

elem_avg_Ts = [5648.03291384, 5543.86624415, 5439.69957445, 5335.53291407, 5023.03287696,
			4710.53295161, 4398.0329145, 3877.19970555, 3356.36612408, 2835.53291513,
			2106.36642258, 1377.19940848, 648.032915921]

plt.plot(num_Zs[::3], elem_avg_Ts[::3], lw=1.5, marker='o', ms=6, label="Elem. Avg. Emission", zorder=1)

local_Ts = [5648.03291384, 5543.86624414, 5439.69957445, 5335.53291407, 5023.03287696,
			4710.53295161, 4398.0329145, 3877.19970555, 3356.36612408, 2835.53291513,
			2106.36642258, 1377.19940848, 648.032915921]

plt.plot(num_Zs[::3], local_Ts[::3], lw=1.5, ls='dotted', marker='o', ms=4, label="Local Emission", zorder=2)

plt.ylabel("$T(z)$ [K]")
plt.xlabel("$z$ [m]")
plt.legend()
plt.savefig("plate_emission_line_samples.pdf")
plt.show()

