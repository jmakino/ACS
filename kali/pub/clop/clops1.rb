class Clops
	def initialize(option_definition_str, argv_array=nil)
		parse_option_definition(option_definition_str)
		parse_command_line if argv_array
	end
	
	def parse_option_definition(option_definition_str)
		@option_definition = option_definition_str
	end
	
	attr_reader :option_definition_str

end


require 'test/unit'
#:segment start: clopstests
class ClopsTests < Test::Unit::TestCase

	SIMPLE_DEFINITION = <<-END
		Description:  This is checks only the description block at the top.
		Long Description: This is checks only the description block at the top.
		  This is a longer, multi-line version of the description of the program.
	END

	def test_simple_definition_saved
		c = Clops.new(SIMPLE_DEFINITION)
		assert_equal(SIMPLE_DEFINITION, c.option_definition_str, "The simple defintion string was not saved.")
	end
end
#:segment end:
