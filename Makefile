fragmentos.h: ia/server.py
	python3 ia/server.py

sint7: src/*.c fragmentos.h
	gcc src/*.c -Iinclude -o sint7 \
      -lraylib -framework OpenGL -framework Cocoa \
      -framework IOKit -framework CoreVideo
