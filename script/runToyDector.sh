# bin/main [fileName] [dc] [d0] [deltac] [rhoc] [useGPU] [totalNumberOfEvent] [verbose]
export DC=3
export D0=5
export DELTAC=5
export RHOC=8

echo "running on CPU"
bin/main toyDetector_1000 $DC $D0 $DELTAC $RHOC 0 10 0
bin/main toyDetector_2000 $DC $D0 $DELTAC $RHOC 0 10 0
bin/main toyDetector_3000 $DC $D0 $DELTAC $RHOC 0 10 0
bin/main toyDetector_4000 $DC $D0 $DELTAC $RHOC 0 10 0
bin/main toyDetector_5000 $DC $D0 $DELTAC $RHOC 0 10 0
bin/main toyDetector_6000 $DC $D0 $DELTAC $RHOC 0 10 0
bin/main toyDetector_7000 $DC $D0 $DELTAC $RHOC 0 10 0
bin/main toyDetector_8000 $DC $D0 $DELTAC $RHOC 0 10 0
bin/main toyDetector_9000 $DC $D0 $DELTAC $RHOC 0 10 0
bin/main toyDetector_10000 $DC $D0 $DELTAC $RHOC 0 10 0

echo "running on GPU"
bin/main toyDetector_1000 $DC $D0 $DELTAC $RHOC 1 10 0
bin/main toyDetector_2000 $DC $D0 $DELTAC $RHOC 1 10 0
bin/main toyDetector_3000 $DC $D0 $DELTAC $RHOC 1 10 0
bin/main toyDetector_4000 $DC $D0 $DELTAC $RHOC 1 10 0
bin/main toyDetector_5000 $DC $D0 $DELTAC $RHOC 1 10 0
bin/main toyDetector_6000 $DC $D0 $DELTAC $RHOC 1 10 0
bin/main toyDetector_7000 $DC $D0 $DELTAC $RHOC 1 10 0
bin/main toyDetector_8000 $DC $D0 $DELTAC $RHOC 1 10 0
bin/main toyDetector_9000 $DC $D0 $DELTAC $RHOC 1 10 0
bin/main toyDetector_10000 $DC $D0 $DELTAC $RHOC 1 10 0
