dir=$1
killall mono
 
konsole -e bash _assets/presets/compile_code.sh $dir
 


for program in $(echo "$dir"robot*.exe); do
    echo "running $program"
    mono $program &
done
