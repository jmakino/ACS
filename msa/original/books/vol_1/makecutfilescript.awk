{
dir = $1;
filename = $2;

print "cd " dir;
print "awk -f ../chap9/cutfile.awk " filename;
print "cd .. ";
}
