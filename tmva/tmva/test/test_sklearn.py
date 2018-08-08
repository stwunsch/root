import ROOT

Utility = ROOT.TMVA.Experimental.Utility
Training = ROOT.TMVA.Experimental.Training
Inference = ROOT.TMVA.Experimental.Inference


def Serialize(model, filename, name):
    model_tmva = Inference.Sklearn.MinMaxScaler("float")()
    min_ = model.min_
    min_vec = ROOT.std.vector("float")()
    for x in min_:
        min_vec.push_back(x)
    scale = model.scale_
    scale_vec = ROOT.std.vector("float")()
    for x in scale:
        scale_vec.push_back(x)

    f = ROOT.TFile(filename, "RECREATE")
    model_tmva.SetMin(min_vec)
    model_tmva.SetScale(scale_vec)
    model_tmva.SetName(name)
    model_tmva.Write()
    f.Close()


if __name__ == "__main__":
    # Create dataframes for each class
    df_sig = ROOT.RDataFrame("TreeS", "tmva_class_example.root")
    df_bkg = ROOT.RDataFrame("TreeB", "tmva_class_example.root")

    # Set up a dataloader
    variables = ROOT.std.vector("string")()
    for var in ["var1", "var2", "var3", "var4"]:
        variables.push_back(var)

    dataloader = Utility.DataLoader("float")(variables)
    dataloader.AddDataFrame(df_sig, "Signal")
    dataloader.AddDataFrame(df_bkg, "Background")

    # Get inputs
    x_sig = dataloader.GetInputs("Signal")
    x_bkg = dataloader.GetInputs("Background")

    # Get targets
    y_sig = dataloader.GetTargets("Signal", Utility.TargetType.Labeled)
    y_bkg = dataloader.GetTargets("Background", Utility.TargetType.Labeled)

    # Convert to numpy via memory adoption
    import numpy as np

    x = np.vstack((np.asarray(x_sig), np.asarray(x_bkg)))
    y = np.hstack((np.asarray(y_sig), np.asarray(y_bkg)))

    print("x: {}".format(x.shape))
    print("y: {}".format(y.shape))

    # Train sklearn variable transformation
    from sklearn.preprocessing import MinMaxScaler
    scaler = MinMaxScaler((-1, 1))
    scaler.fit(x, y)

    # Serialize to disk
    Serialize(scaler, "sklearn.root", "MinMaxScaler")

    # Load with TMVA interface (similar syntax in C++)
    scaler_tmva = Inference.Sklearn.MinMaxScaler("float")("sklearn.root",
                                                          "MinMaxScaler")

    # Compare inference results
    test = np.array([[1, 1, 1, 1]], dtype="float32")
    print("Sklearn (native): {}".format(scaler.transform(test)))
    print("Sklearn (TMVA): {}".format(
        scaler_tmva.Transform(ROOT.AsTensor(test))))
