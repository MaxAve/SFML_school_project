all:
	g++ -o game src/main.cpp src/player.cpp src/physics.cpp src/bullet.cpp src/window.cpp src/physical_animation.cpp -IC:\SFML\include -LC:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system
