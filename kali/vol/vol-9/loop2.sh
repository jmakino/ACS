#!
@ i = 0
@ n = $2
while ($i < $n)
echo -n "==>" ; ( ruby mkplummer3.rb -n$1 | ruby report2.rb ) | & grep escapers
@ i += 1
end
