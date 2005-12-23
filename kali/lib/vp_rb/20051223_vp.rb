#!/usr/bin/env ruby
require 'vpclass.rb'
require 'vpparser.rb'

@input_file = 'stdin'
@output_file = nil

vp_parse_argv()                # parses the command line arguments
if @type != 'window' && @output_file != nil then
  vp = VirtualPlotter.new(@prog, @output_file)  # initializes a VP class
else
  vp = VirtualPlotter.new(@prog)
end

if !@xcol.nil? && !@ycol.nil? then
  vp.data(@input_file, [@xcol,@ycol])
else
  vp.data(@input_file)
end

vp.set_limits(@xrange, @yrange) if !@xrange.nil? && !@yrange.nil? 
vp.set_labels(@xlabel, @ylabel) if !@xlabel.nil? && !@ylabel.nil?
vp.set_title(@title) if !@title.nil?            # sets the title 
vp.plot
vp.close_device