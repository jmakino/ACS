#
# acsstart.cshrc: startup for ACS
#
# one should source this file somewhere
#
if ( $?ACSROOT) then
   if (! $?ACSSCRIPTS ) then
     set path=( $ACSSCRIPTS $path )
   endif
   setenv ACSDOCOPTIONS --reuseoutput
   setenv ACSSCRIPTS $ACSROOT/bin
   setenv ACSLIBS $ACSROOT/lib
   setenv RUBYPATH $ACSSCRIPTS
   setenv RUBYLIB $ACSLIBS
endif
