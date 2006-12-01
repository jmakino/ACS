#!/usr/local/bin/ruby -w

require "acs"

options_text = <<-END

  Description: Creates a document from YAML input using a template 
  Long description:
    This program creates a document from YAML input using a template.

    (c) 2005, Piet Hut, Jun Makino; see ACS at www.artcompsi.org

    example:
    kali  #{$0} -y index.yaml -t index.yhtml  > index.html


  Short name: 		-y
  Long name:		--yaml_file
  Value type:		string
  Variable name:	yaml_file_name
  Description:		Name of YAML input file
  Long description:
    This option specifies the input YAML file name.


  Short name: 		-t
  Long name:		--template_file
  Value type:		string
  Variable name:	template_file_name
  Description:		Name of template input file
  Long description:
    This option specifies the input template file name.

  END

clop = parse_command_line(options_text)

require "yaml"
require "pp"
require "erb"
y = YAML::load(File.open(clop.yaml_file_name))
s= File.read(clop.template_file_name)
erb = ERB.new(s, $SAFE)
print erb.result


