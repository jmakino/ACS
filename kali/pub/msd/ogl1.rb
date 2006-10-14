# First Ruby OpenGL Program
# ogl1.rb

require "opengl"      #1
require "glut"        #1

display = Proc.new{
	GL.Clear(GL::COLOR_BUFFER_BIT)
	GLUT.WireTeapot(0.5)
	GL.Flush()
}

GLUT.Init
GLUT.InitDisplayMode(GLUT::SINGLE | GLUT::RGB)
GLUT.InitWindowSize(250, 250)
GLUT.InitWindowPosition(100, 100)
GLUT.CreateWindow("My First OGL Program")
GLUT.DisplayFunc(display)
GLUT.MainLoop()
