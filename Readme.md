# Muon Lifetime Detector

## Build

Create a build folder
```
$ mkdir build
```

Then Cmake and make
```
$ cd build
$ cmake ../
$ make
```

You need to have sourced your `geant4.sh` before building

## Run
After building, an executable named `mu-raquette` should have appear in the build folder. You can run it to enter interactive mode or run `run1.mac` to run 6000000 events

```
$ build/mu-raquette

or 

$ build/mu-raquette run1.mac
```

## Results
The results of the simulation should appear in the file `buid/Results.root`

## Analysis
You can run the analysis by running in the root CLI
```
[root] .L event.C
[root] .x Analysis.cc
```

Before that you must ensure that the path in `event.h` correctly point to the root file you want tu use for the analysis