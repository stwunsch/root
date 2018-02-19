#!/bin/bash

wget http://root.cern.ch/files/tmva_class_example.root

ARG="tmva_class_example.root "
for i in {1..99}
do
    ARG="$ARG tmva_class_example.root"
done

hadd foo.root $ARG
