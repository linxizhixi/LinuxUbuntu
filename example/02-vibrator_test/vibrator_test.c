#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>

int main(const int argc, const char **argv)
{
	int fd;
	int strong_magnitude = 0xFFFF;
	int length = 5000;

	if (argc != 2) {
		printf("usage: %s <device-file>\n", argv[0]);
		return 1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Error opening file '%s': %s\n", argv[1], strerror(errno));
		return 1;
	}

	if (strong_magnitude > 0xFFFF)
		strong_magnitude = 0xFFFF;
	if (strong_magnitude < 0)
		strong_magnitude = 0;

	int num_effects;
	if (ioctl(fd, EVIOCGEFFECTS, &num_effects) < 0) {
		printf("Error getting number of effects playable at the same time: %s\n", strerror(errno));
		return 1;
	}
	printf("%d effects playable at the same time\n", num_effects);

	struct ff_effect effect;
	effect.type = FF_RUMBLE,
	effect.id = -1,
	effect.u.rumble.strong_magnitude = strong_magnitude;
	effect.u.rumble.weak_magnitude = 0;
	effect.replay.length = length;
	effect.replay.delay = 0;

	if (ioctl(fd, EVIOCSFF, &effect) < 0) {
		printf("Error creating new effect: %s\n", strerror(errno));
		return 1;
	}
	printf("New effect ID: %d\n", effect.id);

	struct input_event play = {
		.type = EV_FF,
		.code = effect.id,
		.value = 1
	};

	if (write(fd, (const void*) &play, sizeof(play)) < 0) {
		printf("Error writing effect to file: %s\n", strerror(errno));
		return 1;
	}

	printf("Wrote effect\n");
	sleep(3);

	return 0;
}
