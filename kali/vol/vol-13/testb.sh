ruby mkplummer.rb -n 3 -s 1 | ruby acstail.rb > tmp0
ruby world1b.rb -p 16 -t 1 -r < tmp0 | ruby acstail.rb > tmp1w
ruby world1b.rb -p 16 -t 1 -r < tmp1w | ruby acstail.rb > tmp2ww
ruby world1b.rb -p 16 -t 2 -o 2 -r < tmp0 | ruby acstail.rb > tmp2w
ruby world1b.rb -p 16 -t 2 -o 2 < tmp0 | ruby acstail.rb > tmp2s
ruby world1b.rb -p 16 -t 1 < tmp1w | ruby acstail.rb > tmp2ws
echo "diff tmp2w tmp2ww | wc:"
diff tmp2w tmp2ww | wc
echo "diff tmp2ws tmp2s | wc:"
diff tmp2ws tmp2s | wc
