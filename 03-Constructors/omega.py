#!/usr/bin/env python

import O
import time
from multiprocessing import Pool, Lock, Manager




def getstress(scene_nb):

	time.sleep(0.)
	#print scene
	lock.acquire()
	b.checkout_scene(scene_nb)
	result = b.get_stress()
	lock.release()
	return result

def addstrain(para):
	scene_nb= para[0]
	dstrain = para[1]
	
	lock.acquire()
	b.checkout_scene(scene_nb)
	b.set_dstrain(dstrain)
	lock.release()


t0 = time.time()
rve_nb = 5

b = O.Omega()
for i in range(rve_nb):
	scene_nb = b.creat_scene()
	b.checkout_scene(scene_nb)
	b.set_dstrain(0)

def init(l):
    global lock
    lock = l


p = Pool(4)
m = Manager()
l = m.Lock()
print  p.map(getstress, zip(range(rve_nb),l))
p.map(addstrain, zip(range(rve_nb),range(rve_nb)))

print  p.map(getstress, range(rve_nb))

p.map(addstrain, zip(range(rve_nb),range(rve_nb)*5))

print  p.map(getstress, range(rve_nb))
print "elapse time", time.time()-t0