import ROOT

df = ROOT.RDataFrame('mini', 'mc_345060.ggH125_ZZ4lep.4lep.root')
h_nom = df.Histo1D(
        ROOT.RDF.TH1DModel('met_et', 'met_et', 20, 0, 100000), 'met_et', 'weight')
h_up = df.Define('met_et_up', 'met_et * 1.2').Histo1D(
        ROOT.RDF.TH1DModel('met_et_up', 'met_et_up', 20, 0, 100000), 'met_et_up', 'weight')
h_down = df.Define('met_et_down', 'met_et * 0.8').Histo1D(
        ROOT.RDF.TH1DModel('met_et_down', 'met_et_down', 20, 0, 100000), 'met_et_down', 'weight')

f = ROOT.TFile('met_et_shifts.root', 'RECREATE')
h_nom.Write()
h_up.Write()
h_down.Write()
f.Close()
