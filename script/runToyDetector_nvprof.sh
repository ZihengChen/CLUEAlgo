# bin/main [fileName] [dc] [deltao] [deltac] [rhoc] [useGPU] [totalNumberOfEvent] [verbose]
export DC=3
export DELTAO=5
export DELTAC=5
export RHOC=8
export NEVENT=10


echo "----------------"
echo "running with GPU"
echo "----------------"
for i in 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000
do
  nvprof ./main toyDetector_$i $DC $DELTAO $DELTAC $RHOC 1 $NEVENT 0
done
