#
# cutfile.awk
#
# cut files into fuctions
#
# Usage: awk -f cutfile.awk filename.C
#
# note: the funny names starting with a number are chosen so that the
#       files appear in a useful order when you do an "ls".
#
# example:
#   typing "awk -f cutfile.awk nbody_leap1.C" gives the following output
#
#     writing top logo into file: 
#       nbody_leap1.C.0_logo_begin
#     writing initial comments into file: 
#       nbody_leap1.C.1_summary
#     writing initial stuff into file: 
#       nbody_leap1.C.2_premain
#     writing function main into file: 
#       nbody_leap1.C.3_main
#     writing function read_options into file: 
#       nbody_leap1.C.read_options
#     writing function get_snapshot into file: 
#       nbody_leap1.C.get_snapshot
#     . . . . . .
#     writing function acceleration_and_potential into file: 
#       nbody_leap1.C.acceleration_and_potential
#     writing bottom logo into file: 
#       nbody_leap1.C.0_logo_end
#
#   while "ls nbody_leap1.C.*" gives the following result:
#
#     nbody_leap1.C.0_logo_begin                nbody_leap1.C.evolve
#     nbody_leap1.C.0_logo_end                  nbody_leap1.C.evolve_step
#     nbody_leap1.C.1_summary                   nbody_leap1.C.get_snapshot
#     nbody_leap1.C.2_premain                   nbody_leap1.C.predict_step
#     nbody_leap1.C.3_main                      nbody_leap1.C.put_snapshot
#     nbody_leap1.C.acceleration_and_potential  nbody_leap1.C.read_options
#     nbody_leap1.C.correct_step                nbody_leap1.C.write_diagnostics
#
BEGIN{
# print  "fname = " ARGV[1] > "/dev/stderr";
  filename = ARGV[1];
  outfilename =  filename ".0_logo_begin";
  print "writing top logo into file: " > "/dev/stderr";
  print "  " outfilename  > "/dev/stderr";
  before_info = 1;
  print_function_name = 1;
}
{
  if (substr($1,1,20) == "*===================")
    if (before_info == 1){
      firstline = $0;
      print firstline > outfilename;
      close(outfilename);
# found end of logo. Change file name:
      funcname = "1_summary";
      outfilename = filename "." funcname ;
      print "writing initial comments into file: " > "/dev/stderr";
      print "  " outfilename  > "/dev/stderr";
      getline;
      print firstline > outfilename;
    }
  if ($1 == "*/")
    if (before_info == 1){
      before_info = 0;
# found end of comments. Change file name:
##    print firstline > outfilename;
      close(outfilename);
      getline;
# I could put a "while blank line, skip" sort-of-thing here
      funcname = "2_premain";
      outfilename = filename "." funcname ;
      print "writing initial stuff into file: " > "/dev/stderr";
      print "  " outfilename  > "/dev/stderr";
    }
  if (substr($1,1,20) == "/*------------------"){
    print_function_name = 1;
# found new function. Change file name;
##  print firstline > outfilename;
    close(outfilename);
    firstline = $0;
    getline;
# function name taken from the 2nd line of a block comment at the beginning
# of each function
    funcname = $2;
    outfilename = filename "." funcname ;
    if (funcname == "main"){
      funcname = "3_main";
      outfilename = filename "." funcname ;
      print "writing function main into file: " > "/dev/stderr";
      print "  " outfilename  > "/dev/stderr";
      print_function_name = 0;
    }
    if (funcname == "\\\\"){
      funcname = "0_logo_end";
      outfilename = filename "." funcname ;
      print "writing bottom logo into file: " > "/dev/stderr";
      print "  " outfilename  > "/dev/stderr";
      print_function_name = 0;
    }
    if (print_function_name == 1){
      print "writing function " funcname " into file: " > "/dev/stderr";
      print "  " outfilename  > "/dev/stderr";
    }
    print firstline > outfilename;
  }
  print $0 > outfilename;
}
