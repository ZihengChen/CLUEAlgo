# bin/main [fileName] [dc] [d0] [deltac] [rhoc] [useGPU] [totalNumberOfEvent] [verbose]
export DC=3
export D0=5
export DELTAC=5
export RHOC=8

export TAG=100layers
# export TAG=1layer
echo "running on GPU"
nvprof -o log/runToyDetector_1000_$TAG.nvvp bin/main toyDetector_1000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_1000_$TAG.nvvp

nvprof -o log/runToyDetector_2000_$TAG.nvvp bin/main toyDetector_2000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_2000_$TAG.nvvp

nvprof -o log/runToyDetector_3000_$TAG.nvvp bin/main toyDetector_3000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_3000_$TAG.nvvp

nvprof -o log/runToyDetector_4000_$TAG.nvvp bin/main toyDetector_4000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_4000_$TAG.nvvp

nvprof -o log/runToyDetector_5000_$TAG.nvvp bin/main toyDetector_5000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_5000_$TAG.nvvp

nvprof -o log/runToyDetector_6000_$TAG.nvvp bin/main toyDetector_6000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_6000_$TAG.nvvp

nvprof -o log/runToyDetector_7000_$TAG.nvvp bin/main toyDetector_7000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_7000_$TAG.nvvp

nvprof -o log/runToyDetector_8000_$TAG.nvvp bin/main toyDetector_8000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_8000_$TAG.nvvp

nvprof -o log/runToyDetector_9000_$TAG.nvvp bin/main toyDetector_9000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_9000_$TAG.nvvp

nvprof -o log/runToyDetector_10000_$TAG.nvvp bin/main toyDetector_10000 $DC $D0 $DELTAC $RHOC 1 10 0
nvprof -i log/runToyDetector_10000_$TAG.nvvp


