## \file
## \ingroup tutorial_tmva
## \notebook -nodraw
## This tutorial illustrates the testing workflow.
##
## \macro_code
## \macro_output
##
## \date August 2019
## \author Stefan Wunsch

import ROOT
import pickle

from tmva100_DataPreparation import variables
from tmva101_Training import load_data


# Load data
x, y_true, w = load_data("test_signal.root", "test_background.root")

# Load trained model
# TODO: Use TMVA inference here!
# from TMVA.Experimental import BDT
# bdt = BDT("BDT", "model.root")
bdt = pickle.load(open("model.pickle", "rb"))

# Make prediction
# TODO: Use TMVA inference here!
# y_pred = bdt.Compute(x)
y_pred = bdt.predict_proba(x)
y_pred = y_pred[:,1] # Take only the signal probability

# Compute ROC using sklearn
from sklearn.metrics import roc_curve, auc
fpr, tpr, _ = roc_curve(y_true, y_pred, sample_weight=w)
score = auc(fpr, tpr, reorder=True)

# Plot ROC
c = ROOT.TCanvas("roc", "", 600, 600)
g = ROOT.TGraph(len(fpr), fpr, tpr)
g.SetTitle("AUC = {:.2f}".format(score))
g.SetLineWidth(3)
g.SetLineColor(ROOT.kRed)
g.Draw("AC")
g.GetXaxis().SetRangeUser(0, 1)
g.GetYaxis().SetRangeUser(0, 1)
g.GetXaxis().SetTitle("False-positive rate")
g.GetYaxis().SetTitle("True-positive rate")
c.Draw()
