import numpy as np
import pandas as pd
import sys
import matplotlib 
from matplotlib import pyplot as plt
matplotlib.use('TkAgg')

nXPoints = 10

class DataframeRowPlotter:
    """
    Plot graphs from dataframe rows.
    """

    def __init__(self, infilepath, cols):
        tempDf = pd.read_csv(infilepath, sep='\t', usecols=cols)
        self.df = tempDf[cols]
        self.df.reset_index()
        self.cols = cols
        print("self.df=\n{}".format(self.df.head()))
        self.nXPoints = 10
        self.xpoints = [x for x in range(0, self.nXPoints)]

    def run(self, outfilepath, xlim, ylim=None, yticks=None):
        fig, axs = plt.subplots(self.df.shape[0], 1, constrained_layout=True)
        fig.set_figheight(12)
        # fig.subplots_adjust(hspace=1.5)

        for ix, row in self.df.iterrows():
            ar = row.to_numpy()
            ypoints = ar[1 : -1]  # excluding distance, autocorr
            axs[ix].plot(self.xpoints, ypoints)
            axs[ix].set_title(f"distance={ar[0]}, autocorr={ar[-1]}")

        print("outfilepath={}".format(outfilepath))
        plt.savefig(outfilepath)
        plt.show()  
        plt.close()


if len(sys.argv) != 3:
    print("Usage: python3 plotdfcols.py infilepath outfilepath")
else:
    infilepath = sys.argv[1]
    outfilepath = sys.argv[2]

    xlim = (0.8, 10.2)
    ylim = (-1.2, 1.2)
    yticks = list(np.arange(-1.2, 1.2, 0.1))

    cols = ['distance'] + [f't{timestep}' for timestep in range(0, nXPoints)] + ['autocorr']

    dp = DataframeRowPlotter(infilepath, cols)
    dp.run(outfilepath, xlim, ylim, yticks)
    print("figure is saved in {}".format(outfilepath))
