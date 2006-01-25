#!/usr/local/bin/ruby -w

require "acs"
require "ymhtml"
options_text = <<-END

  Description: Test code
  Long description:
    This program creates translation templates from an html file

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
    kali  #{$0} -i index.html


  Short name: 		-i
  Long name:		--input
  Value type:		string
  Variable name:	input_file_name
  Description:		Name of HTML input file
  Long description:
    This option specifies the input HTML file name.

  Short name: 		-o
  Long name:		--output
  Value type:		string
  Variable name:	output_file_name
  Description:		Name of output files
  Long description:
    This option specifies the output file name

  END

clop = parse_command_line(options_text)

f = YmHTML::InputStream.openFile(clop.input_file_name)
parser = YmHTML::Parser.new
parser.eliminateWhiteSpace = true
a = []
 parser.parse(f)  do |t, n, d|
  a.push([t, n, d])
  p([t,n,d]) if t == :CDATA
end

a.each{|x|}


