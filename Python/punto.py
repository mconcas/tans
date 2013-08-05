from ROOT import TObject, TMath, TString

#Toolkit function(s) and controls
#Theta
def update_theta(x,y,z):
	return TMath.ACos(z/TMath.Sqrt(x*x+y*y+z*z))
#Phi
def update_phi(x,y,z):
	if x==0:
		if not y==z==0:
			return TMath.Pi()*0.5 #If it isn't the origin -> lays on the YZ plan.
	else:

#define the class
class Punto(TObject):
	"""Point Class, have a Default_Initialization. """
	def __init__(self,Name='Default_Initialization',x=0.,y=0.,z=0,cradius=0,sradius=0,theta=0,phi=0):
		print 'Punto created.'
		self.x=x
		self.y=y
		self.z=z
		#Auto-define spherical and cylindrical coordinates
		self.cradius=TMath.Sqrt(x*x+y*y)
		self.sradius=TMath.Sqrt(x*x+y*y+z*z)
		


		self.Name=Name