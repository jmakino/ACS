#!/usr/bin/env ruby
require "acs"
require "vpl"
a=[]
while s=gets do
  xx=s.split
  a.push([xx[0].to_f,xx[1].to_f])
end
VirtualPlotter.new.data(a).plot
