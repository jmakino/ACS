#
# acsstart.cshrc: startup for ACS
#
# one should source this file somewhere
#
if ( $?ACSROOT) then
  if (! $?ACSSCRIPTS ) then
    setenv ACSDOCOPTIONS --reuseoutput
    setenv ACSSCRIPTS $ACSROOT/bin
   setenv ACSLIBS $ACSROOT/lib
   set path=( $ACSSCRIPTS $path )
  endif
endif
