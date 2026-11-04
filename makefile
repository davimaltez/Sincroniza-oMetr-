all:
	gcc -pthread sync_car.c sync_car_runner.c -o sync_car

clean:
	rm -f sync_car

.PHONY: all clean