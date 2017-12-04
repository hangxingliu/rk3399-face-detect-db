#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#include "../src/media_handlers/media_handlers.hpp"

int oldMain(){
    int fd;
	struct v4l2_capability video_cap;

	if((fd = open("/dev/video0", O_RDWR)) == -1){
        perror("cam_info: Can't open device");
        return 1;
    }

	if(ioctl(fd, VIDIOC_QUERYCAP, &video_cap) == -1)
        perror("cam_info: Can't get capabilities");
	else
		printf("Name: '%s', Driver: '%s'\n", video_cap.card, video_cap.driver);

    close(fd);
    return 0;
}

int main() {
	Capture_Init();
	return 0;
}
