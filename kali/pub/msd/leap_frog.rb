#simple ruby 2 body leapfrog  code

require "opengl"
require "glut"
include Math

#initial values for position, velocity components, and time increments
x = 1.0
y = 0.0
z = 0.0
vx = 0.0
vy = 0.5
vz = 0.0
m1 = 0.6
m2 = 1 - m1
r2 = x*x + y*y + z*z
r3 = r2*sqrt(r2)
ax = -x/r3
ay = -y/r3
az = -z/r3

#This value keeps a smooth orbit on my workstation
#Smaller values slow down the orbit, higher values speed things up
dt = 0.00025

#calculate new positions
orbits = Proc.new{
	
	vx += 0.5*ax*dt
	vy += 0.5*ay*dt
	vz += 0.5*az*dt
	x += vx*dt
	y += vy*dt
	z += vz*dt
	r2 = x*x + y*y + z*z
	r3 = r2*sqrt(r2)
	ax = -x/r3
	ay = -y/r3
	az = -z/r3
	vx += 0.5*ax*dt
	vy += 0.5*ay*dt
	vz += 0.5*az*dt
	#send x,y,z to the display
	GLUT.PostRedisplay()
}

#display the "stars" in their orbits around each other
display = Proc.new {
   GL.Clear(GL::COLOR_BUFFER_BIT)
   GL.PushMatrix()
	GL.Translate(m1*x, m1*y, z)
	#orange color for less massive star
	GL.Color3ub(245, 150, 30)
	GLUT.SolidSphere(0.02, 10.0, 10.0)
   GL.PopMatrix()
   GL.PushMatrix()
	GL.Translate(-m2*x, -m2*y, z)
	#bright yellow for more massive star
	GL.Color3ub(240, 230, 100)
	GLUT.SolidSphere(0.035, 10.0, 10.0)
   GL.PopMatrix()
   GLUT.SwapBuffers()
}

#black background and a somewhat yellow star
def init
   GL.ClearColor(0.0, 0.0, 0.0, 0.0)
end

#window reshape stuff, including view perspective
reshape = Proc.new {|w, h|
   GL.Viewport(0, 0, w,  h)
   GL.MatrixMode(GL::PROJECTION)
   GL.LoadIdentity()
   GLU.Perspective(45.0, 1.0, 1.0, 10.0)
   GLU.LookAt(0.0, 0.0, 1.75, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0)
   GL.MatrixMode(GL::MODELVIEW)

   GL.LoadIdentity()
}

keyboard = Proc.new{|key, x ,y|
	if key == 27  # 27 = 'Esc' key
		exit(0)
	elsif key == ?q
		exit(0)
	end
}

#Initialize OpenGL and GLUT
GLUT.Init
GLUT.InitDisplayMode(GLUT::DOUBLE | GLUT::RGB)
GLUT.InitWindowSize(500, 500)
GLUT.InitWindowPosition(100, 100)
GLUT.CreateWindow("Leap Frog MSA")
GLUT.DisplayFunc(display)
GLUT.ReshapeFunc(reshape)
GLUT.IdleFunc(orbits)
GLUT.KeyboardFunc(keyboard)

init();
GLUT.MainLoop()
