#define AUDIO_FREQUENCY     44100 // 11025,22050,44100,48000
//#define AUDIO_LATENCY       100 // ms
#define AUDIO_BUFFERLEN     _SAUDIO_DEFAULT_BUFFER_FRAMES // (AUDIO_FREQUENCY/(1000/AUDIO_LATENCY))

#define audio_queue(sample,samples) do { /* mixed samples, then [3 separated AY channels + 1 beeper] */ \
    audio_buffer1[audio_write  ] = samples[0]; \
    audio_buffer2[audio_write  ] = samples[1]; \
    audio_buffer3[audio_write  ] = samples[2]; \
    audio_buffer4[audio_write  ] = samples[3]; \
    audio_buffer5[audio_write  ] = samples[4]; \
    audio_buffer6[audio_write  ] = samples[5]; \
    audio_buffer7[audio_write  ] = samples[6]; \
    audio_bufferA[audio_write++] = sample; \
    if (audio_write >= AUDIO_BUFFERLEN) audio_write = 0, saudio_push(audio_bufferA, AUDIO_BUFFERLEN); \
} while(0)

int audio_write = 0;
float audio_buffer1[AUDIO_BUFFERLEN] = {0}; // beeper
float audio_buffer2[AUDIO_BUFFERLEN] = {0}; // AY1, ch0
float audio_buffer3[AUDIO_BUFFERLEN] = {0}; // AY1, ch1
float audio_buffer4[AUDIO_BUFFERLEN] = {0}; // AY1, ch2
float audio_buffer5[AUDIO_BUFFERLEN] = {0}; // AY2, ch0
float audio_buffer6[AUDIO_BUFFERLEN] = {0}; // AY2, ch1
float audio_buffer7[AUDIO_BUFFERLEN] = {0}; // AY2, ch2
float audio_bufferA[AUDIO_BUFFERLEN] = {0}; // all channels

void sys_audio();

void audio_quit(void) {
    saudio_shutdown();
}
void audio_init() {
    saudio_desc desc = {0};
    desc.sample_rate = AUDIO_FREQUENCY;
    desc.buffer_frames = AUDIO_BUFFERLEN; // _SAUDIO_DEFAULT_BUFFER_FRAMES(1024);
    desc.packet_frames = 128/1; // _SAUDIO_DEFAULT_PACKET_FRAMES(128);
    desc.num_packets = 64/2; // _SAUDIO_DEFAULT_NUM_PACKETS(64);
    desc.num_channels = 1;
    saudio_setup(&desc);

    printf("%d audio rate\n", saudio_sample_rate());;
    printf("%d audio frames\n", saudio_buffer_frames());;
    printf("%d audio channels\n", saudio_channels());;

    atexit(audio_quit);
}



//#include "res/audio/insert"  // S16 C1 22050Hz zxsp
//#include "res/audio/eject"   // S16 C1 22050Hz zxsp

//#include "res/audio/motor"   // S16 C1 22050Hz cap32
//enum {motor_volume = 10};
//#include "res/audio/motor2"  // S16 C1 22050Hz rvm
//enum {motor_volume = 3};
#include "res/audio/running" // S16 C1 22050Hz zxsp
enum {motor_volume = 3}; // 5

#include "res/audio/seek"    // S16 C1 22050Hz cap32
//#include "res/audio/seek2"   // S16 C1 22050Hz 

#include "res/audio/read"    // S16 C1 22050Hz
//#include "res/audio/step"    // S16 C1 22050Hz too fast

#include "res/audio/camera"    // S16 C1 22050Hz

typedef struct voice_t {
    int id;
    float *samples;
    unsigned len;   // number of samples
    unsigned count; // number of loops (0=stop, ~0u=inf)
    double pos;     // position within samples (seek)
} voice_t;

enum { voices_max = 4 };

voice_t voice[voices_max];

char *voice_info(int i) {
    int id = voice[i].id;
    return va("play %c%c%c%c x%d %f/%u %p", 
        (voice[i].id>>24)&255,(voice[i].id>>16)&255,(voice[i].id>>8)&255,(voice[i].id>>0)&255,
        voice[i].count,
        voice[i].pos, voice[i].len,
        voice[i].samples );
}

float mix(float dt) {
    float accum = 0, voices = 0;
    for( int i = 0; i < voices_max; ++i ) {
        voice_t *v = voice + i;
        if( v->count == 0 ) continue;

        v->pos += dt;

        if( v->pos >= v->len ) {
            v->pos -= v->len;
            v->count--;
        }

        if( v->count > 0 ) {
            accum += v->samples[(unsigned)v->pos]; 
            ++voices;
        }
    }

    return accum / (voices+!voices);
}

int play_voice(voice_t w, int sample_id, unsigned count) {
    voice_t *v = 0;
    // find current slot
    if( !v ) for( int i = 0; i < voices_max; ++i ) { 
        if( voice[i].id == sample_id ) {
            // already playing? update & exit
            voice[i].count = count;
            return 1;
        }
    }
    // else find free slot
    if( !v ) for( int i = 0; i < voices_max; ++i ) {
        if( voice[i].count ) continue;
        v = voice + i;
        break;
    }
    // else abort
    if( !v ) return 0;

    // update
    *v = w;

    // update markers
    v->id = sample_id;
    v->pos = 0;
    v->count = count;
    return 1;
}

voice_t* play_findvoice(int sample_id) {
    // find slot in use for current sample_id voice
    for( int i = 0; i < voices_max; ++i ) { 
        if( voice[i].id == sample_id ) {
            if( voice[i].count ) {
                return &voice[i];
            }
        }
    }
    return NULL;
}

int play_stream(int sample_id, float *data, int num_samples, unsigned count) { // data is 22khz, 16-bit mono
    if( count == 0 ) {
        voice_t *v = play_findvoice(sample_id);
        if(v) v->count = 0;
        if(v) v->id = 0;
        return !!v;
    }
    voice_t stream = {
        sample_id, data, num_samples
    };
    return play_voice(stream, sample_id, count);
}

voice_t wav2voice(unsigned id, unsigned amplify, const void *bin, int len) {
    voice_t v = {0};

    const int16_t *p = (const int16_t*)((const char*)bin + 44);
    const int num_samples = (len - 44) / 2;

    v.len = num_samples;
    v.samples = calloc(1, sizeof(float)*v.len);

    for( int i = 0; i < num_samples; ++i ) {
        // 16-bit mono [-32768..32767] to float [-1..1], then scaled by `amplify`.
        v.samples[i] = (p[i] / 32768.f) * amplify;
    }

    v.id = id;
    return v;
}

int play(int sample_id, unsigned count) {
    static voice_t  motors[1]; do_once  motors[0] = wav2voice('moto', motor_volume, wavrunning, sizeof(wavrunning));
    static voice_t   seeks[1]; do_once   seeks[0] = wav2voice('seek', 5, wavseek, sizeof(wavseek));
    static voice_t   reads[1]; do_once   reads[0] = wav2voice('read', 5, wavread, sizeof(wavread));
    static voice_t cameras[1]; do_once cameras[0] = wav2voice('cam', 1, wavcamera, sizeof(wavcamera));

    // load known samples
    /**/ if( sample_id == 'moto' ) return play_voice(motors[0], sample_id, count);
    else if( sample_id == 'seek' ) return play_voice(seeks[0], sample_id, count);
    else if( sample_id == 'read' ) return play_voice(reads[0], sample_id, count);
    else if( sample_id == 'cam' )  return play_voice(cameras[0], sample_id, count);
    else return 0;
}

void mixer_reset() {
    memset(voice, 0, sizeof(voice));
}
