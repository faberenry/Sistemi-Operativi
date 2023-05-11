lista=()
while [[ $1 != "" ]]; do 
 lista=( "$1" "${lista[@]}" )
 shift
done 
echo "${lista[@]}"
