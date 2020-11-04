import ROOT
import json
import os

# Run before (in this order):
# - python skim.py
# - python make_met_et_hists.py

# Create a ROOT dataframe for each dataset
# It should be noted that
# - we load the filenames from the external json file placed in the same folder than this script.
# - we use preskimmed files in which the selection in tutorial df106_HiggsToFourLeptons
# is already applied.
files = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), "df106_HiggsToFourLeptons.json")))
processes = files.keys()
df = {}
samples = []
for p in processes:
    for d in files[p]:
        # Construct the dataframes
        sample = d[1] # Sample name
        samples.append(sample)

# Define systematic variations
ROOT.gInterpreter.Declare('''
auto file_met_et = TFile::Open("met_et_shifts.root", "READ");
auto met_et_nominal = file_met_et->Get<TH1D>("met_et");
auto met_et_up = file_met_et->Get<TH1D>("met_et_up");
auto met_et_down = file_met_et->Get<TH1D>("met_et_down");

float InterpolateSystematicWeight(float met_et, bool isUpShift) {
    const auto nominal = met_et_nominal->Interpolate(met_et);
    const auto ref = isUpShift ? met_et_up->Interpolate(met_et) : met_et_down->Interpolate(met_et);
    return ref / nominal;
}
''')

def build_graph(sample, variation):
    # Different reconstruction / different MC sample / variations in the analysis ntuples
    if 'ptcut' in variation:
        filename = "{}.4lep.{}.root".format(sample, variation)
    else:
        filename = "{}.4lep.root".format(sample)
    df = ROOT.RDataFrame("mini", "{}.4lep.root".format(sample))

    # Shifting a variable up/down in the analysis ntuples itself
    lep_pt_shift = 1.0
    if variation == 'lep_pt_up':
        lep_pt_shift = 0.9
    elif variation == 'lep_pt_down':
        lep_pt_shift = 1.1

    # Apply a reweighting
    weight_sys = 1.0
    if 'met_et' in variation:
        weight_sys = 'weight_met_et'
        df = df.Define('weight_met_et', 'InterpolateSystematicWeight(met_et, {})'.format(
            'true' if 'up' in variation else 'false'))

    # Parametrize the graph based on the systematics
    # NOTE: Typically you have hundreds of systematics, which makes this messy
    return (
            df
            .Define("goodlep_pt_sys", "goodlep_pt * {}".format(lep_pt_shift))
            .Define("weight_sys", "weight * {}".format(weight_sys))
            )

variations = {}
for s in samples:
    if 'data' in s:
        variations[s] = ['nominal']
    elif 'mc' in s:
        variations[s] = ['nominal',
                'met_et_up', 'met_et_down',
                'lep_pt_up', 'lep_pt_down',
                'ptcut_up', 'ptcut_down']

# Compute invariant mass of the four lepton system and make a histogram
ROOT.gInterpreter.Declare("""
using VecF_t = const ROOT::RVec<float>&;
float ComputeInvariantMass(VecF_t pt, VecF_t eta, VecF_t phi, VecF_t e)
{
    ROOT::Math::PtEtaPhiEVector p1(pt[0], eta[0], phi[0], e[0]);
    ROOT::Math::PtEtaPhiEVector p2(pt[1], eta[1], phi[1], e[1]);
    ROOT::Math::PtEtaPhiEVector p3(pt[2], eta[2], phi[2], e[2]);
    ROOT::Math::PtEtaPhiEVector p4(pt[3], eta[3], phi[3], e[3]);
    return (p1 + p2 + p3 + p4).M() / 1000;
}
""")
histos = {s: {} for s in samples}
for sample in samples:
    for var in variations[sample]:
        histos[sample][var] = (
                build_graph(sample, var)
                .Define("mass_4l", "ComputeInvariantMass(goodlep_pt_sys, goodlep_eta, goodlep_phi, goodlep_E)")
                .Histo1D(ROOT.RDF.TH1DModel(sample, "mass_4l", 24, 80, 170), "mass_4l", "weight_sys")
                )

# Run the event loop and merge histograms of the respective processes

# RunGraphs allows to run the event loops of the separate RDataFrame graphs
# concurrently. This results in an improved usage of the available resources
# if each separate RDataFrame can not utilize all available resources, e.g.,
# because not enough data is available.
ROOT.RDF.RunGraphs([histos[s][v] for s in samples for v in variations[s]])

def merge_histos(label, variation):
    h = None
    for i, d in enumerate(files[label]):
        t = histos[d[1]][variation].GetValue()
        if i == 0: h = t.Clone()
        else: h.Add(t)
    h.SetNameTitle(label, label)
    return h

merged_samples = ['data', 'higgs', 'zz', 'other']
merged_histos = {s: {} for s in merged_samples}
for s in merged_samples:
    merged_histos[s]['nominal'] = merge_histos(s, "nominal")
    if not s == 'data':
        for var in ['met_et_up', 'met_et_down', 'lep_pt_up', 'lep_pt_down', 'ptcut_up', 'ptcut_down']:
            merged_histos[s][var] = merge_histos(s, var)

# Apply MC correction for ZZ due to missing gg->ZZ process
for var in merged_histos['zz']:
    merged_histos['zz'][var].Scale(1.3)

# Create the plot

# Set styles
ROOT.gROOT.SetStyle("ATLAS")

# Create canvas with pad
c = ROOT.TCanvas("c", "", 600, 600)
pad = ROOT.TPad("upper_pad", "", 0, 0, 1, 1)
pad.SetTickx(False)
pad.SetTicky(False)
pad.Draw()
pad.cd()

# Draw stack with MC contributions
stack = ROOT.THStack()
stack_merged = None
for name, color in zip(['other', 'zz', 'higgs'], [(155, 152, 204), (100, 192, 232), (191, 34, 41)]):
    h = merged_histos[name]['nominal']
    if stack_merged == None:
        stack_merged = h.Clone()
    else:
        stack_merged.Add(h)
    h.SetLineWidth(1)
    h.SetLineColor(1)
    h.SetFillColor(ROOT.TColor.GetColor(*color))
    stack.Add(h)
stack.Draw("HIST")
stack.GetXaxis().SetLabelSize(0.04)
stack.GetXaxis().SetTitleSize(0.045)
stack.GetXaxis().SetTitleOffset(1.3)
stack.GetXaxis().SetTitle("m_{4l}^{H#rightarrow ZZ} [GeV]")
stack.GetYaxis().SetTitle("Events")
stack.GetYaxis().SetLabelSize(0.04)
stack.GetYaxis().SetTitleSize(0.045)
stack.SetMaximum(33)
stack.GetYaxis().ChangeLabel(1, -1, 0)

# Merge systematics to a single uncertainty band and draw uncertainty of the stack of MC samples
# We do here a symmetrized merging to keep it short and simple
stack_merged_up = None
stack_merged_down = None
total_uncertainty = [stack_merged.GetBinError(i)**2 for i in range(1, stack_merged.GetNbinsX() + 1)]
for s in [s for s in merged_samples if not s == 'data']:
    for var in ['met_et', 'lep_pt', 'ptcut']:
        up = merged_histos[s][var + '_up']
        down = merged_histos[s][var + '_down']
        for i in range(1, len(total_uncertainty) + 1):
            err = 0.5 * abs(up.GetBinContent(i) - down.GetBinContent(i))
            total_uncertainty[i - 1] += err**2
from math import sqrt
for i in range(1, len(total_uncertainty) + 1):
    stack_merged.SetBinError(i, sqrt(total_uncertainty[i - 1]))
stack_merged.SetMarkerSize(0)
stack_merged.SetFillColor(1)
stack_merged.SetFillStyle(3354)
stack_merged.Draw("E2 SAME")

# Draw data
data = merged_histos['data']['nominal']
data.SetMarkerStyle(20)
data.SetMarkerSize(1.2)
data.SetLineWidth(2)
data.SetLineColor(ROOT.kBlack)
data.Draw("E SAME")

# Add legend
legend = ROOT.TLegend(0.60, 0.65, 0.92, 0.92)
legend.SetTextFont(42)
legend.SetFillStyle(0)
legend.SetBorderSize(0)
legend.SetTextSize(0.04)
legend.SetTextAlign(32)
legend.AddEntry(merged_histos['data']['nominal'], "Data" ,"lep")
legend.AddEntry(merged_histos['higgs']['nominal'], "Higgs", "f")
legend.AddEntry(merged_histos['zz']['nominal'], "ZZ", "f")
legend.AddEntry(merged_histos['other']['nominal'], "Other", "f")
legend.Draw("SAME")

# Add ATLAS label
text = ROOT.TLatex()
text.SetNDC()
text.SetTextFont(72)
text.SetTextSize(0.045)
text.DrawLatex(0.21, 0.86, "ATLAS")
text.SetTextFont(42)
text.DrawLatex(0.21 + 0.16, 0.86, "Open Data")
text.SetTextSize(0.04)
text.DrawLatex(0.21, 0.80, "#sqrt{s} = 13 TeV, 10 fb^{-1}")

# Save the plot
c.SaveAs("sys.png")
