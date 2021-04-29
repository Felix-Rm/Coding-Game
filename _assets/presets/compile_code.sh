dir=$1
 
echo "compiling project $1"

cd $dir
for file in robot*.cs; do
    printf "compiling $file...\n\n"
    mono-csc $file $(find . -type f \( ! -name "robot_*.cs" -name "*.cs" \))
    if (( $? != 0 )); then
        echo "error compiling $file"
        read
    else
        echo "$file compiled successfully"
    fi
    printf "\n\n\n"
done
sleep 2
    
