from keras.models import Sequential
from keras.layers import Dense

import ROOT
from root_numpy import tree2array

import numpy as np
np.random.seed(1234)

import threading


def generator1():
    f = ROOT.TFile("tmva_class_example.root")
    branches = ["var1", "var2", "var3", "var4"]

    signal_ = tree2array(f.Get("TreeS"), branches=branches)
    signal = np.array([list(x) for x in signal_])

    background_ = tree2array(f.Get("TreeB"), branches=branches)
    background = np.array([list(x) for x in background_])

    f.Close()

    while True:
        idx = np.random.randint(signal.shape[0], size=50)
        s = signal[idx,:]

        idx = np.random.randint(background.shape[0], size=50)
        b = background[idx,:]

        x = np.vstack([s, b])
        y = np.hstack([np.ones(s.shape[0]), np.zeros(b.shape[0])])

        yield (x, y)


class generator2:
    def __init__(self):
        f = ROOT.TFile("tmva_class_example.root")
        branches = ["var1", "var2", "var3", "var4"]

        signal_ = tree2array(f.Get("TreeS"), branches=branches)
        self.signal = np.array([list(x) for x in signal_])

        background_ = tree2array(f.Get("TreeB"), branches=branches)
        self.background = np.array([list(x) for x in background_])

        f.Close()

        self.lock = threading.Lock()

    def __iter__(self):
            return self

    def __next__(self):
        #idx = np.random.randint(self.signal.shape[0], size=3000)
        s = self.signal#[idx,:]

        #idx = np.random.randint(self.background.shape[0], size=3000)
        b = self.background#[idx,:]

        x = np.vstack([s, b])
        y = np.hstack([np.ones(s.shape[0]), np.zeros(b.shape[0])])

        return (x, y)

model = Sequential()
model.add(Dense(100, activation="relu", input_shape=(4,)))
model.add(Dense(1, activation="sigmoid"))
model.compile(loss="binary_crossentropy", optimizer="adam", metrics=["accuracy"])

model.fit_generator(generator2(), steps_per_epoch=1000, epochs=1, workers=10)
