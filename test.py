import ROOT

# Make simple dataframe
df = ROOT.RDataFrame(5)
df2 = df.Define("x", "float(tdfentry_)")
df3 = df2.Define("y", "2*int(tdfentry_)")

# Prepare node
df_rnode = ROOT.PyROOT.AsRNode(df3)
columns = ROOT.std.vector("string")()
columns.push_back("x")
columns.push_back("y")

# Register helper
helper = ROOT.PyROOT.AsNumpyHelper("float", "int")()
booking_helper = ROOT.PyROOT.BookingHelper("float", "int")()
df_npy = booking_helper.Book(df_rnode, helper, columns)

# Get structured numpy array / record array
pytuple = df_npy.GetValue()
print(repr(pytuple))
dtype = [("x", "f4"), ("y", "i4")]
# npy_array = np.array(pytuple, dtype)
