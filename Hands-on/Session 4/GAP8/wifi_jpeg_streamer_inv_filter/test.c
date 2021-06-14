#-----------------------------------------------------------------------------#
# File: test.c
# Original code: Bitcraze                                                     #
# Link: https://github.com/bitcraze/AIdeck_examples/tree/master/GAP8/         #
#              /test_functionalities/wifi_jpeg_streamer                       #                                 
# Contributors:                                                               #
#          Bitcraze <contact@bitcraze.io>                                     #
#          Lorenzo Lamberti <lorenzo.lamberti@unibo.it>                       #
# Date:    10.06.2021                                                         #
#-----------------------------------------------------------------------------# 

#include "bsp/camera/himax.h"
#include "bsp/camera/mt9v034.h"
#include "bsp/transport/nina_w10.h"
#include "tools/frame_streamer.h"
#include "stdio.h"

#if defined(CONFIG_GAPUINO) || defined(CONFIG_AI_DECK)
#define CAM_WIDTH    324
#define CAM_HEIGHT   244
#else
#define CAM_WIDTH    320
#define CAM_HEIGHT   240
#endif

// uncomment if you want to invert image colors
#define INVERTING_KERNEL_FC


static pi_task_t task1;
static unsigned char *imgBuff0;
static struct pi_device camera;
static struct pi_device wifi;
static frame_streamer_t *streamer1;
static pi_buffer_t buffer;
static pi_buffer_t buffer2;
static volatile int stream1_done;
#if defined(INVERTING_KERNEL_FC)
static unsigned char *imgBuff0_inv;
static pi_buffer_t buffer_inv;
#endif


static void streamer_handler(void *arg);

void inverting(unsigned char *input, unsigned char* output, int width, int height)
{
    int idx = 0;

    for (int y = 0; y < height ; y++)
    {
        for (int x = 0; x < width ; x++)
        {
            int idx = y * width + x;

            output[idx] = 255 - input[idx];
                
        }
    }
}

static void cam_handler(void *arg)
{
  pi_camera_control(&camera, PI_CAMERA_CMD_STOP, 0);

  stream1_done = 0;
  
  inverting(imgBuff0, imgBuff0_inv, CAM_WIDTH, CAM_HEIGHT);

  frame_streamer_send_async(streamer1, &buffer_inv, pi_task_callback(&task1, streamer_handler, (void *)&stream1_done));

  return;
}



static void streamer_handler(void *arg)
{
  *(int *)arg = 1;
  if (stream1_done) // && stream2_done)
  {
    pi_camera_capture_async(&camera, imgBuff0, CAM_WIDTH*CAM_HEIGHT, pi_task_callback(&task1, cam_handler, NULL));
    pi_camera_control(&camera, PI_CAMERA_CMD_START, 0);
  }
}



static int open_pi_camera_himax(struct pi_device *device)
{
  struct pi_himax_conf cam_conf;

  pi_himax_conf_init(&cam_conf);

  cam_conf.format = PI_CAMERA_QVGA;

  pi_open_from_conf(device, &cam_conf);
  if (pi_camera_open(device))
    return -1;
  pi_camera_control(device, PI_CAMERA_CMD_AEG_INIT, 0);

  return 0;
}



static int open_pi_camera_mt9v034(struct pi_device *device)
{
  struct pi_mt9v034_conf cam_conf;

  pi_mt9v034_conf_init(&cam_conf);

  cam_conf.format = PI_CAMERA_QVGA;

  pi_open_from_conf(device, &cam_conf);
  if (pi_camera_open(device))
    return -1;
  
  return 0;
}



static int open_camera(struct pi_device *device)
{
#ifdef CONFIG_GAPOC_A
  return open_pi_camera_mt9v034(device);
#endif
#if defined(CONFIG_GAPUINO) || defined(CONFIG_AI_DECK)
  return open_pi_camera_himax(device);
#endif
  return -1;
}


static int open_wifi(struct pi_device *device)
{
  struct pi_nina_w10_conf nina_conf;

  pi_nina_w10_conf_init(&nina_conf);

  nina_conf.ssid = "";
  nina_conf.passwd = "";
  nina_conf.ip_addr = "0.0.0.0";
  nina_conf.port = 5555;
  pi_open_from_conf(device, &nina_conf);
  if (pi_transport_open(device))
    return -1;

  return 0;
}


static frame_streamer_t *open_streamer(char *name)
{
  struct frame_streamer_conf frame_streamer_conf;

  frame_streamer_conf_init(&frame_streamer_conf);

  frame_streamer_conf.transport = &wifi;
  frame_streamer_conf.format = FRAME_STREAMER_FORMAT_JPEG;
  frame_streamer_conf.width = CAM_WIDTH;
  frame_streamer_conf.height = CAM_HEIGHT;
  frame_streamer_conf.depth = 1;
  frame_streamer_conf.name = name;

  return frame_streamer_open(&frame_streamer_conf);
}
static pi_task_t led_task;
static int led_val = 0;
static struct pi_device gpio_device;
static void led_handle(void *arg)
{
  pi_gpio_pin_write(&gpio_device, 2, led_val);
  led_val ^= 1;
  pi_task_push_delayed_us(pi_task_callback(&led_task, led_handle, NULL), 500000);
}

int main_task(void) {
{
  printf("Entering main controller...\n");

  pi_freq_set(PI_FREQ_DOMAIN_FC, 150000000);

  pi_gpio_pin_configure(&gpio_device, 2, PI_GPIO_OUTPUT);

  pi_task_push_delayed_us(pi_task_callback(&led_task, led_handle, NULL), 500000);

  imgBuff0 = (unsigned char *)pmsis_l2_malloc((CAM_WIDTH*CAM_HEIGHT)*sizeof(unsigned char));
  if (imgBuff0 == NULL) {
      printf("Failed to allocate Memory for Image \n");
      return 1;
  }
  printf("Allocated Memory for Image\n");

#if (defined(INVERTING_KERNEL_FC) || defined(INVERTING_KERNEL_CLUSTER))
  imgBuff0_inv = pmsis_l2_malloc(CAM_WIDTH*CAM_HEIGHT);
  pi_buffer_init(&buffer_inv, PI_BUFFER_TYPE_L2, imgBuff0_inv);
  pi_buffer_set_format(&buffer_inv, CAM_WIDTH, CAM_HEIGHT, 1, PI_BUFFER_FORMAT_GRAY);
  if (imgBuff0_inv == NULL){ return -1;}
  printf("Allocated Memory for inverting filter buffer\n");
#endif


  if (open_camera(&camera))
  {
    printf("Failed to open camera\n");
    return -1;
  }
  printf("Opened Camera\n");

  // rotate image
  uint8_t set_value=3;
  uint8_t reg_value;

  pi_camera_reg_set(&camera, IMG_ORIENTATION, &set_value);
  pi_camera_reg_get(&camera, IMG_ORIENTATION, &reg_value);
  if (set_value!=reg_value)
  {
    printf("Failed to rotate camera image\n");
    return -1;
  }
  printf("Rotated camera image\n");


  if (open_wifi(&wifi))
  {
    printf("Failed to open wifi\n");
    return -1;
  }
  printf("Opened WIFI\n");



  streamer1 = open_streamer("camera");
  if (streamer1 == NULL)
    return -1;

  printf("Opened streamer\n");

  pi_buffer_init(&buffer, PI_BUFFER_TYPE_L2, imgBuff0);
  pi_buffer_set_format(&buffer, CAM_WIDTH, CAM_HEIGHT, 1, PI_BUFFER_FORMAT_GRAY);

  pi_camera_control(&camera, PI_CAMERA_CMD_STOP, 0);
  pi_camera_capture_async(&camera, imgBuff0, CAM_WIDTH*CAM_HEIGHT, pi_task_callback(&task1, cam_handler, NULL));
  pi_camera_control(&camera, PI_CAMERA_CMD_START, 0);
  printf("Transmission started\n");

  while(1)
  {
    pi_yield();
  }

  return 0;
}


/* Program Entry. */
int main(void) {

	printf("\n\n\t *** PMSIS Kickoff trasmission ***\n\n");
	return pmsis_kickoff((int *) main_task);
}
