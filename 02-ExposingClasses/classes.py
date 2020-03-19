#!/usr/bin/env python

import classes

t = classes.World()
t.set("bom dia!")
print t.greet()

t.many(['Good Morning', 'Buon giorno', 'Kali mera'])

t.set_strain([0.213,213,312,4,3,2])
t.show_strain()
#print t.greet()


