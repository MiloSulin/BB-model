## What is this?
This code was written as part of a project for the course Network Theory at the University of Turku in 2024 (course code TFYS7048).
I wanted to simulate so-called Bianconi-Barabási models [^bb1] but found no pre-existing libraries that included necessary tools. Instead of changing the subject of my project I decided to create the simulation code myself.
Because I was limited on time I was forced to hard code some parameters of the model and chose to implement values found in the original studies to be better able to compare the results of my work.
Finally I also created a function that uses the C ABI to be able to call my code in Python. The final project where this simulation was used can be found [here](https://github.com/MiloSulin/nt-project) (in Finnish only).

## What exactly does it simulate?
The Bianconi-Barabási model is a dynamic growing network/graph model where each vertex is assigned a _fitness_ at its moment of creation [^bb2]. When new vertices are created they will choose which older vertices to connect to based on this fitness
as well as their _degree_ (how many links they already have). The fitness values are chosen from a distribution using the rejection method and the user can decide if this distribution is uniform or such that it produces
behaviour analogous to Bose-Einstein condensate (BEC) systems [^bb3]. When using the BEC distribution the central _beta_ parameter is also user defined. The basic two arguments required by the simulation are the number of vertices to generate
and the amount of links each new vertex creates. The network is always initialised with a ring, ten vertices each with two links, regardless of the other user defines parameters.

## How does it work and what issues does it have?
The task of choosing vertices from a constantly changing discrete proabability distribution is computationally heavy and for this I chose to implement the data structure and algorithm by _Matias et al._ [^mat].
This structure is very complicated and the need for updating many elements each iteration while keeping them organised is where most of the time is spent due to me using the STL std::unordered_map and std::map 
containers in many places. These were the easiest to use because the main goal was to simply get the tool working so that I could do my project. Containers better suited for this may exist in other 3rd party libraries
or could be implemented from scratch. Additionally there are definitely points with completely unnecessary cases of indirection which is due to inexperience on my part. In creating the data structures/classes which
make up the network structure I also didn't consider their memory layout. Despite these issues I am quite happy with the code's performance as it can simulate a network of a million vertices, each adding 2 new links,
with the more difficult BEC distribution in ~2 seconds on my machine (intel i7-8700 desktop and intel i5-12500H laptop) totalling about 4-5 seconds when called in jupyter notebook using Python.

I intend to use the experience I gained creating this simulation to either extend one of the existing network theory libraries or write my own to facilitate the simulation of general dynamic networks with more complicated
methods of growth. For more information about see for example the sources at the relevant [Wikipedia page](https://en.wikipedia.org/wiki/Preferential_attachment).


[^bb1]: https://en.wikipedia.org/wiki/Bianconi%E2%80%93Barab%C3%A1si_model
[^bb2]: https://doi.org/10.1209/epl/i2001-00260-6
[^bb3]: https://doi.org/10.1103/PhysRevLett.86.5632
[^mat]: https://doi.org/10.1007/s00224-003-1078-6
