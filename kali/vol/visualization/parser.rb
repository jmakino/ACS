# parser.rb
# (c) Pradeep Elankumaran, 2005

class CLOption
  attr_accessor :opt_short, :opt_long, :desc, :req_arg, :num_args, 
                :block
  def initialize(opt_short=nil, opt_long=nil, desc=nil, 
                 block=nil, req_arg=true, num_args=1)
    @opt_short, @opt_long = opt_short, opt_long
    @desc, @block = desc, block
    @req_arg, @num_args = req_arg, num_args
  end
  
  def arg_req?
    @req_arg end
  
  def run(arg)
    @block.call(arg) end
end

class Parser < Array
  def print_help(header)
    puts header
    self.each do |option|
      puts "#{option.opt_short}  #{option.opt_long}\t\t#{option.desc}"
    end
    exit(0)
  end
  
  def load(opt_arr)
    self.push( CLOption.new(opt_arr[0], opt_arr[1], opt_arr[2], opt_arr[3], 
                            opt_arr[4], opt_arr[5]) ) 
  end
  
  def match_opt(opt)
    self.each do |saved|
      if opt == saved.opt_short || opt == saved.opt_long then
        return saved
      end
    end
  end
  
  def parse_argv()
    argv = ARGV
    while opt = argv.shift
      if (match = match_opt(opt)) == nil then
        puts "option #{opt} not found! try reading -h or --help"
        exit(0) end
      arg = []
      match.num_args.times do 
        arg.push(argv.shift)
      end
      match.run(arg) if !match.nil? && match.num_args != 1
      match.run(arg[0]) if !match.nil? && match.num_args == 1
    end
  end
end