#include <SDL2/SDL.h>                                                           
#define SDL_MAIN_HANDLED

size_t current_audio_buffer_size = 200000;
float current_frequency = 3.f;
float current_gain = 1000.f;
float x = 0.f;


void sound_callback(void *userdata, Uint8 *stream, int len) {

  int i;

  short int *sequence = (short int*)userdata;
  
  if (len == 0) return;

  if (len > current_audio_buffer_size) {
    current_audio_buffer_size = len;
    sequence = (short int*)
      realloc(stream,current_audio_buffer_size);
  }
  
  for (i = 0; i < len/2; i++) {
      x += .010f;
      sequence[i] = sin(x*current_frequency) * current_gain;
      if(x*current_frequency > M_PI) x = 0;
  }

  memcpy(stream,sequence,len);
  
}

void color_out_image(unsigned int *image,
		     unsigned int width,
		     unsigned int height,
		     unsigned char r,
		     unsigned char g,
		     unsigned char b) {

  int i;
  unsigned int* pixel_i;
  unsigned char* pixel_c;
  
  for(i=0;i<width*height;i++) {
    pixel_i = image+i;
    pixel_c = (unsigned char*)pixel_i;
    
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

    pixel_c[0] = b;
    pixel_c[1] = g;
    pixel_c[2] = r;
    pixel_c[3] = 255;

#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    
    pixel_c[3] = b;
    pixel_c[2] = g;
    pixel_c[1] = r;
    pixel_c[0] = 255;

#endif
  }
}
  


int main(int argc, char** argv) {

  unsigned int width = 1280;
  unsigned int height = 920;

  int pitch = 4*width;
  
  SDL_Window *touchscreen_window = NULL;

  unsigned int touchscreen_window_id;

  SDL_Renderer *touchscreen_renderer = NULL;
  SDL_Texture *touchscreen_texture;

  unsigned int* touchscreen_frame;

  SDL_Event event;

  SDL_AudioDeviceID audio_device;
  SDL_AudioSpec audio_spec;

  float x;

  float current_finger_distance_delta;
  float current_finger_angle_delta;
  
  short int current_sample;
  const int sample_size = sizeof(short int);

  Uint8* audio_buffer = (Uint8*)malloc(sizeof(Uint8)*current_audio_buffer_size);
  
  SDL_SetMainReady();

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);

  SDL_zero(audio_spec);
  audio_spec.freq = 44100;
  audio_spec.format = AUDIO_S16SYS;
  audio_spec.channels = 1;
  audio_spec.samples = 128;
  audio_spec.callback = sound_callback;
  audio_spec.userdata = audio_buffer;

  audio_device = SDL_OpenAudioDevice( NULL, 0, &audio_spec, NULL, 0);

  SDL_PauseAudioDevice(audio_device, 0);
  
  touchscreen_window = SDL_CreateWindow("SDL-Theremin",100,20,width,height,0);

#ifdef SOFTWARE_RENDERING
  
  touchscreen_renderer =
    SDL_CreateRenderer(touchscreen_window,-1,SDL_RENDERER_SOFTWARE);

#else

  touchscreen_renderer =
     SDL_CreateRenderer(touchscreen_window,-1,SDL_RENDERER_ACCELERATED);

#endif

  touchscreen_texture = SDL_CreateTexture(touchscreen_renderer,
					  SDL_PIXELFORMAT_ARGB8888,
					  SDL_TEXTUREACCESS_STREAMING,
					  width, height);

  touchscreen_window_id = SDL_GetWindowID(touchscreen_window);

  SDL_LockTexture(touchscreen_texture,NULL,(void**)&touchscreen_frame, &pitch);
  color_out_image(touchscreen_frame,width,height,0,0,0);
  SDL_UnlockTexture(touchscreen_texture);

  SDL_RenderCopy(touchscreen_renderer,touchscreen_texture,NULL,NULL);
  SDL_RenderPresent(touchscreen_renderer);

  while(1) {

    while(SDL_PollEvent(&event)) {
      
      switch(event.type) {

      case SDL_QUIT:
	goto finish;
	break;

      case SDL_WINDOWEVENT:
	if(event.window.event == SDL_WINDOWEVENT_CLOSE &&
	   event.window.windowID == touchscreen_window_id ) {
	  goto finish;
	}
	break;

      case SDL_MULTIGESTURE:
	current_finger_distance_delta = event.mgesture.dDist;
        current_finger_angle_delta = event.mgesture.dTheta;

	printf("%f %f\n",current_frequency, current_gain);
	current_frequency += current_finger_distance_delta*100;
	current_gain += current_finger_angle_delta*10000;
      }

    }
    SDL_Delay(5);
  }
 finish:
  free(audio_buffer);
  SDL_CloseAudioDevice(audio_device);
  SDL_Quit();
}
	
	
    
    
  
  
    
