#!/bin/bash

#./clean.sh
mkdir -p $TANS/data/Noise_X_Multscatt_disabled_events_100K
mkdir -p $TANS/data/Noise_X_Multscatt_disabled_events_1M
root -l Punto.cxx++ Vertice.cxx++ Direzione.cxx++ Hit.cxx++ SimulationCore.cxx++ $1+
