#define AUDIO_FREQUENCY     44100 // 11025,22050,44100,48000
//#define AUDIO_LATENCY       100 // ms
#define AUDIO_BUFFERLEN     _SAUDIO_DEFAULT_BUFFER_FRAMES // (AUDIO_FREQUENCY/(1000/AUDIO_LATENCY))

int audio_write = 0;
float audio_buffer1[0x1000*4/*AUDIO_BUFFERLEN*/] = {0}; // beeper
float audio_buffer2[0x1000*4/*AUDIO_BUFFERLEN*/] = {0}; // AY1, ch0
float audio_buffer3[0x1000*4/*AUDIO_BUFFERLEN*/] = {0}; // AY1, ch1
float audio_buffer4[0x1000*4/*AUDIO_BUFFERLEN*/] = {0}; // AY1, ch2
float audio_buffer5[0x1000*4/*AUDIO_BUFFERLEN*/] = {0}; // AY2, ch0
float audio_buffer6[0x1000*4/*AUDIO_BUFFERLEN*/] = {0}; // AY2, ch1
float audio_buffer7[0x1000*4/*AUDIO_BUFFERLEN*/] = {0}; // AY2, ch2
float audio_bufferA[0x1000*4/*AUDIO_BUFFERLEN*/] = {0}; // all channels

typedef int static_assert_ispow2[ countof(audio_bufferA) & (countof(audio_bufferA) - 1) ? -1 : 1 ];

void sys_audio();

void audio_queue(float sample,float samples[7]) { // mixed samples, then [1 beeper+x3 AY1 channels+x3 AY2 channels]
    audio_buffer1[audio_write  ] = samples[0];
    audio_buffer2[audio_write  ] = samples[1];
    audio_buffer3[audio_write  ] = samples[2];
    audio_buffer4[audio_write  ] = samples[3];
    audio_buffer5[audio_write  ] = samples[4];
    audio_buffer6[audio_write  ] = samples[5];
    audio_buffer7[audio_write  ] = samples[6];
    audio_bufferA[audio_write++] = sample;
    audio_write &= countof(audio_bufferA)-1;

    float mix(float);
    const int num_samples = saudio_expect();
    if (num_samples > 0 && audio_write >= num_samples) {
        for( int i = 0; i < num_samples; ++i )
            audio_bufferA[i] += mix(0.5);
        saudio_push(audio_bufferA, num_samples);
        audio_write -= num_samples;
        if( audio_write > 0 )
        memmove(audio_bufferA, audio_bufferA + num_samples, audio_write * sizeof(audio_bufferA[0]));
    }
}

void audio_reset(void) {
    #define memsetf memset32
    memsetf(audio_buffer1, 0, countof(audio_buffer1));
    memsetf(audio_buffer2, 0, countof(audio_buffer2));
    memsetf(audio_buffer3, 0, countof(audio_buffer3));
    memsetf(audio_buffer4, 0, countof(audio_buffer4));
    memsetf(audio_buffer5, 0, countof(audio_buffer5));
    memsetf(audio_buffer6, 0, countof(audio_buffer6));
    memsetf(audio_buffer7, 0, countof(audio_buffer7));
    memsetf(audio_bufferA, 0, countof(audio_bufferA));
    audio_write = 0;
}
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


#include "res/audio/seek"    // S16 C1 22050Hz cap32
//#include "res/audio/seek2"   // S16 C1 22050Hz 

//#include "res/audio/read"    // S16 C1 22050Hz
//#include "res/audio/step"    // S16 C1 22050Hz too fast
//#define wavread wavstep
#include "res/audio/525_step_1_1"   // S16 C1 22050Hz
#define wavread wav525_step_1_1

//#include "res/audio/motor"   // S16 C1 22050Hz cap32
#include "res/audio/motor2"  // S16 C1 22050Hz rvm
#define wavmotor wavmotor2
//#include "res/audio/running" // S16 C1 22050Hz zxsp
//#define wavmotor wavrunning

//#include "res/audio/insert"  // S16 C1 22050Hz zxsp
//#include "res/audio/eject"   // S16 C1 22050Hz zxsp

enum { FDC_VOLUME = 5 };



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
    unsigned id = voice[i].id;
    return va("play x%u %3d%% %p %x %c%c%c%c", 
        voice[i].count,
        (int)(voice[i].pos * 100 / (voice[i].len+!voice[i].len)),
        voice[i].samples,
        id,
        (id>>24)&255,(id>>16)&255,(id>>8)&255,(id>>0)&255);
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
    static voice_t  motors[1]; do_once  motors[0] = wav2voice('moto', 1*1.5*FDC_VOLUME, wavmotor, sizeof(wavmotor));
    static voice_t   seeks[1]; do_once   seeks[0] = wav2voice('seek', 3*2*FDC_VOLUME, wavseek, sizeof(wavseek));
    static voice_t   reads[1]; do_once   reads[0] = wav2voice('read', 5*2*FDC_VOLUME, wavread, sizeof(wavread));
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


/* 
 * Applies a 10kHz low-pass filter to a normalized float audio stream.
 * 
 * @param input: Float input sample in range [0, 1].
 * @param prev_output: Previous filtered output (for IIR filter state), in [-1, 1].
 * @param sample_rate: Audio sample rate in Hz (e.g., 44100).
 * @return: Filtered output sample in range [0, 1].
 */
float low_pass_filter_10khz(float input, float *prev_output, float sample_rate) {
    // Convert input from [0, 1] to bipolar [-1, 1]
    float x = 2.0f * input - 1.0f;
    
    // Calculate filter coefficient (alpha) for 10kHz cutoff
    float cutoff_freq = 10000.0f; // 10kHz
    float dt = 1.0f / sample_rate;
    float RC = 1.0f / (2.0f * 3.1415926535897932384626433832795f * cutoff_freq);
    float alpha = dt / (RC + dt);
    
    // Apply first-order IIR filter: y[n] = (1-alpha)*y[n-1] + alpha*x[n]
    float y = (1.0f - alpha) * (*prev_output) + alpha * x;
    
    // Update previous output for next iteration
    *prev_output = y;
    
    // Convert output from [-1, 1] back to [0, 1]
    return 0.5f * (y + 1.0f);
}

// Simple DC offset removal filter
// The AY-3-8910 (and similar PSG chips) often produce output with non-zero average (DC bias) due to asymmetrical square waves or envelope shapes.
// If we don't remove it:
// - The waveform can look vertically offset when plotted.
// - It can cause low-frequency rumble or speaker clicks when played back.
// This function keeps the waveform centered around zero, both for visualization and audio quality.
typedef struct {
    float prev_input;
    float prev_output;
    float alpha;
} dcr_t;
void dcr_init(dcr_t* dc, float sample_rate, float cutoff_hz) {
    float rc = 1.0f / (2.0f * 3.1415926535f * cutoff_hz);
    float dt = 1.0f / sample_rate;
    dc->alpha = rc / (rc + dt);

    dc->prev_input = 0.0f;
    dc->prev_output = 0.0f;
}
float dcr_filter(dcr_t* dc, float s) {
    float out = s - dc->prev_input + dc->alpha * dc->prev_output;
    dc->prev_input = s;
    dc->prev_output = out;
    return out;
}
