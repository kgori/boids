Boids
=====

This is my version of the famous [Boids](https://www.red3d.com/cwr/boids/) flocking algorithm by Craig Reynolds.
I'm using [SFML](https://www.sfml-dev.org/index.php) for visualisation and to play sounds, which were recorded by
Daniel Simion (CC Attribution 3.0).

I added a quadtree to partition the space so that boids only need to interact with nearby neighbours.
