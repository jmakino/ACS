cat *tex | tr "\\{}/" "    " | awk -f findincludedsources.awk | sort | uniq |\
awk -f makecutfilescript.awk > tmp.csh
csh -f tmp.csh
rm tmp.csh


