 # The program takes an initial word or phrase from
 # the command line (or in the absence of a
 # parameter from the first line of standard
 # input).  In then reads successive words or
 # phrases from standard input and reports whether
 # they are angrams of the first word.
 #
 # Author::    Dave Thomas  (mailto:dave@x.y)
 # Copyright:: Copyright (c) 2002 The Pragmatic Programmers, LLC
 # License::   Distributes under the same terms as Ruby

 # This class holds the letters in the original
 # word or phrase. The is_anagram? method allows us
 # to test if subsequent words or phrases are
 # anagrams of the original.

 class Anagram

   # Remember the letters in the initial word
   def initialize(text)
     @initial_letters = letters_of(text)
   end

   # Test to see if a new word contains the same
   # letters as the original
   def is_anagram?(text)
     @initial_letters == letters_of(text)
   end

   # Determine the letters in a word or phrase
   #
   # * all letters are converted to lower case
   # * anything not a letter is stripped out
   # * the letters are converted into an array
   # * the array is sorted
   # * the letters are joined back into a string

   def letters_of(text)
     text.downcase.delete('^a-z').split('').sort.join
   end
 end


 tester = Anagram.new(ARGV.shift || gets)

 ARGF.each do |text|
   puts "Anagram! " if tester.is_anagram? text
 end
