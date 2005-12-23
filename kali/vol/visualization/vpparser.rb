# vpparser.rb -> defines vp.rb-specific command line argument
# parsing functions
# (c) Pradeep Elankumaran, 2005
require 'parser.rb'

@help_header = <<ENDSTR
vp.rb-> a virtual plotting package
written by: Pradeep Elankumaran, Peter Teuben, UMCP 2005

NOTE:
by require-ing vpclass.rb in a ruby code, you can effectively
incorporate VP into your own code. look at vp.rb for an example.

usage:
vp.rb [options] < input_file
ENDSTR

def vp_parse_argv()
  parser = Parser.new
  parser.load ['-h', '--help', 'prints out help',
      Proc.new{ parser.print_help(@help_header) }, false, 0]
  parser.load ['-p', '--program', 'the program to use: gnuplot | sm | tioga', 
      Proc.new{ |arg| @prog = arg }, true, 1]
  parser.load ['-t', '--type', 'the output mode: window | file',
      Proc.new{ |arg| @type = arg }, true, 1]
  parser.load ['-o', '--output_file', 'the output filename: <string>',
      Proc.new{ |arg| @output_file = arg }, true, 1]
  parser.load ['-xl', '--xlabel', 'sets the x-axis label: <string>',
      Proc.new{ |arg| @xlabel = arg }, true, 1]
  parser.load ['-yl', '--ylabel', 'sets the y-axis label: <string>',
      Proc.new{ |arg| @ylabel = arg }, true, 1]
  parser.load ['-t', '--title', 'sets the plot title: <string>',
      Proc.new{ |arg| @title = arg }, true, 1]
  parser.load ['-xr', '--xrange', 'sets the x-axis range: <float>:<float>',
      Proc.new { |arg| 
        arg = arg.split(':').collect!{|x| x.to_f}
        @xrange = arg },
      true, 1]
  parser.load ['-yr', '--yrange', 'sets the y-axis range: <float>:<float>',
      Proc.new { |arg| 
        arg = arg.split(':').collect!{|x| x.to_f}
        @yrange = arg }, 
      true, 1]
  parser.load ['-xc', '--xcolumn', 'input file column for x-axis: <int>',
      Proc.new{ |arg| @xcol = arg.to_i }, true, 1]
  parser.load ['-yc', '--ycolumn', 'input file column for y-axis: <int>',
      Proc.new{ |arg| @ycol = arg.to_i }, true, 1]
  parser.load ['-f', '--input_filename', 'input file: <filename>',
      Proc.new{ |arg| @input_file = arg }, true, 1]
  parser.parse_argv()
end