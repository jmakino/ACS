ruby mkplummer.rb -n 3 -s 42 | ruby acstail.rb > tmp0
ruby world1.rb -p 16 -t 1 < tmp0 | ruby acstail.rb > tmp1s
ruby world1.rb -p 16 -t 1 -q < tmp0 | ruby acstail.rb > tmp1w
ruby world1.rb -p 16 -t 1 -q -r < tmp1w | ruby acstail.rb > tmp2ww
ruby world1.rb -p 16 -t 2 -o 2 -q < tmp0 | ruby acstail.rb > tmp2w
ruby world1.rb -p 16 -t 2 -o 2 < tmp0 | ruby acstail.rb > tmp2s
ruby world1.rb -p 16 -t 1  < tmp1s | ruby acstail.rb > tmp2ss
ruby world1.rb -p 16 -t 1 -r < tmp1w | ruby acstail.rb > tmp2ws
ruby world1.rb -p 16 -t 1 -q < tmp1s | ruby acstail.rb > tmp2sw
ruby world1.rb -p 16 -t 1 -q -r < tmp2ww | ruby acstail.rb > tmp3www
ruby world1.rb -p 16 -t 1 -q -r < tmp2w | ruby acstail.rb > tmp3ww
ruby world1.rb -p 16 -t 3 -o 3 -q < tmp0 | ruby acstail.rb > tmp3w
echo "diff tmp2w tmp2ww | wc:"
diff tmp2w tmp2ww | wc
echo "diff tmp2ws tmp2s | wc:"
diff tmp2ws tmp2s | wc
echo "diff tmp3w tmp3ww | wc:"
diff tmp3w tmp3ww | wc
echo "diff tmp3w tmp3www | wc:"
diff tmp3w tmp3www | wc
echo "diff tmp2ss tmp2s | wc:"
diff tmp2ss tmp2s | wc
echo "cat tmp2s | wc:"
cat tmp2s | wc
