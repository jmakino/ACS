def myprint(a : Int32)
  p a
end
def myprint(a : Float)
  p a
end
def myprint(a : String)
  p a
end
a=[12345, 0.1, "This is string"]

a.each{|i| myprint(i)}
