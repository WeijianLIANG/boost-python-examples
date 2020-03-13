#!/usr/bin/env python

import Omega
import time
from multiprocessing import Pool, Lock, Manager
import numpy as np



def getstress(scene_nb):

	result = Omega.get_stress()
	return result

def addstrain(para):
	scene_nb= para[0]
	dstrain = para[1]
	
	Omega.checkout_scene(scene_nb)
	Omega.set_dstrain(dstrain)


t0 = time.time()
rve_nb = 5
Omega = Omega.Omega()
n=1000
for i in range(rve_nb):
	scene_nb = Omega.creat_scene()
	Omega.checkout_scene(scene_nb)
	Omega.set_dstrain(range(n))
	a=np.asarray(Omega.get_stress())
	
	
b=a.reshape((250,4))
print b
# p = Pool(1)

# p.map(addstrain, zip(range(rve_nb),range(rve_nb)))

# p.map(addstrain, zip(range(rve_nb),range(rve_nb)*5))


print "elapse time", time.time()-t0