dati=($( ls ))
lista=()
for i in ${!dati[@]}; do 
  lista=( "${dati[$i]}" "${lista[@]}" )
done 
echo $( ls )
echo "${lista[@]}"