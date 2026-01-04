all:
	@g++ -o bin/game.exe src/main.cpp src/player.cpp src/physics.cpp src/bullet.cpp src/window.cpp src/physical_animation.cpp -lsfml-graphics -lsfml-window -lsfml-system
