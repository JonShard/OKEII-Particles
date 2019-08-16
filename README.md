# Overkill Engine II - Particles

### About
A fork of the Overkill Engine II(OKEII) here:  
https://github.com/Stektpotet/overkill-engine-II

OKEII - Particles is a fork made to show off the particle system in the small engine, adding some new functionallity to interact with the partiles on screen that doesn't belong in the base repository.  

The particle system has a data driven structure to speed up the particle data caluculation pn the CPU. The drawing of the particles are using instance rendering on the data stored in a Shader Storage Buffer(SSBO) on the GPU to draw 10'000 particles on my laptop.

![](https://github.com/JonShard/OKEII-Particles/blob/master/docs/PrettyColors2.gif?raw=true)
![](https://github.com/JonShard/OKEII-Particles/blob/master/docs/PrettyColors4.gif?raw=true)
