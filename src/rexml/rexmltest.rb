#!/usr/bin/ruby

require 'rexml/document'

class Particle
  attr_accessor :id,:mass, :pos, :vel
  def initialize(id, mass, pos, vel)
    @id,@mass, @pos, @vel = id,mass,pos,vel
  end
  def to_xml
    xml = REXML::Element.new("particle")
    xml.attributes["id"] = @id.to_s
    xmlmass = REXML::Element.new("mass")
    xmlmass.text = @mass.to_s
    xml.add_element(xmlmass)
    xmlpos = REXML::Element.new("pos")
    xmlpos.text = @pos.join(" ")
    xml.add_element(xmlpos)
    xmlvel = REXML::Element.new("vel")
    xmlvel.text = @vel.join(" ")
    xml.add_element(xmlvel)
    xml.text = "Sample text"
    xml
  end
end

def convert_xml_to_particle_array(doc)
  parray=[]
  id = mass = pos = vel = 0
  doc.root.elements.each("particle"){|x| 
    id = x.attributes["id"].to_i
    x.elements.each("mass"){ |y| 
      mass = y.get_text().value.to_f
    }
    x.elements.each("pos"){ |y| 
      pos = y.get_text().value.split.collect{|z| z.to_f}
    }
    x.elements.each("vel"){ |y| 
      vel = y.get_text().value.split.collect{|z| z.to_f}
    }
    pp = Particle.new(id,mass,pos,vel)
    parray.push(pp)
  }
  parray
end

def convert_particle_array_to_xml(pa,doc)
  if doc == nil
    doc = REXML::Document.new "<ACS></ACS>"
    doc << REXML::XMLDecl.new
  end
  pa.each{|pp|
    id = pp.id
    found = nil
    doc.root.each_element_with_attribute('id', id.to_s){|x| found = x}
    if found == nil
      doc.root.add_element(pp.to_xml)
    else
      found.elements.each("mass"){ |y| y.text= pp.mass.to_s }
      found.elements.each("pos"){ |y| y.text= pp.pos.join(" ")}
      found.elements.each("vel"){ |y| y.text= pp.vel.join(" ")}
    end
  }
  doc
end

print "args = "
p ARGV
infile = ARGV[0]
doc = nil
if File.exist? infile
  File.open(infile) {|f|  doc = REXML::Document.new(f.read) }
else
  doc = REXML::Document.new "<ACS></ACS>"
  doc << REXML::XMLDecl.new
  doc.xml_decl.encoding = 'euc-jp'
  p = Particle.new(0,0,[1,1,1],[2,2,2]);
  doc.root.add_element(p.to_xml)
end


pa= convert_xml_to_particle_array(doc)
pa[0].mass = 2
pa[1].pos[2]=99999
convert_particle_array_to_xml(pa,doc)
doc.write($stdout,0)
