# ./main [fileName] [dc] [deltao] [deltac] [rhoc] [useGPU] [totalNumberOfEvent] [verbose]
# ./mainCuplaCPUTBB toyDetector_10000 3 5 5 8 1 10 0

export DC=3
export DELTAO=5
export DELTAC=5
export RHOC=8
export NEVENT=10

echo "----------------"
echo "running with CPU"
echo "----------------"
for i in 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000
do
  ./main toyDetector_$i $DC $DELTAO $DELTAC $RHOC 0 $NEVENT 0
done

echo "----------------"
echo "running with GPU"
echo "----------------"
for i in 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000
do
  ./main toyDetector_$i $DC $DELTAO $DELTAC $RHOC 1 $NEVENT 0
done

echo "----------------"
echo "running with CUPLA CPU TBB"
echo "----------------"
for i in 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000
do
  ./mainCuplaCPUTBB toyDetector_$i $DC $DELTAO $DELTAC $RHOC 1 $NEVENT 0
done

echo "----------------"
echo "running with CUPLA CPU Serial"
echo "----------------"
for i in 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000
do
  ./mainCuplaCPUSerial toyDetector_$i $DC $DELTAO $DELTAC $RHOC 1 $NEVENT 0
done

echo "----------------"
echo "running with CUPLA CUDA"
echo "----------------"
for i in 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000
do
  ./mainCuplaCUDA toyDetector_$i $DC $DELTAO $DELTAC $RHOC 1 $NEVENT 0
done