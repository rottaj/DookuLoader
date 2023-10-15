run: main.c
	x86_64-w64-mingw32-gcc main.c -o dooku.exe Core/Dooku.c Core/Win32.c Core/Http.c Core/Encrypt.c Cmd/ArgHandler.c -municode -ICore. -lwininet