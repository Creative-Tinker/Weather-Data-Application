Prepare:
	[wda]$ mkdir build
	[wda]$ cd build
	[wda/build]$ cmake ..

Build and run (after preparing):
	[wda/build]$ cmake --build . --config Release
	[wda/build]$ ./wda

Test (after building):
	CLI tests:
		[wda]$ test/cli.sh
	Unit tests
		[wda]$ cd build
		[wda/build]$ ctest --output-on-failure


For a user manual see "wda --help".
