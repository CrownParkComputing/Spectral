// # build (windows)
// cl app.c /FeSpectral.exe /O2 /MT /DNDEBUG=3 /GL /GF /arch:AVX
//
// # build (linux, debian)
// sudo apt-get install mesa-common-dev libx11-dev gcc libgl1-mesa-dev libasound2-dev
// gcc app.c -o Spectral -O3 -DNDEBUG=3 -Wno-unused-result -Wno-format -Wno-multichar -lm -ldl -lX11 -lGL -lasound -lpthread
//
// # done
// cpu, ula, mem, rom, 48/128, key, joy, ula+, tap, ay, beep, sna/128, fps, tzx, if2, zip, rf, menu, kms, z80, scr,
// key2/3, +2a/+3, fdc, dsk, autotape, gui, KL modes, load "" code, +3 fdc sounds, +3 speedlock, issue 2/3,
// pentagon, trdos, trdos (boot), translate game menus, 25/30/50/60 hz, game2exe,
// zxdb, custom tiny zxdb fmt, embedded zxdb, zxdb cache, zxdb download on demand, zxdb gallery
// ay player, pzx, rzx (wip), redefineable FN keys, mpg recording, mp4 recording, nmi, zx palettes,
// gamepads, gamepad bindings, turbosound (turboAY), autofire, alt z80 rates, media selector,
// border effects, border overscan, rainbow graphics, multicolor, HAL10H8 bugs, zoom x1..x4,
// lenslok, resizeable, ultrawide ula,
// glue sequential tzx/taps in zips (side A) -> side 1 etc)
// sequential tzx/taps/dsks do not reset model
// scan folder if dropped or supplied via cmdline

// @todo:
// [ ] pok: mask menu + undo
// [ ] zx: multiface1, multiface128/genie128, multiface3
// [ ] autofps: exolon/alien8 @ 50fps
// [x] overlay: optimize: clear & redraw only if mouse == mouse_prev
// [ ] snow: Robocop 3, iLogicAll, Fantasy World Dizzy, Astro Marine Corps, Vectron...
// [ ] coop: ghost mode (single player vs ghost) time-based games: outrun, enduro
// [ ] disasm: breakpoint on a source code expression with wildcards. ie, break on "HALT\n*\nJ*\n"
// [ ] beeper: 10khz @dmitryurbanovich4748 "The music sounds harsh only because it was never supposed to be played at such high fidelity. It was expected to be played through low-cost amplifier and a speaker, which can't produce anything above 10kHz. Also, since it is only "full on" and "off", it caused all sorts of subtle distortion in the amplifier, so it actually sounded quite warm and pleasant (in comparison to what we hear in your video)."
// [ ] media: load romtrap
// [ ] media: push/pop zx state for thumbnails and miniatures
// [x] zxdb: emit warning on failed game downloads. or, do not proceed into boot() at least
// [x] ui: adjust chrX/chrY in menus, so we can use zoom x3/x4 in low screen resolutions
// [ ] tape buttons
// [ ] tape 'tape' wav
// [ ] tape idea: add initial leading blank silence
// [ ] tape idea: do not add mega silence if "side 1" is first block (italy1990winnersedition)
// [ ] tape idea: remove most of extremely large ending silences (krakatoa)
// [ ] tape idea: eat contiguous silences while tape polarity does not change and autoplay is on (welcometohollywood)
// [ ] gallery: search should use our textinput widget
// [ ] gallery: local favourites not showing up in fav tab
// [ ] gallery: find tab misses key shortcuts, bookmark/issue icons and thumbnails mode, and filtering
// [ ] gallery: normalize zxdb/local tape numbers
// 
//  +-------------- game title: positive for zxdb ids, negative for local file ids in current folder
//  |  +----------- zxdb: 0 or id in case of multiple downloads. local: 0 or id in case of multiple files in zip
//  |  |   +------- zxdb: 0 or id in case of multiple files. local: 0 or id in case of multiple basic programs
//  |  |   |   +--- zxdb: 0 or id in case of multiple basic programs
//  |  |   |   |
// #ID#SEQ#BIN#BAS@MEDIASEEK
// 
// #zxdbID#downloadID#diskID#basID
// #zxdbID#downloadID#tapeID#basID
// 
// #localID#fileID#basID etc

// @fixme:
// [ ] zxdb: afterburner/arkanoid: cheats
// [ ] ui_print(): dims not very accurate when ui_monospaced==0. see chasehq2 instructions boundings
// [ ] mp3: @leak
// [ ] mp3: [N] 404 /pub/sinclair/music/bonustracks/DeseertIsalndFloppies(DanDefoe&TheCastaways).mp3.zip|0|27|Bonus soundtrack(s) in MP3 format
// [x] sav: mask3.sav, cybernoid2.sav, jack2.sav
// [ ] lower bottom megashock/
// [ ] turborom + bleepload (sentinel, sidewize, crosswize, jaws)

// @fixme
// [x] TurboAY + output[6]
// [x] linux tigr may ignore x2/x3 zoom settings and go x4 if (384px native * (125/100) dpi == 1920px) which matches desktop monitor
// [ ] linux polyfill (does not work yet)
// [ ] contention: gauntlet3.dsk
// [ ] floating: arkanoid way too fast, sidewize too slow and/or blinks
// [ ] floating: floatspy (all models need +1 TS; 128 also needs +2TS because of bonanzadsk)
// [ ] zxdb platoon.tap.zip with .txt file in zipentry #0
// [ ] dsk/corrupt vram: cesare plus3.dsk, italia90.dsk
// [ ] lightgun: billy the kid, rookie #4239, space smugglers
// [ ] audio: after 3 mins of play, user got white noise (jetpac)
// [x] screen: should use overlay, and any mouse button or keypress should remove that. next step would be to add a gallery for screenshots
// [ ] gallery: thumbnails 12x12 could use a magnified hovering rect
// [x] gallery: dim background as game list is difficult to read sometimes
// [ ] gallery: wordwap needed when thumbnail names are too large: "Sherlock Holmes: The Lamberley Mystery (1990)(Zenobi Software)"
// [ ] mouse: wheel linux (@juntelart)
// [ ] mouse: wheel inverted (@hexaee, @korb)
// [x] media: wrong clear media + reset model: after loading a game, then changing to another model e.g. 128K to 48K, the previous game automatically starts to load. It also happens if you reset the machine in between
// [x] ui: X2/X1 selector
// [x] ui: can the pause and record icons (top left and top right) disappear?
// [x] zxdb: ZX_MEDIA (ZX_LASTLOAD=#23#2#1)
// [x] zxdb: info gone after save state reload.
// [ ] zxdb: wrong 48/128 selection: elite, narc, nigelmansell
//     Elite would not load. This is the entry titled: Elite (1985)(Firebird Software Ltd). It starts to load but after the loader, the main block of code shows a red and blue flashing border and the loading noise goes away.
// [ ] zxdb: infer zx model for local files when the filename is an exact match: plyuk.tap [ok, so 128k], jaws.z80 [no, so 48k]
//     1st) read zxdb info (Elite: 48k/128k)
//     2nd) read filename info (Elite128_2.tzx)
//     3rd) read basic program "ELITE48" (48k)

// @timings
// [ ] overscan/border: sentinel (48), defenders of the earth (+3), dark star (hiscore), gyroscope II (?), super wonder boy (pause)
// [ ] contended mem/multicolor: venom mask3, shock megademo, MDA, action force 2, old tower, hardy
// [ ] contended IO/border: venom mask3 (border), aquaplane, olympic games, vectron, jaws, platoon (trainer), treasure island 84
// [ ] contended mem: both venom mask3 + exolon main tunes require contended memory; tempo is wrong otherwise
// [ ] floating: arkanoid (original), cobra (original), sidewize, short circuit, emlyn hughes
// [ ] detection: borderbreak, overscan demo
// [ ] multicolor title: puzznic, eliminator, cabal, bad dream
//
// title (model)                | detection | contended mem:rainbow/multicolor | contended io:overscan/border effects | flick:floating bus
// defenders of the earth (+3)                                                                                ✓
//

#if 0
// mp3 (see: https://worldofspectrum.net/pub/sinclair/music/bonustracks/)
/sinclair/music/bonustracks/720Degrees.mp3.zip
/sinclair/music/bonustracks/Afterburner.mp3.zip
/sinclair/music/bonustracks/CP-M2.2-ModuloDeEntrenamientoSide1.mp3.zip
/sinclair/music/bonustracks/CP-M2.2-ModuloDeEntrenamientoSide2.mp3.zip
/sinclair/music/bonustracks/CZSpectrum-ProgramaDeIniciacion_SideA(LongVersion).mp3.zip
/sinclair/music/bonustracks/CZSpectrum-ProgramaDeIniciacion_SideA(ShortVersion).mp3.zip
/sinclair/music/bonustracks/CaptainAmericaInTheDoomTubeOfDrMegalomann_Resister-WhosCryingNow.mp3.zip
/sinclair/music/bonustracks/CarrierCommand.mp3.zip
/sinclair/music/bonustracks/ChallengeOfTheGobots.mp3.zip
/sinclair/music/bonustracks/ChallengeOfTheGobots_LoFi.mp3.zip
/sinclair/music/bonustracks/Confuzion.mp3.zip
/sinclair/music/bonustracks/Corruption.mp3.zip
/sinclair/music/bonustracks/CusterdsQuest.mp3.zip
/sinclair/music/bonustracks/DaleyThompsonsOlympicChallenge_TheChallenge.mp3.zip
/sinclair/music/bonustracks/DeseertIsalndFloppies(DanDefoe&TheCastaways).mp3.zip
/sinclair/music/bonustracks/DeusExMachinaPart1.mp3.zip
/sinclair/music/bonustracks/DeusExMachinaPart2.mp3.zip
/sinclair/music/bonustracks/DodgyGeezers.mp3.zip
/sinclair/music/bonustracks/DoomdarksRevenge_TheStory.mp3.zip
/sinclair/music/bonustracks/EnormousTurnipThe.mp3.zip
/sinclair/music/bonustracks/EveryonesAWally.mp3.zip
/sinclair/music/bonustracks/ExtricatorThe.mp3.zip
/sinclair/music/bonustracks/FiveLittleDucks(VijfKleineEendjes)(WoltersSoftware).mp3.zip
/sinclair/music/bonustracks/FiveLittleDucks_Side1.mp3.zip
/sinclair/music/bonustracks/HoldMyHand.mp3.zip
/sinclair/music/bonustracks/InsertCoins2_OriginalSoundtrack.mp3.zip
/sinclair/music/bonustracks/JamesBond007ActionPack.mp3.zip
/sinclair/music/bonustracks/KingdomOfKrellThe.mp3.zip
/sinclair/music/bonustracks/LOKOsoftsBestCrap_BonusTrack.mp3.zip
/sinclair/music/bonustracks/MorrisMeetsTheBikers.mp3.zip
/sinclair/music/bonustracks/MusicMachineThe-DemoSong-SideA.mp3.zip
/sinclair/music/bonustracks/MusicMachineThe-DemoSong-SideB.mp3.zip
/sinclair/music/bonustracks/OutRun.mp3.zip
/sinclair/music/bonustracks/PilandInternationalAnthemThe.mp3.zip
/sinclair/music/bonustracks/SU+QuicksilvaPresentTheMegaTape.mp3.zip
/sinclair/music/bonustracks/Sabrina.mp3.zip
/sinclair/music/bonustracks/SideArms_Resister-RightThisTime.mp3.zip
/sinclair/music/bonustracks/Slingshot.mp3.zip
/sinclair/music/bonustracks/SoftCuddly.mp3.zip
/sinclair/music/bonustracks/SpectrumStarterPack1.mp3.zip
/sinclair/music/bonustracks/Starglider2_ExtendedStereoSoundTrack.mp3.zip
/sinclair/music/bonustracks/SuperBowl.mp3.zip
/sinclair/music/bonustracks/ThompsonTwinsAdventureThe_Introduction.mp3.zip
/sinclair/music/bonustracks/TombOfSyrinx.mp3.zip
/sinclair/music/bonustracks/Trantor-TheLastStormtrooper.mp3.zip
/sinclair/music/bonustracks/UnDosTresRespondaOtraVez(3-2-1)(CheetahsoftLtd).mp3.zip
/sinclair/music/bonustracks/UnDosTresRespondaOtraVez.mp3.zip
/sinclair/music/bonustracks/UncleCroucho(LadyClairSinclive&ThePiman).mp3.zip
/sinclair/music/bonustracks/Valkyrie17.mp3.zip
/sinclair/music/bonustracks/Wriggler.mp3.zip
/sinclair/music/bonustracks/automata/01-AllayPallyWally.mp3.zip
/sinclair/music/bonustracks/automata/02-BankruptStock.mp3.zip
/sinclair/music/bonustracks/automata/03-Dartz.mp3.zip
/sinclair/music/bonustracks/automata/04-DeusExMachinaOuttakes.mp3.zip
/sinclair/music/bonustracks/automata/05-ExtremelySillySong.mp3.zip
/sinclair/music/bonustracks/automata/06-Fertilizer.mp3.zip
/sinclair/music/bonustracks/automata/07-IGotBugs.mp3.zip
/sinclair/music/bonustracks/automata/08-NewWheelsJohn.mp3.zip
/sinclair/music/bonustracks/automata/09-Olympimania.mp3.zip
/sinclair/music/bonustracks/automata/10-Pi-Balled.mp3.zip
/sinclair/music/bonustracks/automata/11-Pi-Eyed.mp3.zip
/sinclair/music/bonustracks/automata/12-PutTheCatOutMother.mp3.zip
/sinclair/music/bonustracks/automata/13-SomePeople.mp3.zip
/sinclair/music/bonustracks/automata/14-ThePiracyTango.mp3.zip
/sinclair/music/bonustracks/automata/15-Angel.mp3.zip
/sinclair/music/bonustracks/automata/16-BitOfACult.mp3.zip
/sinclair/music/bonustracks/automata/17-ComputerAlphabet.mp3.zip
/sinclair/music/bonustracks/automata/18-CountryMusac.mp3.zip
/sinclair/music/bonustracks/automata/19-CrummySong.mp3.zip
/sinclair/music/bonustracks/automata/20-DonkayHotay.mp3.zip
/sinclair/music/bonustracks/automata/21-Groucho.mp3.zip
/sinclair/music/bonustracks/automata/22-LeaderOfThePack.mp3.zip
/sinclair/music/bonustracks/automata/23-Pi-BalledBlues.mp3.zip
/sinclair/music/bonustracks/automata/24-Pimania.mp3.zip
/sinclair/music/bonustracks/automata/25-PompeyRock.mp3.zip
/sinclair/music/bonustracks/automata/26-VideoNasty.mp3.zip

// Missing
// [?] Adidas Championship Football
// [?] Frankie Goes to Hollywood
// [?] Hercules
// [?] Platoon
// [?] The Biz (Virgin)
// [?] Sheepwalk (Virgin)
// [?] Starfire (Virgin)
// [?] Flying Train (RandomRecords)
// [?] The Shaky Game (Olympic)
// [?] Titanic (R&R)
// [?] Power drift
// [ ] Soft Aid (Compilation)
// [ ] SU.92 [Nov.89] Megatape 21 
// [ ] XL1 (Compilation)
#endif

#define SPECTRAL "v1.09"

#if NDEBUG >= 2
#define DEV 0
#define ifdef_DEV(t,...)  __VA_ARGS__
#define ifndef_DEV(t,...) t
#else
#define DEV 1
#define ifdef_DEV(t,...)  t
#define ifndef_DEV(t,...) __VA_ARGS__
#endif



// ref: http://www.zxdesign.info/vidparam.shtml
// https://worldofspectrum.org/faq/reference/48kreference.htm
// https://faqwiki.zxnet.co.uk/wiki/ULAplus
// https://foro.speccy.org/viewtopic.php?t=2319

// @todo:
// [x] widescreen fake borders
// [ ] animated states
// [ ] auto-saves, then F11 to rewind. use bottom bar
// [ ] live coding disasm (like bonzomatic)
// [ ] convert side-b/mp3s into voc/pulses
// [ ] db interface (F2 to rename)
//     on hover: show animated state if exists. show loading screen otherwise.
// [ ] embed torrent server/client to mirror the WOS/ZXDB/NVG/Archive.org distros
//     http://www.kristenwidman.com/blog/33/how-to-write-a-bittorrent-client-part-1/
//     https://wiki.theory.org/BitTorrentSpecification
//     http://bittorrent.org/beps/bep_0003.html
//     https://github.com/willemt/yabtorrent
//     https://github.com/jech/dht
//
// idea: when stop-block is off
// - turn autoplay=off
// - wait silently for any key/joy button to be pressed, then turn autoplay=on again
//
// profiler (auto)
// -           []
// -   []      [][]
// - [][][][][][][][][][] ...
// - 4000 4800 5000 5800  ...
// profiler (instrumented)
// IN TS
// OUT PROFILER+0, COLOR; OUT PROFILER+1, TS_DIFF
// profiler (instrumented, other; ticks get accumulated within a second; border is not rendered as usual)
// OUT 254, COLOR // on
// OUT 254, 0 // off
// analyzer
// https://www.youtube.com/watch?v=MPUL0LAUsck&ab_channel=theALFEST
// "Stuff that uses the keyboard" and "What wrote this pixel/attribute" is just awesome.
//

// todo (tapes)
// [ ] overlay ETA
// [ ] auto rewind
// [ ] auto-rewind at end of tape if multiload found (auto-stop detected)
// [ ] auto-insert next tape at end of tape (merge both during tzx_load! argv[1] argv[2])
// [ ] when first stop-the-tape block is reached, trim everything to the left, so first next block will be located at 0% progress bar
// [ ] trap rom loading, edge detection
// [ ] glue consecutive tzx/taps in disk
// [ ] glue: do not glue two consecutive tapes if size && hash match (both sides are same)
// [ ] glue: if tape1 does not start with a basic block, swap tapes
// [ ] prefer programs in tape with "128" string on them (barbarian 2; dragon ninja)
//     - if not found, and a "48" string is found, switch model to 48k automatically
// score128: 128 in filename + memcmp("19XY") X <= 8 Y < 5 + sizeof(tape) + memfind("in ayffe") + side b > 48k + program name "128" + filename128  -> load as 128, anything else: load as usr0
//      if single bank > 49k (navy seals), if size(tap)>128k multiload (outrun)
// test autotape with: test joeblade2,atlantis,vegasolaris,amc
// find 1bas-then-1code pairs within tapes. provide a prompt() call if there are more than 1 pair in the tape
//     then, deprecate 0x28 block

// notes about TESTS mode:
// - scans src/tests/ folder
// - creates log per test
// - 48k
// - exits automatically
// - 50% frames not drawn
// - 50% drawn in fastest mode
// @todo: tests
// - send keys via cmdline: "--keys 1,wait,wait,2"
// - send termination time "--maxidle 300"

// try
// https://github.com/anotherlin/z80emu
// https://github.com/kspalaiologos/tinyz80/
// https://github.com/jsanchezv/z80cpp/

// try
// https://damieng.com/blog/2020/05/02/pokes-for-spectrum/
// test(48): RANDOMIZE USR 46578
// test(2Aes): RANDOMIZE USR 20000

// try
// rodolfo guerra's roms: https://sites.google.com/view/rodolfoguerra
// paul farrow's zx81 roms: http://www.fruitcake.plus.com/Sinclair/Interface2/Cartridges/Interface2_RC_New_ZX81.htm

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#if !DEV // disable console logging in release builds (needed for linux/osx targets)
#define printf(...) 0
#define puts(...)   1
#endif

// 384x304 to fit border_break.trd resolution
enum { _256 = 384, _255 = _256-1, _257 = _256+1, _32 = (_256-256)/2 };
enum { _192 = 304, _191 = _192-1, _193 = _192+1, _24 = (_192-192)/2 };

extern struct Tigr* app;
#define _319 (_320-1)
#define _320 (app->w)
#define _321 (_320+1)
#define _239 (_240-1)
#define _240 (app->h)
#define _241 (_240+1)

#include "3rd.h"
#include "emu.h"
#include "sys.h"
#include "zx.h"
#include "app.h"

enum { OVERLAY_ALPHA = 96 };
window *app, *canvas, *ui, *dbg, *overlay, *dialog, *irc; 
int do_overlay, do_irc;

enum {
    SLOT_PNG,
    SLOT_MP4,
};

uint16_t file_counter(unsigned slot) {
    static uint16_t counter[16] = {0}; ++counter[slot %= 16];
    return counter[slot++] - 1;
}

int screenshot(const char *filename, Tigr *app) {
    uint16_t count = file_counter(SLOT_PNG);

#if 0
    time_t timer = time(NULL);
    struct tm* tm_info = localtime(&timer);

    char stamp[32];
    strftime(stamp, 32, "%Y%m%d %H%M%S", tm_info);

    extern window* app;
    int ok1 = writefile(va("%s %s %04x.scr", filename, stamp, count), VRAM, 6912);
    int ok2 = tigrSaveImage(va("%s %s %04x.png", filename, stamp, count), app);
#else
    FILE *png = fopen8(va("%s-%04x.png", filename, count), "wb");
    int ok1 = tigrSaveImageFile(png, app); if(png) fclose(png);
    int ok2 = writefile(va("%s-%04x.scr", filename, count), VRAM, 6912);
#endif

    return ok1 && ok2;
}

int save_config() {
    mkdir(".Spectral", 0777);
    int errors = 0;
    if( !ZX_PLAYER ) for( FILE *fp = fopen(".Spectral/Spectral.ini", "wt"); fp; fclose(fp), fp = 0 ) {
        #define INI_SAVE_NUM(opt) errors += fprintf(fp, "%s=%d\n", #opt, opt) != 2;
        #define INI_SAVE_STR(opt) errors += fprintf(fp, "%s=%s\n", #opt, opt?opt:"") != 2;
        INI_OPTIONS_NUM(INI_SAVE_NUM)
        INI_OPTIONS_STR(INI_SAVE_STR)
    }
    return !errors;
}
int load_config() {
    int errors = 0;
    if( !ZX_PLAYER ) for( FILE *fp = fopen(".Spectral/Spectral.ini", "rt"); fp; fclose(fp), fp = 0 ) {
        while( !feof(fp) ) {
        int tmp = 0; char buf[128] = {0}; errors += fscanf(fp, "%[^=]=%d\n", buf, &tmp) != 2;
        #define INI_LOAD_NUM(opt) if( strcmpi(buf, #opt) == 0 ) opt = tmp; else 
        INI_OPTIONS_NUM(INI_LOAD_NUM) {}
        }
        rewind(fp);
        while( !feof(fp) ) {
        char key[128] = {0},val[128] = {0}; errors += !(fscanf(fp, " %127[^=]=%127[^\n]\n", key, val) >= 1);
        #define INI_LOAD_STR(opt) if( strcmpi(key, #opt) == 0 ) opt = STRDUP(val); else 
        INI_OPTIONS_STR(INI_LOAD_STR) {}
        }
        extern int cmdkey;
        extern const char* cmdarg;
        if(ZX_FOLDER && ZX_FOLDER[0] > 32) cmdkey = 'SCAN', cmdarg = ZX_FOLDER;
    }
    return !errors;
}


bool load_overlay_rgba(const rgba *bitmap, unsigned w, unsigned h) {
    extern window *overlay;

    if( bitmap ) {
        if(overlay) tigrFree(overlay);
        overlay = tigrBitmap(w,h);
        memcpy(overlay->pix, bitmap, w * h * 4);

        tigrRenderInitMap();
        return true;
    }

    return false;
}
bool load_overlay(const void *data, int len) {
    unsigned w, h;
    if( ui_image_info(data,len,&w,&h) ) {
        rgba *bitmap = ui_image(data,len,w,h,0);
        if( bitmap ) {
            bool rc = load_overlay_rgba(bitmap, w, h);
            free( bitmap );
            return rc;
        }
    }
    return false;
}

int desktop_w, desktop_h;
int app_wouldfit(int zoom) { // given some zoom level, check whether an upcoming window would fit desktop size
    return 1;
    float scale = 0.90; // 0.90 to allow for some boundary clipping
    return (zoom * _256 * scale) <= desktop_w && (zoom * _192 * scale) <= desktop_h;
}
int app_resize() {
    if(ui) tigrFree(ui);            ui = tigrBitmap(_320, _240);
    if(dbg) tigrFree(dbg);          dbg = tigrBitmap(_320, _240);
    if(overlay) tigrFree(overlay);  overlay = tigrBitmap(_320, _240); do_overlay = 0;
    if(dialog) tigrFree(dialog);    dialog = tigrBitmap(_320, _240);
    if(irc) tigrFree(irc);          irc = tigrBitmap(_320, _240);
    tigrClear(app, tigrRGB(0,0,0));
    tigrClear(irc, tigrRGBA(0,0,0,128));
    return 1;
}
int app_create(const char *title, int fs, int zoom) {
    tigrGetDesktop(&desktop_w, &desktop_h);
    while( !app_wouldfit(zoom) ) zoom--;
    zoom += !zoom;

    if( app && (zoom == ZX_ZOOM && fs == ZX_FULLSCREEN) ) {
        // do nothing
    } else {
        // force change view
        if(app) tigrFree(app);          app = tigrWindow(_256, _192, title, window_flags(fs, zoom));  // _32+256+_32, (_24+192+_24),

        // postfx. @fixme: reload user shader too
        crt(ZX_CRT);

        app_resize();
    }

    return !!app;
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
// x1 Beeper + x3 AY1 + (x3 AY2) oscilloscopes
// inspiration: https://www.youtube.com/watch?v=EXFngtcADc8
void draw_audio(Tigr *ui, int CHANNELS, float *channels[4], int count) {
    // based on code by SteveJohn (Zen emulator)
    // MIT licensed, https://github.com/stevehjohn/Zen/blob/master/LICENSE

    static dcr_t dc[1+3+3];
    do_once
        dcr_init(&dc[0], 44100, 100.0f), // 44.1KHz samplerate, 100Hz cutoff
        dcr_init(&dc[1], 44100, 100.0f), // 44.1KHz samplerate, 100Hz cutoff
        dcr_init(&dc[2], 44100, 100.0f), // 44.1KHz samplerate, 100Hz cutoff
        dcr_init(&dc[3], 44100, 100.0f), // 44.1KHz samplerate, 100Hz cutoff
        dcr_init(&dc[4], 44100, 100.0f), // 44.1KHz samplerate, 100Hz cutoff
        dcr_init(&dc[5], 44100, 100.0f), // 44.1KHz samplerate, 100Hz cutoff
        dcr_init(&dc[6], 44100, 100.0f); // 44.1KHz samplerate, 100Hz cutoff

    unsigned white = tigrRGBA(255,255,255,32).rgba; //ui_colors[1]; //RGB(255,0,0); //ui_colors[1];
    unsigned paper = ZXPalette[ZXBorderColor];

    for( int ch = 0; ch < CHANNELS; ++ch ) {
        float *sample = channels[ch];
        dcr_t* D = &dc[ch];

#if 1
        int beginX = 0;
#else
        // find max/min sample in stream
        // display wave so max is in middle of wave
        int maxp = 0; float maxv = -1000;
        int minp = 0; float minv = 1000;
        for( int p = 0; p < count; ++p ) {
            if( sample[p] > maxv ) maxv = sample[p], maxp = p;
            if( sample[p] < minv ) minv = sample[p], minv = p;
        }
        int beginX = maxp - (maxp-minp) / 2; 
        beginX += (beginX < 0) * count;
#endif

        enum { GAIN = 4*2*2 };
        int yOff = (ch+1) * (_240 / (CHANNELS+1));
        int y0 = yOff;

        for( int x = 0; x < _320; x++ ) {
            float v = sample[(beginX + x) % count];
#if 0
            v = dcr_filter(D, v);
#endif
            int y = yOff - v * GAIN;

            if( y >= 0 && y < _240 ) {
                unsigned *dst = (unsigned *)&ui->pix[x + y * _320];
                *dst++ = white;
            }

            // draw vertical line (y0..y)
            for( int inc = y0 < y ? 1 : -1; y0 != y; y0 += inc ) {
                if( y0 >= 0 && y0 < _240 ) {
                    unsigned *dst = (unsigned *)&ui->pix[x + y0 * _320];
                    *dst++ = white;
                }
            }
        }
    }
}

// command keys: sent either physically (user) or virtually (ui)
int cmdkey;
const char *cmdarg;

const char *commands[] = {
    "♬",
    "  AY:Toggle AY core\n0:off, 1:fast, 2:accurate",
    "",
    "BOOT:Restart Game",
    "🗲",
    " CPU:Toggle Z80 speed\n0:25Hz, 1:30Hz, 2:50Hz, 3:60Hz, 4:75Hz, 5:7MHz, 6:14MHz",
    "",
    " DEV:Debugger",
    "d",
    "DEVT:Toggle DevTools",
    "",
    "FAST:Toggle FastLoad\n0:off, 1:faster loading speed",
    "",
    "FIRE:Toggle Autofire\n0:off, 1:slow, 2:fast, 3:faster",
    PLAY_STR, //PAUSE_STR,
    "GAME:Game Browser",
    "\xB",
    "GUNS:Toggle Lightgun\n0:off, 1:lightgun+gunstick",
    "i",
    "HELP:About Dialog",
    JOYSTICK_STR,
    " JOY:Toggle Joysticks\n0:off, 1:cursor, 2:kempston, 3:custom",
    "k",
    "KEYB:Toggle Keyboard Issue 2/3\n2:earlier, 3:classic keyboard",
    "L",
    "  KL:Toggle 48-BASIC input mode\nK:token based, L:letter based",
    "⭡",
    "LOAD:Load savegame",
    "\x9",
    "MICE:Toggle Mouse\n0:off, 1:kempston",
    "",
    "MODE:Toggle Model\n16, 48, 128, +2, +2A, +3, Pentagon\n",
    PLAY_STR "\b\b\b" PLAY_STR "\b\b\b",
    "NEXT:Find next tape block",
    "",
    " NMI:Magic button (NMI)",
    "",
    "PAD0:Gamepad bindings",
    "",
    " PAL:Toggle Palette\n0:own, ...:others",
    SNAP_STR,
    " PIC:Screenshot",
    PLAY_STR,
    "PLAY:Play Tape",
    "+",
    "POLR:Toggle Tape Polarity",
    "\xf\b\b\b\xf\b\b\b",
    "PREV:Find previous tape block",
    "R",
    " REC:VideoREC\nRecords session to mp4 file (no sound)",
    "🯇",
    " RUN:Toggle Run-Ahead\n0:off, 1:improved input latency",
    "⭣",
    "SAVE:Save Game",
    FOLDER_STR,
    "SCAN:Scan local folder",
    "■",
    "STOP:Stop Tape",
    "T",
    "TENG:Translate game menu\n0:off, 1:poke game menu into English",
    "",
    "TURB:Toggle TurboROM\n0:off, 1:TurboROM .tap loader",
    "▒",
    "  TV:Toggle TV mode\n0:off, 1:rf, 2:crt, 3:rf+crt",
    "U",
    " ULA:Toggle ULAplus\n0:off, 1:on, 2:on+ultrawide",
    "",
    "WIPE:Clear Medias",
//  "X",
//  "POKE:",
};

void vk_redefine2(const char *arg) {
    int vk, cmd, replace;
    if( sscanf(arg, "%d %d", &vk, &cmd)) {
        replace = vk < 0;
        vk = abs(vk);

        int in_use = 0;
        for( int j = 0; j < countof(ZX_FN); ++j ) in_use |= ( j != vk && cmd == ZX_FN[j] );
        if( cmd && in_use && !replace ) {
            ui_dialog_new(NULL);
            ui_dialog_option(0, "Command already bound to another key. Assign anyways?\n", NULL, 0,NULL);
            ui_dialog_separator();
            ui_dialog_option_ex(1, "Yes, continue\n", NULL, vk_redefine2,va("-%s",arg));
            ui_dialog_option(1, "Cancel\n", NULL, 0,NULL);
        }
        else {
            ZX_FN[vk] = cmd;
        }
    }
}
void vk_redefine(unsigned vk) { // 0=ESC,1..12=F1..F12
    ui_dialog_new(va("- Bind command to \5%s\7 key -\n",ZX_FN_STR[vk]));
    enum { COLUMNS = 3 };
    int num_commands = countof(commands)/2;
    for( int i = 0; i < num_commands; ++i ) {
        const char *icon = commands[i*2+0];
        const char *info = commands[i*2+1];
        union { struct { byte a,b,c,d; }; unsigned fourcc; } tag;
        tag.fourcc = bswap32(*((unsigned*)info));
        tag.a *= tag.a != 0x20;
        tag.b *= tag.b != 0x20;
        tag.c *= tag.c != 0x20;
        tag.d *= tag.d != 0x20;

        int in_use = 0;
        for( int j = 0; j < countof(ZX_FN); ++j ) in_use |= ( j != vk && tag.fourcc == ZX_FN[j] );

        int lf = !((i+1)%COLUMNS);
        char *hint = va("%s",info+5); if(strchr(hint,'\n')) *strchr(hint,'\n') = '\0';
        ui_dialog_option_ex(1,va("%c%s %.*s%s",in_use ? '\2' : tag.fourcc == ZX_FN[vk] ? '\5':'\7',icon,4,info, lf ? "\n":""),hint,vk_redefine2,va("%d %d",vk,tag.fourcc));
    }
    for( int i = COLUMNS-(num_commands%COLUMNS); i--; ) {
        ui_dialog_option_ex(0,i?"      ":"      \n",NULL,0,0);
    }
    ui_dialog_separator();
    ui_dialog_option_ex(1,"Reset\n",NULL,vk_redefine2,va("%d %d",vk,0));
    ui_dialog_option(1,"Cancel\n",NULL,0,0);
}
int vk_find( int vk ) {
    union { struct { uint8_t d,c,b,a; }; unsigned u; } x; x.u = bswap32(vk);
    x.a += ' ' * !x.a;
    x.b += ' ' * !x.b;
    x.c += ' ' * !x.c;
    x.d += ' ' * !x.d;

    for( int j = 0; j < countof(commands); ++j ) {
        if( memcmp(commands[j], &x.d, 4) == 0 ) {
            return j;
        }
    }
    return -1;
}
void input() {
    // keyboard
    ZXKeyboardClear();

    // gamepad
    const unsigned pad = gamepad(), pad2 = pad;

        char* keys = tigrKeys(app);

        // custom gamepad mappings
        if( pad ) {
        if( pad&0x0001 ) keys[ZX_PAD[ 0]]|=1;
        if( pad&0x0002 ) keys[ZX_PAD[ 1]]|=1;
        if( pad&0x0004 ) keys[ZX_PAD[ 2]]|=1;
        if( pad&0x0008 ) keys[ZX_PAD[ 3]]|=1;
        if( pad&0x0010 ) keys[ZX_PAD[ 4]]|=1;
        if( pad&0x0020 ) keys[ZX_PAD[ 5]]|=1;
        if( pad&0x0040 ) keys[ZX_PAD[ 6]]|=1;
        if( pad&0x0080 ) keys[ZX_PAD[ 7]]|=1;
        if( pad&0x0100 ) keys[ZX_PAD[ 8]]|=1;
        if( pad&0x0200 ) keys[ZX_PAD[ 9]]|=1;
        if( pad&0x0400 ) keys[ZX_PAD[10]]|=1;
        if( pad&0x0800 ) keys[ZX_PAD[11]]|=1;
        if( pad&0x1000 ) keys[ZX_PAD[12]]|=1;
        if( pad&0x2000 ) keys[ZX_PAD[13]]|=1;
        if( pad&0x4000 ) keys[ZX_PAD[14]]|=1;
        if( pad&0x8000 ) keys[ZX_PAD[15]]|=1;
        }

    if( !ZX_DEVTOOLS ) {

        // autofire
        static byte frame = 0;
        int autofire = 0;
        /**/ if(ZX_AUTOFIRE == 1) autofire = !!(++frame & 0x08);
        else if(ZX_AUTOFIRE == 2) autofire = !!(++frame & 0x04);
        else if(ZX_AUTOFIRE == 3) autofire = !!(++frame & 0x02);

        // joysticks
        int left = keys[TK_LEFT], right = keys[TK_RIGHT];
        int up   = keys[TK_UP],   down = keys[TK_DOWN];
        int fire = keys[TK_TAB];
        ZXJoysticks(up,down,left,right,fire ^ (fire && ZX_AUTOFIRE ? autofire : 0));

        // keyboard
        #define KEYS(k) \
            k(0)k(1)k(2)k(3)k(4)k(5)k(6)k(7)k(8)k(9)\
            k(A)k(B)k(C)k(D)k(E)k(F)k(G)k(H)k(I)k(J)\
            k(K)k(L)k(M)k(N)k(O)k(P)k(Q)k(R)k(S)k(T)\
            k(U)k(V)k(W)k(X)k(Y)k(Z)
        #define K(x) if(keys[ 0[#x] ]) ZXKey(ZX_##x);
        KEYS(K);
        if(keys[TK_SPACE])      {ZXKey(ZX_SPACE); /*if(mic_on) mic_on = 0, tap_prev();*/ }
        if(keys[TK_BACKSPACE])  {ZXKey(ZX_SHIFT); ZXKey(ZX_0);}
        if(keys[TK_RETURN])      ZXKey(ZX_ENTER);
        if(keys[TK_SHIFT])       ZXKey(ZX_SHIFT);
        if(keys[TK_CONTROL])     ZXKey(ZX_SYMB);
        if(keys[TK_ALT])         ZXKey(ZX_CTRL);
    }

    // z80_opdone() returns 0 indefinitely while z80 is in halt state, often seen during BASIC sessions.
    // hack: force a benign keypress when user wants to close window; so z80_opdone() returns 1 hopefully.
    if( 0 && window_closed() ) {
        static int flip = 0; if( flip ^= 1 ) ZXKey(ZX_SHIFT);
    }

    // prepare command keys

    #define KEYBINDING(N) \
    if( key_longpress( TK_F##N) ) cmdkey = 'F00' + N; /*vk_redefine(N);*/ \
    else \
    if( key_released(  TK_F##N) ) if(!/*dialog_*/num_options) if((cmdkey = ZX_FN[N]) == 0) cmdkey = 'F00' + N;

    KEYBINDING( 1);
    KEYBINDING( 2);
    KEYBINDING( 3);
    KEYBINDING( 4);
    KEYBINDING( 5);
    KEYBINDING( 6);
    KEYBINDING( 7);
    KEYBINDING( 8);
    KEYBINDING( 9);
    KEYBINDING(10);
    KEYBINDING(11);
    KEYBINDING(12);
}




float fps;





void help() {
    int total = numok+numwarn+numerr;

    ui_dialog_new(NULL);
    ui_dialog_option(0,"<Spectral " SPECTRAL " (Public Domain).\n",NULL, 0,NULL );
    ui_dialog_option(1,"<https://spectral.zxe.io\n\n",NULL, 'LINK',"https://spectral.zxe.io" );
    ui_dialog_option(0,va("<ZXDB Library %s: %d entries\n",ZXDB_VERSION + countof("version"),zxdb_count()), NULL, 0,NULL );
    ui_dialog_option(0,va("<Local Library: %d games found (%d%%%% ✓)\n\n",numgames, 100 - (numerr * 100 / (total + !total))), NULL, 0,NULL );
    ui_dialog_option(0,"<ESC" PLAY_STR "\f","Game browser", 0,NULL );
    for( int i = 1; i <= 12; ++i ) {
        int cmd = vk_find(ZX_FN[i]);
        const char *icon = cmd >= 0 ? commands[cmd - 1] : "";
        const char *text = cmd >= 0 ? commands[cmd] + 5 : NULL;
        if( text && strchr(text, '\n') ) text = va("%.*s", (int)(strchr(text, '\n') - text), text);
        ui_dialog_option(1,va("\b\bF%d%s",i,icon),text, 'F00'+i,NULL );
    }
    ui_dialog_option(0,"\n\n\n",NULL, 0,NULL );
    ui_dialog_ok();
}

void titlebar(const char *filename) {
    filename = filename ? filename : "";
    const char *basename = strrchr(filename, *DIR_SEP_); basename += !!basename;
    const char *title = basename ? basename : filename;
    const char *models[] = { [1]="16",[3]="48",[8]="128",[9]="P128",[12]="+2",[13]="+2A",[18]="+3" };
    const char *titlebar = ZX_PLAYER ? __argv[0] : va("Spectral%s %s%s%s", DEV ? " DEV" : "", models[(ZX/16)|ZX_PENTAGON], title[0] ? " - " : "", title);
    window_title(titlebar);

    (free(ZX_TITLE), ZX_TITLE = strdup(title));
}

void draw_ui() {

    enum { UI_X = 0, UI_Y = 0 };

    // draw_compatibility_stats(ui);

    // ui
    int UI_LINE1 = (ZX_CRT ? 2 : 0); // first visible line

    struct mouse m = mouse();
    if( m.cursor == 0 ) {
        m.x = _320/2, m.y = _240/2; // ignore mouse; already clipped & hidden (in-game)
    } else {
        if( !active ) mouse_cursor(1);
    }

    // ui animation
    enum { _60 = 58+8-4 };
    int hovering_border = !active && !do_overlay && (m.x > (_320 - _60) || m.x < _60 );
    static float smooth; do_once smooth = hovering_border;
    smooth = smooth * 0.75 + hovering_border * 0.25;
    // left panel: game options
    if( smooth > 0.1 )
    {
        {
            // draw black panel
            TPixel transp = { 0,0,0, 192 * smooth };
            tigrFillRect(ui, -1,-1, smooth * _60, _240+2, transp);
            tigrLine(ui, smooth * _60-2,-1,smooth * _60-2,_240+2, ((TPixel){255,255,255,240*smooth}));
        }

        // left panel
        float chr_x = REMAP(smooth,0,1,-6,0.5) * 11, chr_y = REMAP(smooth,0,1,-4,2.5+1-0.5) * 11 + 2;
        int right = chr_x+8*4-4;
//        int bottom = chr_y+8*31.0-1;
        int bottom = _240-11*4.5+chr_y; // +8*31.0-1;

        ui_at(ui,chr_x-2,chr_y-11*2.5-2-1-2);
        //ui_at(ui, chr_x, chr_y);
        if( ZX_BROWSER == 2 ) {
            // ZXDB builds
            if( ui_click(active ? "Resume" : "Pause", active ? PLAY_STR : PAUSE_STR) ) cmdkey = 'GAME'; // active ^= 1, ui_dialog_new(NULL);
        } else {
            // NOZXDB builds
            if( !numgames )
            if( ui_click("- Scan games folder -", "%c\n", FOLDER_CHR) ) cmdkey = 'SCAN';
            if(  numgames )
            if( ui_click(active ? "Resume" : "Pause", active ? PLAY_STR : PAUSE_STR) ) cmdkey = 'GAME'; // active ^= 1, ui_dialog_new(NULL);
        }

        // tape controls, top-left
        if( 0 && tape_inserted() ) {
            ui_at(ui,chr_x-4+1+1,chr_y-11*2-4+1);
            ui_label("     \f\f");
            // ui_at(ui,chr_x-4+1+1,bottom+1);
            //if( ui_click(NULL, REVPLAY_STR "\b\b\b" REVPLAY_STR "\f") ) cmdkey = 'PREV';
            if( ui_click(tape_playing() ? "Stop tape" : "Play tape", tape_playing() ? "■"/*PLAY_STR*/ "\f" : PLAY_STR "\b\b\b" PLAY_STR "\f" ) ) cmdkey = tape_playing() ? 'STOP' : 'PLAY';
            //if( ui_click(NULL, PLAY_STR "\b\b\b" PLAY_STR) ) cmdkey = 'NEXT';
        }

        // top-left zxdb column
        ui_at(ui,chr_x,chr_y);

        int len;

        if( ZXDB.ids[0] ) {

        const char *roles[] = {
            ['?'] = "",
            ['C'] = "Code: ",
            ['D'] = "Design: ",
            ['G'] = "Graphics: ",
            ['A'] = "Inlay: ",
            ['V'] = "Levels: ",
            ['S'] = "Screen: ",
            ['T'] = "Translation: ",
            ['M'] = "Music: ",
            ['X'] = "Sound Effects: ",
            ['W'] = "Story Writing: ",
        };

        // zxdb
        if( ui_click(va("- %s -", ZXDB.ids[0]), "ZXDB"));
        {
        char *link = va("- Visit game page -\nhttps://spectrumcomputing.co.uk/entry/%s", ZXDB.ids[0]);
        if( ui_click(link, "\f\f\x19\n")) visit(link + countof("- Visit game page-\n"));
        }
        if( ui_click(va("- %s -", ZXDB.ids[2]), "Title\n"));
        if( ZXDB.ids[3][0] )
        if( ui_click(va("- %s -", ZXDB.ids[3]), "Alias\n"));
        if( ui_click(va("- %s -", ZXDB.ids[1]), "Year\n"));
        if( ui_click(va("- %s -", ZXDB.ids[4]), "Brand\n"));

        if( ui_click(va("- %s -", ZXDB.ids[7] + strspn(ZXDB.ids[7],"0123456789")), "Genre\n"));
        if( ZXDB.ids[6][0] && ui_click(va("- %s -", ZXDB.ids[6]), "Score\n"));

        if( ZXDB.authors[0] ) {
            if( ZXDB.authors[1] ) {
                char text[(1+9)*64] = {0}, *ptr = text;
                for( int i = 0; i < 9/*countof(ZXDB.authors)*/; ++i ) {
                    if( i == 0 )
                        ptr += snprintf(ptr, 64, "- Credits -\n");
                    if( ZXDB.authors[i] )
                        ptr += snprintf(ptr, 64, "%s%s\n",roles[ZXDB.authors[i][0]],ZXDB.authors[i]+1);
                }
                if( ui_click(text, "Team\n"));
            } else {
                int i = 0;
                if( ui_click(va("- %s%s -",roles[ZXDB.authors[i][0]],ZXDB.authors[i]+1), "Author\n"));
            }
        }

        if( ZXDB.ids[8] )
        if( ui_click(ZXDB.ids[8], "Tags\n"));
//          if( ui_click("- AY Sound -", "Feat.\n"));
//          if( ui_click("- Multicolour (Rainbow Graphics) -", "Feat.\n"));

        if( ui_click(va("- %s -", strchr(ZXDB.ids[5], ',')+1), "Model\n"));
        if( ui_click("- Change media -", "Media\n")) cmdkey = 'LIST', cmdarg = va("#%s", ZXDB.ids[0]);

        if( zxdb_url(ZXDB, "inlay") && ui_click("- Toggle Inlay -", "Inlay\n")) { // @todo: include scanned instructions and tape scan
            for( char *data = zxdb_download(ZXDB,zxdb_url(ZXDB, "inlay"), &len); data; free(data), data = 0 ) {
                do_overlay ^= 1;
                tigrClear(overlay, !do_overlay ? tigrRGBA(0,0,0,0) : tigrRGBA(0,0,0,OVERLAY_ALPHA));
                if( do_overlay ) {
                    load_overlay(data,len);
                }
            }
        }
        if( zxdb_url(ZXDB, "screen") && ui_click("- Toggle Screen$ -", "Screen\n")) {
            for( char *data = zxdb_download(ZXDB,zxdb_url(ZXDB, "screen"), &len); data; free(data), data = 0 ) {
                //if( len == 6912 ) memcpy(VRAM, data, len);
                if( len == 6912 ) {
                    rgba *tx = thumbnail(data, len, 1, false);

                    do_overlay ^= 1;
                    tigrClear(overlay, !do_overlay ? tigrRGBA(0,0,0,0) : tigrRGBA(0,0,0,OVERLAY_ALPHA));
                    if( do_overlay ) {
                        load_overlay_rgba(tx,256,192);
                        free(tx);
                    }
                }
            }
        }
        if( zxdb_url(ZXDB, "ay") && ui_click("- Toggle Music Tracks -", "Tunes\n")) {
            // use loading screen as a background
            int scrlen; char *scrdata = zxdb_download(ZXDB,zxdb_url(ZXDB, "screen"), &scrlen);

            // load & play tune
            for( char *data = zxdb_download(ZXDB,zxdb_url(ZXDB, "ay"), &len); data; free(data), data = 0 ) {
                loadbin(data, len, false, 0);
            }

            if( scrlen == 6912 ) memcpy(VRAM, scrdata, scrlen);
            free(scrdata);
        }
        static int timer = 0; timer = (timer + 1) % 100;
        if( zxdb_url(ZXDB, "mp3") && ui_click("- Toggle Bonus Track(s) -", va("Bonus%s\n", !play_findvoice('mp3') ? "" : (timer > 50) * 2 + "\f\f\f\f♪"))) {
            cmdkey = 'MP3L'; // mp3 list
        }
        if( zxdb_url(ZXDB, "poke") && ui_click("- Enable Cheats -", "Cheats\n") ) {
            for( char *data = zxdb_download(ZXDB,zxdb_url(ZXDB, "poke"), &len); data; free(data), data = 0 ) {
                loadbin(data, len, false, 0);
            }
        }
        if( zxdb_url(ZXDB, "map") && ui_click("- Toggle Game Map -", "Maps\n")) {
            for( char *data = zxdb_download(ZXDB,zxdb_url(ZXDB, "map"), &len); data; free(data), data = 0 ) {
                do_overlay ^= 1;
                tigrClear(overlay, !do_overlay ? tigrRGBA(0,0,0,0) : tigrRGBA(0,0,0,OVERLAY_ALPHA));
                if( do_overlay ) {
                    load_overlay(data, len);
                }
            }
        }
        if( zxdb_url(ZXDB, "instructions") && ui_click("- Toggle Instructions -", "Help\n")) { // @todo: word wrap maybe. see Afterburner for a good test case
            for( char *data = zxdb_download(ZXDB,zxdb_url(ZXDB, "instructions"), &len); data; free(data), data = 0 ) {

                // is it a zip? unzip & try to recurse... (see: IndianaJonesAndTheLastCrusade)
                // @todo: also .rar, .gz @todo: move this over zxdb_download(). good idea?
                if( len > 4 && !memcmp(data, "PK\3\4", 4) ) {
                    for( FILE *fp = fopen(".Spectral/$$help.zip","wb"); fp; fclose(fp), fp = 0) {
                        fwrite(data, len, 1, fp);
                    }
                    free(data); data = unzip(".Spectral/$$help.zip/*", &len);
                    unlink(".Spectral/$$help.zip");
                    if(!data) continue;
                }

                do_overlay ^= 1;
                tigrClear(ui, !do_overlay ? tigrRGBA(0,0,0,0) : tigrRGBA(0,0,0,OVERLAY_ALPHA));
                if( do_overlay ) {
                    const char *text = as_utf8(replace(data, "\t", " "));

                    int dims = (ui_monospaced = 1, ui_print(NULL, 4,4, ui_colors, text));
                    int w = dims & 0xFFFF;
                    int h = dims >> 16;
                    w = w < _320 ? _320 : w + 16-(w%16);
                    h = h < _240 ? _240 : h + 16-(h%16);

                    tigrFree(overlay);
                    overlay = tigrBitmap(w, h);

                    (ui_monospaced = 1, ui_print(overlay, 4,4, ui_colors, text));

                    tigrRenderInitMap();
                }
            }
        }

        // mags reviews
        // netplay lobby
        }
    }

    // expert mode
    static int rmb_prev = 0, rmb_then = 0, rmb_now = 0;
    rmb_prev = rmb_then;
    rmb_then = rmb_now;
    rmb_now = mouse().rb;
    int rmb_held = rmb_now, rmb_up = rmb_prev;

    int shift = key_pressed( TK_SHIFT);

    // right panel: emulator options
    if( 1 )
    {
        int chr_x = REMAP(smooth,0,1,_320+11,_320-6*11) + 0, chr_y = REMAP(smooth,0,1,-4,2.5) * 11;
        int right = chr_x+8*4-4;
        int bottom = _240-11*(4+0.5*tape_inserted())+chr_y; // +8*31.0-1;

        {
            // draw black panel
            TPixel transp = { 0,0,0, 192 * smooth };
            tigrFillRect(ui, REMAP(smooth,0,1,_320,_320-_60), -1, _320*1/2, _240+2, transp);
            tigrLine(ui, REMAP(smooth,0,1,_320,_320-_60)+1,-1,REMAP(smooth,0,1,_320,_320-_60)+1,_240+2, ((TPixel){255,255,255,240*smooth}));
        }

        // bat anim
        static byte about = 0, frames = 26;
        static const char *abouts[] = {"",""};
        const char *bat = abouts[(about = (about+1)%frames) / (frames/2)];

        ui_at(ui,chr_x - 8,chr_y-11*2-2-1);
        ui_y++;
        if( ui_press("- Frames per second -\n(hold to boost)", "\b%s\f%d", bat, (int)fps) ) cmdkey = 'MAX';
        ui_y--;

        ui_x = chr_x + 3 * 8;
        if( ui_click("- Screenshot -\n(Right-click captures UI)", "%c", SNAP_CHR) ) cmdkey = rmb_up ? 'PIC_':'PIC'; // send screenshot command
        if( ui_click("- VideoREC -\n(Right-click records UI)", "\f\f" )) cmdkey = rmb_up ? 'REC_':'REC';

        ///if( ui_press("- Full Throttle -\n(hold)", "%c\b\b\b%c\b\b\b%c%d\n\n", PLAY_CHR,PLAY_CHR,PLAY_CHR,(int)fps) ) cmdkey = 'MAX';

        ui_at(ui,chr_x - 4/*ui_x - 8*/,ui_y + 11*3);

        const char *models[] = { [1]=" 16 ",[3]=" 48 ",[8]="128K",[9]="P128",[12]=" +2 ",[13]=" +2A",[18]=" +3 " };
        ui_y--;
        if( ui_click("- Clear Medias -", "") ) cmdkey = 'WIPE';
        ui_y++;
        if( ui_click(rmb_held*17+"- Toggle Model -\0- Toggle Model -\n16, 48, 128, +2, +2A, +3, Pentagon\n", "%s%s",models[(ZX/16)|ZX_PENTAGON],ZX_ALTROMS ? "!":"")) if(rmb_up) cmdkey = 'MODE'; else
        {
            int mode = ZX + ZX_PENTAGON;
            ui_dialog_new("- Toggle Model -");
            ui_dialog_option(1,(mode!=16)+"\00516K\n", "Basic model with 16KB RAM", 'MODE',"16");
            ui_dialog_option(1,(mode!=48)+"\00548K\n", "Popular model with 48KB RAM", 'MODE',"48");
            ui_dialog_option(1,(mode!=128)+"\005128K\n", "Upgraded model with 128KB RAM and AY sound chip", 'MODE',"128");
            ui_dialog_option(1,(mode!=200)+"\005+2\n", "128K with built-in tape recorder", 'MODE',"200");
            ui_dialog_option(1,(mode!=210)+"\005+2A\n", "Rebranded +2 model with 64K ROM", 'MODE',"210");
            ui_dialog_option(1,(mode!=300)+"\005+3\n", "+2A compatible with built-in 3\" floppy disk drive", 'MODE',"300");
            ui_dialog_option(1,(mode!=129)+"\005Pentagon\n", "Soviet ZX Spectrum 128 clone with BetaDisk drive", 'MODE',"129");
        }
        ui_y-=1;
        if( ui_click(rmb_held*23+"- Magic button (NMI) -\0- Magic button (NMI) -\nGenerates a Non-Maskable Interrupt", "\f\n") ) cmdkey = 'NMI';
        ui_y+=1;

        int tableHz[] = {[50]=0,[60]=1,[100]=2,[120]=3,[150]=4,[200]=5,[400]=6};
        if( ui_click(rmb_held*21+"- Toggle Z80 speed -\0- Toggle Z80 speed -\n0:25Hz, 1:30Hz, 2:50Hz, 3:60Hz, 4:75Hz, 5:7MHz, 6:14MHz", "🗲\f%d", tableHz[(int)ZX_FPSMUL]) ) if(rmb_up) cmdkey = 'CPU'; else
        {
            ui_dialog_new("- Toggle Z80 speed -");
            ui_dialog_option(1,(ZX_FPSMUL!=400)+"\005400%% (14 MHz)\n",NULL,'CPU',"400");
            ui_dialog_option(1,(ZX_FPSMUL!=200)+"\005200%% (7 MHz)\n",NULL,'CPU',"200");
            ui_dialog_option(1,(ZX_FPSMUL!=150)+"\005150%% (75 Hz)\n",NULL,'CPU',"150");
            ui_dialog_option(1,(ZX_FPSMUL!=120)+"\005120%% (60 Hz)\n",NULL,'CPU',"120");
            ui_dialog_option(1,(ZX_FPSMUL!=100)+"\005100%% (50 Hz)\n",NULL,'CPU',"100");
            ui_dialog_option(1,(ZX_FPSMUL!= 60)+"\00560%% (30 Hz)\n",NULL,'CPU',"60");
            ui_dialog_option(1,(ZX_FPSMUL!= 50)+"\00550%% (25 Hz)\n",NULL,'CPU',"50");
        }
        ui_x += 8;
        if( ui_click(rmb_held*19+"- Toggle ULAplus -\0- Toggle ULAplus -\n0:off, 1:on, 2:on+ultrawide", "%c\f%d\n", ZX_ULAPLUS ? 'U':'u'/*CHIP_CHR '+'*/, ZX_ULAPLUS) ) if(rmb_up) cmdkey = 'ULA'; else
        {
            ui_dialog_new("- Toggle ULAplus -");
            ui_dialog_option(1,(ZX_ULAPLUS!=2)+"\5Ultrawide ULA+\n",NULL,'ULA',"2");
            ui_dialog_option(1,(ZX_ULAPLUS!=1)+"\5Enhanced ULA+\n",NULL,'ULA',"1");
            ui_dialog_option(1,(ZX_ULAPLUS!=0)+"\5Classic ULA\n",NULL,'ULA',"0");
        }

        if( ui_click(rmb_held*16+"- Toggle Zoom -\0- Toggle Zoom -\nx1, x2, x3, x4", "X\f%d",ZX_ZOOM+!ZX_ZOOM) ) if(rmb_up) cmdkey = 'ZOOM'; else
        {
            int w, h; tigrGetDesktop(&w, &h);

            ui_dialog_new("- Toggle Zoom -");
            for( int i = 1; i <= 4; ++i ) {
                if( app_wouldfit(i) )
                    ui_dialog_option(1,va((ZX_ZOOM!=i)+"\005X%d\n",i), NULL,'ZOOM',va("%d",i));
            }
        }
        ui_x += 8;
        if( ui_click(rmb_held*22+"- Toggle Fullscreen -\0- Toggle Fullscreen -\n0:off, 1:on", "\f%d\n", ZX_FULLSCREEN) ) if(rmb_up) cmdkey = 'FULL'; else
        {
            ui_dialog_new("- Toggle Fullscreen -");
            ui_dialog_option(1,(!ZX_FULLSCREEN)+"\5Fullscreen\n",NULL,'FULL',"1");
            ui_dialog_option(1,( ZX_FULLSCREEN)+"\5Windowed\n",NULL,'FULL',"0");
        }

        if( ui_click(rmb_held*19+"- Toggle TV mode -\0- Toggle TV mode -\n0:off, 1:rf, 2:crt, 3:crt+rf", "▒\f%d", (ZX_CRT << 1 | ZX_RF)) ) if(rmb_up) cmdkey = 'TV'; else
        {
            int mode = (ZX_CRT << 1 | ZX_RF);
            ui_dialog_new("- Toggle TV mode -");
            ui_dialog_option(1,(mode!=3)+"\5<CRT and RF (extra CPU cost)\n",NULL,'TV',"3");
            ui_dialog_option(1,(mode!=2)+"\5<CRT only\n",NULL,'TV',"2");
            ui_dialog_option(1,(mode!=1)+"\5<RF only (extra CPU cost)\n",NULL,'TV',"1");
            ui_dialog_option(1,(mode!=0)+"\5<Crisp\n",NULL,'TV',"0");
        }
        ui_x += 8;
        if( ui_click(rmb_held*19+"- Toggle Palette -\0- Toggle Palette -\n0:Spectral, X:others", "\f%d\n", ZX_PALETTE) ) if(rmb_up) cmdkey = 'PAL'; else
        {
            ui_dialog_new("- Toggle Palette -");
            for( int i = 0; i < countof(ZXPaletteNames); ++i)
            ui_dialog_option(1,va((ZX_PALETTE!=i)+"\005%s\n",ZXPaletteNames[i]),NULL,'PAL',va("%d",i));
        }

        if( ui_click(rmb_held*19+"- Toggle AY core -\0- Toggle AY core -\n0:off, 1:fast, 2:accurate", /*𝄞*/"♬\f%d",ZX_AY) ) if(rmb_up) cmdkey = 'AY'; else
        {
            ui_dialog_new("- Toggle AY core -");
            ui_dialog_option(1,(ZX_AY!=2)+"\5Accurate AY\n",NULL,'AY',"2");
            ui_dialog_option(1,(ZX_AY!=1)+"\5Fast AY\n",NULL,'AY',"1");
            ui_dialog_option(1,(ZX_AY!=0)+"\5Off\n",NULL,'AY',"0");
        }
        ui_x += 8;
        if( ui_click(rmb_held*21+"- Toggle Waveforms -\0- Toggle Waveforms -\n0:off, 1:on", "\f%d\n",ZX_WAVES) ) if(rmb_up) cmdkey = 'WAVE'; else
        {
            ui_dialog_new("- Toggle Waveforms -");
            ui_dialog_option(1,(ZX_WAVES!=1)+"\5Visualize waveforms\n",NULL,'WAVE',"1");
            ui_dialog_option(1,(ZX_WAVES!=0)+"\5Off\n",NULL,'WAVE',"0");
        }

        const char joyicon[256] = {
            //[64]='X',   // Kempston C [port 95]
            [32]='B',     // Kempston B [port 55]
            [16]='F',     // (F)uller
            [8]='2',      // Sinclair/Interface (2)
            [4]='1',      // Sinclair (1)
            [2]='K',      // (K)empston
            [1]='C',      // (C)ursor
            [0]='0',      // (0)Off
            [16+2+1]='J', // (J)oysticks mask: cursor+kempston+fuller
        };
        if( ui_click(rmb_held*21+"- Toggle Joysticks -\0- Toggle Joysticks -\n0:off, 1:sinclair1, 2:sinclair/interface2\nC:ursor, K:empston, J:fuller+cursor+kempston", "%c\f%c", JOYSTICK_CHR, ZX_JOYSTICK > 0xFF ? (ZX_JOYSTICK>>8) + 'a' : joyicon[ZX_JOYSTICK&0xFF] ? joyicon[ZX_JOYSTICK&0xFF] : '*')) if(rmb_up) cmdkey = 'JOY'; else
        {
            cmdkey = 'JOY0';
        }
        ui_x += 8;
        if( ui_click("- Gamepad bindings -", "\f0\n") ) // if(rmb_up) cmdkey = 'PAD0'; else
        {
            cmdkey = 'PAD0';
        }

        if( ui_click(rmb_held*20+"- Toggle Lightgun -\0- Toggle Lightgun -\n0:off, 1:lightgun+gunstick", /**/"\xB\f%d", ZX_GUNSTICK) ) if(rmb_up) cmdkey = 'GUNS'; else
        {
            ui_dialog_new("- Toggle Lightgun -");
            ui_dialog_option(1,(!ZX_GUNSTICK)+"\5Lightgun + Gunstick\n",NULL,'GUNS',"1");
            ui_dialog_option(1,( ZX_GUNSTICK)+"\5No lightgun\n",NULL,'GUNS',"0");
        }
        ui_x += 8;
        if( ui_click(rmb_held*20+"- Toggle Autofire -\0- Toggle Autofire -\n0:off, 1:slow, 2:fast, 3:faster", "\f%d\n", ZX_AUTOFIRE) ) if(rmb_up) cmdkey = 'FIRE'; else
        {
            ui_dialog_new("- Toggle Autofire -");
            ui_dialog_option(1,(ZX_AUTOFIRE!=3)+"\5Faster autofire\n",NULL,'FIRE',"3");
            ui_dialog_option(1,(ZX_AUTOFIRE!=2)+"\5Fast autofire\n",NULL,'FIRE',"2");
            ui_dialog_option(1,(ZX_AUTOFIRE!=1)+"\5Slow autofire\n",NULL,'FIRE',"1");
            ui_dialog_option(1,(ZX_AUTOFIRE!=0)+"\5No autofire\n",NULL,'FIRE',"0");
        }

        if( ui_click(rmb_held*17+"- Toggle Mouse -\0- Toggle Mouse -\n0:off, 1:kempston", "\x9\f%d", ZX_MOUSE) ) if(rmb_up) cmdkey = 'MICE'; else
        {
            ui_dialog_new("- Toggle Mouse -");
            ui_dialog_option(1,(!ZX_MOUSE)+"\5Kempston Mouse\n",NULL,'MICE',"1");
            ui_dialog_option(1,( ZX_MOUSE)+"\5No mouse\n",NULL,'MICE',"0");
        }
        ui_x += 8;
        if( ui_click(rmb_held*21+"- Toggle Run-Ahead -\0- Toggle Run-Ahead -\n0:off, 1:improved input latency", !ZX_RUNAHEAD ? "🯆\f0\n" : "🯇\f1\n") ) if(rmb_up) cmdkey = 'RUN'; else
        {
            ui_dialog_new("- Toggle Run-Ahead -");
            ui_dialog_option(1,(!ZX_RUNAHEAD)+"\5Improved input latency (extra CPU cost)\n",NULL,'RUN',"1");
            ui_dialog_option(1,( ZX_RUNAHEAD)+"\5Normal input latency\n",NULL,'RUN',"0");
        }

        if( ui_click(rmb_held*31+"- Toggle 48-BASIC input mode -\0- Toggle 48-BASIC input mode -\nK:token based, L:letter based", "~%c~\f%d", ZX_KLMODE ? 'L' : 'K', ZX_KLMODE) ) if(rmb_up) cmdkey = 'KL'; else
        {
            ui_dialog_new("- Toggle 48-BASIC input mode -");
            ui_dialog_option(1,( ZX_KLMODE)+"\5Tokens\n","Use traditional input mode",'KL',"0");
            ui_dialog_option(1,(!ZX_KLMODE)+"\5Letters\n","Use modern input mode",'KL',"1");
        }
        ui_x += 8;
        if( ui_click(rmb_held*30+"- Toggle Keyboard Issue 2/3 -\0- Toggle Keyboard Issue 2/3 -\n2:earlier, 3:classic keyboard", "k\f%d\n", issue2 ? 2 : 3)) if(rmb_up) cmdkey = 'KEYB'; else
        {
            ui_dialog_new("- Toggle Keyboard Issue 2/3");
            ui_dialog_option(1,(!!issue2)+"\5Classic keyboard\n",NULL,'KEYB',"3");
            ui_dialog_option(1,( !issue2)+"\5Earlier keyboard\n",NULL,'KEYB',"2");
        }

        if( ui_click(rmb_held*20+"- Toggle FastLoad -\0- Toggle FastMedia -\n0:off, 1:faster media loading", "\f%d", ZX_FASTTAPE )) if(rmb_up) cmdkey = 'FAST'; else
        {
            ui_dialog_new("- Toggle FastLoad -");
            ui_dialog_option(1,(!ZX_FASTTAPE)+"\5Faster loading speed\n",NULL,'FAST',"1");
            ui_dialog_option(1,( ZX_FASTTAPE)+"\5Normal loading speed\n",NULL,'FAST',"0");
        }
        ui_x += 8;
        if( ui_click(rmb_held*20+"- Toggle TurboROM -\0- Toggle TurboROM -\n0:off, 1:TurboROM .tap loader", va("\f%d\n",ZX_TURBOROM))) if(rmb_up) cmdkey = 'TURB'; else
        {
            ui_dialog_new("- Toggle TurboROM -");
            ui_dialog_option(1,(!ZX_TURBOROM)+"\5Turbo ROM loader\n",NULL,'TURB',"1");
            ui_dialog_option(1,( ZX_TURBOROM)+"\5Compatible ROM loader\n",NULL,'TURB',"0");
        }

        if( ui_click(rmb_held*24+"- Translate game menu -\0- Translate game menu -\n0:off, 1:poke game menu into English", "T\f%d", ZX_AUTOLOCALE)) if(rmb_up) cmdkey = 'TENG'; else
        {
            ui_dialog_new("- Translate game menu -");
            ui_dialog_option(1,1/*(!ZX_AUTOLOCALE)*/+"\5Poke translation\n","Poke game menu into English",'TENG',"1");
            ui_dialog_option(1,1/*( ZX_AUTOLOCALE)*/+"\5Cancel\n",NULL,'TENG',"0");
        }
        ui_x += 8;
        if( ui_click("- Toggle Lenslok -\nFocus on the center of the encoded image", "𜲌\f%d\n", ZX_LENSLOK)) cmdkey = 'LENS';

#if DEV
        if( ui_click("- Toggle DevTools -", "d\f%d\n", ZX_DEVTOOLS)) cmdkey = 'DEVT';
#endif

#if 0
        static int use_console = 0;
//        if( DEV )
        if( ui_click("- Toggle Console -", "\f%d\n", use_console)) { // cmdkey = 'TERM';
            void logo();
            use_console ^= 1;
            if( use_console ) AllocConsole(), /*enable_ansi(),*/ logo();
            else FreeConsole();
        }
#endif

        //if( ui_click("- Toggle TapePolarity -", "%c\f%d\n", mic_low ? '+':'-', !mic_low) ) cmdkey = 'POLR';

        ui_at(ui,chr_x - 8,bottom+1);
        if( ui_click(NULL, "i") ) cmdkey = 'HELP';

        // debug
        ui_at(ui,right,bottom);
        if( ui_click("- Debug -", "") ) cmdkey = 'DEV'; // send disassemble command
    }

    if( record_active() && ZXFlashFlag ) {
        ui_print(ui, _320-8*1-3, 0*11+4-2+2+1-2-1, ui_colors, "\2\7" );
    }

#if 1
    if( active ) {
        ui_at(ui, 1*11-4+2-8+2, 0*11+4-2);
        if( ui_click(active ? "Resume" : "Pause", active ? PLAY_STR : PAUSE_STR) ) cmdkey = 'GAME'; // active ^= 1, ui_dialog_new(NULL);
    }
#endif

    // bottom slider: tape browser. @todo: rewrite this into a RZX player/recorder too
    #define MOUSE_HOVERED_X() (m.x >= TOFF && m.x < (TOFF+_320))
    #define MOUSE_HOVERED_Y() (m.y >= (_240-11-11*(m.x<_320*5/6)) && m.y < (_240+11) ) // m.y > 0 && m.y < 11 
    #define MOUSE_ACTION(pct) tape_seekf(pct)
    #define BAR_Y()           (_240-REMAP(smoothY,0,1,-7,7)) // REMAP(smoothY,0,1,-10,UI_LINE1)
    #define BAR_PROGRESS()    tape_tellf()
    #define BAR_VISIBLE()     ( !tape_inserted() ? 0 : (MOUSE_HOVERED_Y() || BAR_PROGRESS() <= 1. && mic_on/*tape_playing()*/) ) // (m.y > -10 && m.y < _240/10) )
    #define BAR_FILLING(...)  for(int pct = (int)((x)*1000.0/(T320+!T320)), *e = &pct; e; e = 0) if( (x&1) && tape_preview[pct] ) { __VA_ARGS__; }

    // bottom slider: tape browser
    const int TOFF = _320/36, T320 = _320 - TOFF;
    int visible = !active && !do_overlay && BAR_VISIBLE();
    static float smoothY; do_once smoothY = visible;
    smoothY = smoothY * 0.75 + visible * 0.25;
    if( smoothY > 0.01 )
    {
        int y = BAR_Y();

        TPixel white = {255,255,255,255}, black = {0,0,0,255}, *bar = &ui->pix[TOFF + y * _320];

        if( ZX_CRT && (y > _240/2) ) // scanline correction to circumvent CRT edge distortion
            bar -= _320 * 2;

        // manual tape controls
        if( (ui_at(ui,TOFF/2-8/2,y-2), ui_click(NULL, tape_playing() ? "■" : PLAY_STR)) )
            cmdkey = tape_playing() ? 'STOP' : 'PLAY';
            //if( ui_click(NULL, "\xf\b\b\b\xf") ) cmdkey = 'PREV';
            //if( ui_click(NULL, "%c\b\b\b%c", PLAY_CHR, PLAY_CHR) ) cmdkey = 'NEXT';
            //if( ui_click(NULL, "%d", autoplay)) ZX_AUTOPLAY ^= 1;
            //if( ui_click(NULL, "%d", autostop)) ZX_AUTOSTOP ^= 1;

        unsigned mark = BAR_PROGRESS() * T320;
        if( y < (_240/2) ) {
            // bars & progress (top)
            if(y>= 0) for( int x = 0; x < T320; ++x ) bar[x] = white;
            if(y>=-2) for( int x = 0; x < T320; ++x ) bar[x+2*_320] = white;
            if(y>= 1) for( int x = 0; x<=mark; ++x )  bar[x+_320] = white;
            if(y>=-2) for( int x = 1; x<=mark; ++x )  bar[-1+2*_320] = black;
            if(y>=-1) for( int x = 0; x < T320; ++x ) BAR_FILLING(bar[x+1*_320] = white);
            // triangle marker (top)
            if(y>=-4) bar[mark+4*_320] = white;
            if(y>=-5) for(int i = -1; i <= +1; ++i) if((mark+i)>=0 && (mark+i)<_320) bar[mark+i+5*_320] = white;
            if(y>=-6) for(int i = -2; i <= +2; ++i) if((mark+i)>=0 && (mark+i)<_320) bar[mark+i+6*_320] = white;
        } else {
            // triangle marker (bottom)
            if(y<=_239-0) for(int i = -2; i <= +2; ++i) if((mark+i)>=0 && (mark+i)<_320) bar[mark+i+0*_320] = white;
            if(y<=_239-1) for(int i = -1; i <= +1; ++i) if((mark+i)>=0 && (mark+i)<_320) bar[mark+i+1*_320] = white;
            if(y<=_239-2) bar[mark+2*_320] = white;
            // bars & progress (bottom)
            //if(y<=_239-4) for( int x = 1; x<=mark; ++x )  bar[-1+4*_320] = black;
            if(y<=_239-4) for( int x = 0; x < T320; ++x ) bar[x+4*_320] = white;
            if(y<=_239-5) for( int x = 0; x < T320; ++x ) BAR_FILLING(bar[x+5*_320] = white);
            if(y<=_239-5) for( int x = 0; x<=mark; ++x )  bar[x+5*_320] = white;
            if(y<=_239-6) for( int x = 0; x < T320; ++x ) bar[x+6*_320] = white;
        }

        // is mouse hovering
        if( MOUSE_HOVERED_Y() && MOUSE_HOVERED_X() ) {
            mouse_cursor(2);
            if( m.buttons ) {
                // MOUSE_ACTION((CLAMP(m.x,TOFF,TOFF+T320)-TOFF) / ((float)T320));
                MOUSE_ACTION((m.x-TOFF) / ((float)T320));
                // cmdkey = 'STOP';
            }
            static int prev = 0;
            if( prev && !m.buttons ) {
                cmdkey = 'PLAY';
            }
            prev = m.buttons;
        }
    }

    // bottom slider. @todo: rewrite this into a RZX player/recorder
    if( 0 )
    if( ZX_DEBUG )
    if( !active && !do_overlay ) {
        static float my_var = 0; // [-2,2]

        TPixel white = {255,255,255,255}, black = {0,0,0,255}, *bar = &ui->pix[0 + (_240-7) * _320];
        unsigned mark = REMAP(my_var, -2,2, 0,1) * _320;
        // triangle marker (bottom)
        for(int i = -2; i <= +2; ++i) if((mark+i)>=0 && (mark+i)<_320) bar[mark+i+0*_320] = white;
        for(int i = -1; i <= +1; ++i) if((mark+i)>=0 && (mark+i)<_320) bar[mark+i+1*_320] = white;
        bar[mark+2*_320] = white;
        bar += _320 * 4;
        // bars & progress
        for( int x = 0; x < _320; ++x ) bar[x] = bar[x+2*_320] = white;
        for( int x = 0; x<=mark; ++x ) bar[x+_320] = white; bar[_320-1+_320] = white;
        // mouse seeking
        if( m.y >= (_240-11) && m.y < _240 ) {
            mouse_cursor(2);
            if( m.buttons/*&4*/ ) {
                m.x = m.x < 0 ? 0 : m.x > _320 ? _320 : m.x;
                float target = REMAP(m.x, 0,_320, 0.98,1.2);
                my_var = my_var * 0.50f + target * 0.50f ; // animate seeking
                // print my_var value
                char text[32]; sprintf(text, "%.4f", my_var);
                ui_print(ui, (mark+5)/11.f,(_240-12.0)/11, ui_colors, text);
            }
        }
    }
}


char* game_browser(int version) { // returns true if loaded
    // scan files
    if( !numgames && !zxdb_loaded() ) {
        do_once {
            uint64_t then = time_ns();
            const char *folder = "./games/";
            #if TESTS
            folder = "./src/tests/";
            #endif
            rescan(folder);
            printf("%5.2fs rescan\n", (time_ns() - then)/1e9);
        }
    }

    if( !numgames && !zxdb_loaded() ) return 0;

    if( !active ) return 0;

    // game browser
    if( active ) {
        // disable overlay
        if( do_overlay ) tigrClear(overlay, tigrRGBA(0,0,0,0));
        do_overlay = 0;

        // restore mouse interaction in case it is being clipped (see: kempston mouse)
        mouse_clip(0);
        mouse_cursor(1);
    }

    char *entry = version == 2 ? game_browser_v2() : game_browser_v1();
    if( entry || cmdkey == 'ZXDB' ) {
        //mouse_cursor(1); // @fixme: restore mouse cursor after clicking an item list
        ui_dialog_new(NULL); // cancel any internal dialog that remains active
    }
    if( entry ) {
        return strendi(entry, "/") ? rescan(entry), NULL : entry;
    }

    return NULL;
}

void logo(void) {
    cputs("\3 \3 \3 \3 \3 \3 \3 \3 \2 \2 \2 \2 \2 \2 \2 \2 \2 \2 \2 \2 \2 \2 \2 \2 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \1 \1 \1 \1 \1█");
    cputs("\3█\3▀\3▀\3▀\3▀\3▀\2▀\2▀\2▀\2▀\2 \2█\2▀\2▀\2▀\2▀\2▀\2▀\2▀\2▀\2█\2 \6█\6▀\6▀\6▀\6▀\6▀\6▀\6▀\6▀\6▀\6 \6█\6▀\4▀\4▀\4▀\4▀\4▀\4▀\4▀\4▀\4 \4▀\4▀\4▀\4▀\4█\4▀\4▀\4▀\4▀\5▀\5 \5█\5▀\5▀\5▀\5▀\5▀\5▀\5▀\5▀\5▀\5 \5▀\5▀\5▀\5▀\5▀\1▀\1▀\1▀\1▀\1█\1 \1█");
    cputs("\3▀\3▀\3▀\3▀\2▀\2▀\2▀\2▀\2▀\2█\2 \2█\2 \2 \2 \2 \2 \2 \2 \2 \6█\6 \6█\6▀\6▀\6▀\6▀\6▀\6▀\6▀\6▀\6▀\6 \4█\4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4█\4 \4 \5 \5 \5 \5 \5█\5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5█\5▀\5▀\1▀\1▀\1▀\1▀\1▀\1▀\1█\1 \1█");
    cputs("\3▀\3▀\2▀\2▀\2▀\2▀\2▀\2▀\2▀\2▀\2 \2█\2▀\2▀\2▀\2▀\2▀\2▀\6▀\6▀\6▀\6 \6▀\6▀\6▀\6▀\6▀\6▀\6▀\6▀\6▀\4▀\4 \4▀\4▀\4▀\4▀\4▀\4▀\4▀\4▀\4▀\4▀\4 \4 \4 \4 \4 \5▀\5 \5 \5 \5 \5 \5 \5▀\5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5▀\1▀\1▀\1▀\1▀\1▀\1▀\1▀\1▀\1▀\1 \1▀" "\007 " SPECTRAL);
    cputs("\3 \2 \2 \2 \2 \2 \2 \2 \2 \2 \2 \2█\2 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \6 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \5 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1 \1" ANSI_RESET);

    // works on win, lin, osx. wont work on lubuntu/lxterminal, though.
    //cputs("\3┏\2┓\2 \6 \6 \6 \4 \4 \4 \5 \5 \5 \1┓");
    //cputs("\2┗\2┓\6┏\6┓\6┏\4┓\4┏\4╋\5┏\5┓\5┏\1┓\1┃");
    //cputs("\2┗\6┛\6┣\6┛\4┗\4 \4┗\5┗\5┛\5 \1┗\1┻\1┗" "\007" SPECTRAL);
    //cputs("\6 \6 \4┛\4 \4 \4 \5 \5 \5 \1 \1 \1 \1 " ANSI_RESET);

    //puts("┌┐          ┐");
    //puts("└┐┌┐┌┐┌┼┌┐┌┐│");
    //puts("└┘├┘└ └└┘ └┴└");
    //puts("  ┘          ");
    //puts("┏┐          ┐");
    //puts("└┐┏┐┏┐┏┼┏┐┏┐┃");
    //puts("└┛├┛└ └└┛ └┻└");
    //puts("  ┛          ");
    //puts("┏┓          ┓");
    //puts("┗┓┏┓┏┓┏╋┏┓┏┓┃");
    //puts("┗┛┣┛┗ ┗┗┛ ┗┻┗");
    //puts("  ┛          ");
}

#if defined _WIN32 && defined NDEBUG && NDEBUG > 0
int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    if(1) {
        //NOTE: these calls are the closest i'm aware you can get to /SUBSYSTEM:CONSOLE
        //      in a gui program while cleanly handling existing consoles (cmd.exe),
        //      pipes (ninja) and no console (VS/RemedyBG; double-clicking the game).
        //      the other option is to compile with /SUBSYSTEM:CONSOLE and call FreeConsole()
        //      if no console is needed but it is amateur to flash a console for a second
        if (!AttachConsole(ATTACH_PARENT_PROCESS) && GetLastError() != ERROR_ACCESS_DENIED) assert(AllocConsole());
        printf("\n"); //print >= 1 byte to distinguish empty stdout from a redirected stdout (fgetpos() position <= 0)
        fpos_t pos = {0};
        if (fgetpos(stdout, &pos) != 0 || pos <= 0) {
            assert(freopen("CONIN$" , "r", stdin ));
            assert(freopen("CONOUT$", "w", stderr));
            assert(freopen("CONOUT$", "w", stdout));
        }
    }
#else
int main() {
#endif

#ifdef _WIN32 // 3rd_tfd.h requires this
    HRESULT lHResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
#endif

#if !DEV && defined(_WIN32)
    // do not print logo on win32 (release). we dont want to flash a console window
#else
    // logo can be printed on linux/macos builds (dev+release) and also on win32 (dev) builds.
    logo();
#endif

    // install icon hooks for any upcoming window or modal creation
    window_override_icons();

    // convert relative paths
    for( int i = 1; i < __argc; ++i ) {
        if( __argv[i][0] != '-' ) {
            char full[MAX_PATH] = {0};
            realpath(__argv[i], full);
            __argv[i] = strdup(full); // @leak
        }
    }

    // initialize tests
    printer = stdout;
#if TESTS
    {
        if( __argc <= 1 ) die("error: no test file provided");
        printer = fopen(va("%s.txt", __argv[1]), "wt"); //"a+t");
        if(!printer) die("cant open file for logging");

        ZX_FASTCPU = 1;
    }
#endif

    // relocate to exedir
    cwdexe();
    // normalize argv[0] extension
#ifdef _WIN32
    __argv[0] = strdup(va("%s%s", __argv[0], strendi(__argv[0], ".exe") ? "" : ".exe" ));
#endif
    // normalize argv[0] path
    if( strrchr(__argv[0], '/') ) __argv[0] = strdup(va("%s", strrchr(__argv[0], '/')+1));
    if( strrchr(__argv[0],'\\') ) __argv[0] = strdup(va("%s", strrchr(__argv[0],'\\')+1));

    // init external zxdb (high priority)
    zxdb_init("Spectral.db");

    // init embedded zxdb (medium priority)
    // also change app behavior to ZX_PLAYER if embedded games are detected.
    {
        int embedlen; char *embed;
        for( unsigned id = 0; (embed = embedded(id, &embedlen)); ++id ) {
            if(!memcmp(embed + 0000, "\x1f\x8b\x08",3))
            if(!memcmp(embed + 0x0A, "Spectral.db",11)) {
                //tinyARC4(embed + 72, embed + 72, embedlen - 72, "FuckM$Defender");
                zxdb_initmem(embed, embedlen);
                continue;
            }
            if(!memcmp(embed + 0000, "Rar!",4))
            if(!memcmp(embed + 0x38, "Spectral.db",11)) {
                //tinyARC4(embed + 72, embed + 72, embedlen - 72, "FuckM$Defender");
                zxdb_initmem(embed, embedlen);
                continue;
            }
            if( embedlen ) ZX_PLAYER |= 1;
        }
    }

    // init embedded zxdb as resource (low priority)
    #if _WIN32
    {
        HMODULE hModule = NULL;
        HRSRC resourceInfo = FindResourceA(hModule, "ZXDBData", "CUSTOMDATA");
        if (resourceInfo) {
            HGLOBAL resource = LoadResource(hModule, resourceInfo);
            if (resource) {
                DWORD resourceSize = SizeofResource(hModule, resourceInfo);
                zxdb_initmem(resource, resourceSize);
            }
        }
    }
    #endif

    // import config
    if(!ZX_PLAYER)
    load_config();

    // fixed settings on zxplayer builds
    if( ZX_PLAYER ) {
        ZX_RF = 1;
        ZX_CRT = 1;
    }

    // prepare title based on argv[0]
    char *apptitle = va("%s", __argv[0]);
    while( strrchr(apptitle,'\\') ) apptitle = strrchr(apptitle,'\\') + 1;
    while( strrchr(apptitle, '/') ) apptitle = strrchr(apptitle, '/') + 1;
    if( strrchr(apptitle, '.') ) *strrchr(apptitle, '.') = '\0';
    apptitle = va("%s%s", apptitle, DEV ? " DEV" : "");

    // main app and bitmap layers
    if( !app_create(apptitle,ZX_FULLSCREEN,ZX_ZOOM) ) die("cannot create app window");
    canvas = tigrBitmap(_256, _192);

    // disable win key
    // disable nag window after holding shift for 8s (windows)
    // ensure this is called after win32 handles are created
    enable_os_keys(0);

    // must be as close to frame() as possible
    audio_init();

    // zx
    boot(128, 0);

    // load embedded games (if any)
    {
        int embedlen; const char *embed;
        for( unsigned id = 0; (embed = embedded(id, &embedlen)); ++id ) {
            // skip spectral.db.gz
            if(!memcmp(embed + 0000, "\x1f\x8b\x08",3))
            if(!memcmp(embed + 0x0A, "Spectral.db",11))
                continue;
            // skip spectral.db.rar
            if(!memcmp(embed + 0000, "Rar!",4))
            if(!memcmp(embed + 0x38, "Spectral.db",11))
                continue;
            loadbin(embed, embedlen, 1, 0);
        }
    }

    // zxplayer does not load/save state
    if(!ZX_PLAYER)

    {
        int seekpos = 0;

        // restore media: almost same than reload(ZX);
        titlebar(ZX_TITLE);
        if( ZX_MEDIA ) {
            char *at = strrchr(ZX_MEDIA, '@'); if(at) *at = 0, seekpos = atoi(at+1);
            if( !loadfile(ZX_MEDIA, 1, 0)) if( !zxdb_load(ZX_MEDIA, 0) ) {} // @fixme: emit warning
        }

        // import state; do not clear medias we just loaded 3 lines ago
        for( FILE *state = fopen("Spectral.sav","rb"); state; fclose(state), state = 0) {
            import_state(state, 0|KEEP_MEDIA);
        }

        tape_seeki(seekpos);
    }

    // main loop
    do {

    #if 0 && NEWCORE
    do {
    #endif

        // flush
        if( tigrClosed(app) )
            break;
        tigrUpdate(app);

#if 1
        // 4 parameters in our shader. we use parameters[0] to track time
        if( ZX_CRT )
        tigrSetPostFX(app, (ticks / (69888 * 50.)), -mouse_offsets()[0],-mouse_offsets()[1],0);
        else
        tigrSetPostFX(app, 0, 0,0,1); // app->w,app->h,1);

        // update background color so it matches. this is especially visible during fullscreen
        glColor[0] = glColor[1] = glColor[2] = 0, glColor[3] = 1;
#if 0
        if( !ZX_CRT )
        glColor[0] = ((ZXPalette[ZXBorderColor] >> 0) & 255) / 255.0,
        glColor[1] = ((ZXPalette[ZXBorderColor] >> 8) & 255) / 255.0,
        glColor[2] = ((ZXPalette[ZXBorderColor] >>16) & 255) / 255.0;
#endif

#endif

        ui_frame_begin();
        input();
        if(do_overlay) ZXKeyboardClear(); // do not submit keys to ZX while overlay is drawn on top

        // handle ESC/RMB (cancel buttons)
        int escape = key_trigger( TK_ESCAPE);
        int cancel = escape || mouse().rb;
        if( cancel ) {
            // cancel dialog
            if( num_options && mouse().cursor != 2 ) ui_dialog_new(NULL);
            // cancel overlay
            else if( do_overlay ) do_overlay = 0, tigrClear(overlay, tigrRGBA(0,0,0,0));
            // else toggle browser (ESC key only)
            else if( !mouse().rb ) cmdkey = 'GAME';
        }

        // detect disk activity: fdc.timeout can be 0 or 512 while idling. wd.Wait is 255 for every cmd, then gets decremented every tick.
        // not using fdc.motor because some games leave that set while playing (cybernoid2,smaily,rickdangerous2)
        int fdc_inuse = (ZX_PENTAGON || ZX == 300) && ((fdc.timeout & 0x1FF) || (wd.Wait == 255) || (PC(cpu) < 0x4000 && GET_MAPPED_ROMBANK() == GET_3DOS_ROMBANK()) || play_findvoice('read') || play_findvoice('seek'));
        static int disk_hz = 0; disk_hz = fdc_inuse; // disk_hz += 50 * fdc_inuse; disk_hz = CLAMP(disk_hz-1, 0, 50);

        int disk_likely_loading = disk_hz > 0;
        int tape_likely_loading = (PC(cpu) & 0xFF00) == 0x0500 ? 1 : tape_hz > 40;

        int media_accelerated = ZX_FASTCPU ? 1
            : tape_inserted() && tape_peek() == 'o' ? 0 
            : ZX_FASTTAPE && tape_likely_loading && tape_playing() ? 1
            : ZX_FASTTAPE && disk_likely_loading ? 1
            : 0;

        if( active ) media_accelerated = 0;

        // z80, ula, audio, etc
        // static int frame = 0; ++frame;
        int do_sim = active ? 0 : 1;
        int do_drawmode = 1; // no render (<0), full frame (0), scanlines (1)
        int do_flashbit = media_accelerated ? 0 : 1;
        int do_runahead = media_accelerated ? 0 : ZX_RUNAHEAD;

#if TESTS
        // be fast. 50% frames not drawn. the other 50% are drawn in the fastest mode
        static byte even = 0; ++even;
        do_drawmode = even & 1; 

        // monitor test for completion
        static byte check_tests = 0;
        if( !check_tests++ )
        {
            static unsigned prev = 0;
            static unsigned stalled = 0;

            struct stat st;
            if( fstat(fileno(printer), &st) == 0 ) {
                if( prev == st.st_size ) ++stalled;
                else prev = st.st_size, stalled = 0;
            }

            // finish test after being idle for 15,000,000 frames
            if( stalled >= (50*300000/256) ) {
                fprintf(printer, "Quitting test because of inactivity.\n");
                exit(0);
            }
        }
#endif

        if( ZX_TURBOROM )
        rom_patch_turbo();
        rom_patch_klmode();

        static byte counter = 0; // flip flash every 16 frames @ 50hz
        if( !((++counter) & 15) ) if(do_flashbit) ZXFlashFlag ^= 1;

if( do_runahead == 0 ) {
        do_audio = 1;
        frame(do_drawmode, do_sim); //media_accelerated ? (frame%50?0:1) : 1 );
} else {
        // runahead:
        // - https://near.sh/articles/input/run-ahead https://www.youtube.com/watch?v=_qys9sdzJKI // https://docs.libretro.com/guides/runahead/

        do_audio = 1;
        frame(-1, do_sim);

        quicksave(10);

        do_audio = 0;
        frame(do_drawmode, do_sim);

        quickload(10);
}

        // @todo: optimize me
        tigrClear(app, tigrRGB(0,0,0));
        tigrClear(ui, !active && !do_overlay ? tigrRGBA(0,0,0,0) : tigrRGBA(0,0,0,128));

        // blit zx layer (canvas) to main app
        {
            // center
            int x = canvas->w < app->w ? (app->w - canvas->w) / 2 : -(canvas->w - app->w) / 2;
            int y = canvas->h < app->h ? (app->h - canvas->h) / 2 : -(canvas->h - app->h) / 2;
            // detect resize change
            static int prevx = 0, prevy = 0;
            if( x != prevx || y != prevy )
                app_resize();
            prevx = x, prevy = y;
            // blit
            tigrBlit(app, canvas, x,y, 0,0,_256,_192);
            // adjust mouse coords for next frame
            mouse_offsets()[0] = -x;
            mouse_offsets()[1] = -y;
            // ultrawide ula: for every paper scanline, repeat leftmost/rightmost pixels to infinity
            if( ZX_ULAPLUS >= 2 ) {
                if( x > 0 )
                for( int sc = 0; sc < (0+_192); ++sc)
                    if( (sc+y) >= 0 && (sc+y) < _240 )
                    memset32(&app->pix[0+(y+sc)*_320], *(rgba*)&canvas->pix[0+sc*_256], x),
                    memset32(&app->pix[_319-x+(y+sc)*_320], *(rgba*)&canvas->pix[_256-1+sc*_256], x);
                // truncate last line since it is unlikely to be a full width line in any zx model
                if( y > 0 )
                for( int sc = _192-1; sc < (0+_192); ++sc)
                    if( (sc+y) >= 0 && (sc+y) < _240 )
                    memset32(&app->pix[0+(y+sc)*_320], *(rgba*)&canvas->pix[0+sc*_256], _320);
            }
        }

        char* game = game_browser(ZX_BROWSER);
        if( game ) {
            active = 0;

            bool insert_next_disk_or_tape = false;
            if( ZX_TITLE ) { // @fixme: ZXDB might use ZX_MEDIA and search for sequential #X diffs instead
                if( 0 != strcmp(game, ZX_TITLE) ) {
                    const char *a1 = game, *a2 = ZX_TITLE;

                    // basenames and their lengths
                    const char *b1 = strrchr(a1, '/') ? strrchr(a1, '/')+1 : a1; int l1 = strlen(b1);
                    const char *b2 = strrchr(a2, '/') ? strrchr(a2, '/')+1 : a2; int l2 = strlen(b2);
                    // printf("%s(%d) %s(%d)\n", b1,l1, b2,l2);

                    // multi-load tapes and disks are well named (eg, Mutants - Side 1.tzx). 
                    // following oneliner hack prevents some small filenames to be catched in the 
                    // diff trap below. eg, 1942.tzx / 1943.tzx; they do not belong to each other
                    // albeit their ascii diff is exactly `1`.
                    if( l1 > 8 )

                    if( l1 == l2 ) {
                        int diff = 0;
                        for( int i = 0; i < l1; ++i ) {
                            diff += b1[i] - b2[i];
                        }
                        insert_next_disk_or_tape = diff == 1;
                    }
                }
            }

            int model = key_pressed( TK_SHIFT) ? 128 : 48;
            /**/ if( strstr (game+1, "128") )       model = 128;
            else if( strstri(game+1, "pentagon") )  model = 129;
            else if( strstri(game+1, ".dsk") )      model = 300;
            else if( strstri(game+1, ".$b") )       model = 129;
            else if( strstri(game+1, ".$c") )       model = 129;
            else if( strstri(game+1, ".scl") )      model = 129;
            else if( strstri(game+1, ".img") )      model = 129;
            else if( strstri(game+1, ".mgt") )      model = 129;
            else if( strstri(game+1, ".fdi") )      model = 129;
            else if( strstri(game+1, ".trd") )      model = 129;
            else if( strstr (game+1, "ZX7") )       model = 128;

            int must_clear = insert_next_disk_or_tape || strstri(game, ".pok") ? 0 : 1;
            int must_turbo = key_pressed(TK_CONTROL) || ZX_TURBOROM ? 1 : 0;
            int use_preloader = must_clear ? 1 : 0;

            if( must_clear ) boot(model, 0);
            if( must_turbo ) rom_patch_turbo();

            if( loadfile(game,use_preloader,model) ) {
                titlebar(game);

                (free(ZX_MEDIA), ZX_MEDIA = strdup(game));

                // clear window keys so the current key presses are not being sent to the 
                // next emulation frame. @fixme: use ZXKeyboardClear(); instead
                memset(tigrInternal(app)->keys, 0, sizeof(tigrInternal(app)->keys));
                memset(tigrInternal(app)->prev, 0, sizeof(tigrInternal(app)->prev));
            }
        }

        // measure time & frame lock (50.01 fps)
        int max_speed = media_accelerated || !ZX_FPSMUL || ZX_FASTCPU; // max speed if media_accelerated or no fps lock
        if( max_speed ) {
            dt = tigrTime();
            // constant time flashing when loading accelerated tapes (every 16 frames @ 50hz)
            static float accum = 0; accum += dt;
            if( accum >= 0.32f ) accum = 0, ZXFlashFlag ^= 1;
        } else {
#if 0 // no lock
            dt = tigrTime();
#elif 0 // naive
            sys_sleep(1000/50.f); // 50 -> 39 fps
            dt = tigrTime();
#elif 0 // less naive
            dt = tigrTime();
            if( dt < (1000/50.f) ) sys_sleep( (1000/50) - dt );
#elif 0 // try precise sleep (requires SDL_DelayPrecise backend)
            static uint64_t prev_dt = 0;
            uint64_t now_dt = time_ns();
            int64_t diff = now_dt - prev_dt;
            prev_dt = now_dt;

            float target = (ZX_FPSMUL/100.0) * (ZX < 128 ? 50.08:50.01);
            int64_t frame_in_ns = ((1000/target)*SDL_NS_PER_MS);
            if( diff < frame_in_ns ) sys_sleep_precisens( frame_in_ns - dt );

            dt = tigrTime();
#else // accurate (beware of CPU usage)
            float target = (ZX_FPSMUL/100.0) * (ZX < 128 ? 50.08:50.01);

            // be nice to os
            sys_sleep(ZX_FPSMUL > 120 ? 1 : 5);
            // complete with shortest sleeps (yields) until we hit target fps
            dt = tigrTime();
            for( float target_fps = 1.f/(target+!target); dt < target_fps; ) {
                sys_yield();
                dt += tigrTime();
            }
#endif
        }

        // zxplayer quits loop early, so...
        // it wont be drawing UI
        // it wont be drawing game browser
        // it wont be processing cmdline/FN-keys commands
        // it wont be processing drag 'n drops
        if( ZX_PLAYER ) {
            continue;
        }

        // calc fps
        static int frames = 0; ++frames;
        static double time_now = 0; time_now += dt;
        if( time_now >= 1 ) { fps = frames / time_now; time_now = frames = 0; }

        // tape timer
        if(tape_playing()) timer += dt;

        static char dev_status[128] = "";
        if( DEV || ZX_DEBUG ) {
            char *ptr = dev_status;
            ptr += sprintf(ptr, "%dm%02ds ", (unsigned)(timer) / 60, (unsigned)(timer) % 60);
            ptr += sprintf(ptr, "%5.2ffps%s %d mem%s%d%d%d%d ", fps, do_runahead ? "!":"", ZX, rom_patches ? "!":"", GET_MAPPED_ROMBANK(), (page128&8?7:5), 2, page128&7);
            ptr += sprintf(ptr, "%02X%c%02X %04X ", page128, page128&32?'!':' ', page2a, PC(cpu));
            ptr += sprintf(ptr, "%c%c%d %4dHz %dHz ", "  +-"[tape_inserted()*2+tape_level()], toupper(tape_peek()), mic_on, tape_hz, disk_hz);
        }

        // rec before/after UI,
        int rec(Tigr*);
        int gui(const char *);
        gui(dev_status), rec(ZX_PRINTUI ? app : canvas);

        #define LOAD(ZX,TURBO,file) if(file) do { \
                boot(ZX, 0); if(TURBO || key_pressed(TK_CONTROL)) rom_patch_turbo(); \
                if( !loadfile(file,1,0) ) { \
                    if( !load_shader( file ) ) { \
                        if( is_folder(file) ) cmdkey = 'SCAN', cmdarg = file; \
                        else alert(va("cannot open '%s' file\n", file)); \
                    } \
                } else titlebar(file), (free(ZX_MEDIA), ZX_MEDIA = strdup(file)); \
            } while(0)

        // parse drag 'n drops. reload if needed
        for( char **list = tigrDropFiles(app,0,0); list; list = 0)
        for( int i = 0; list[i]; ++i ) {
            #if TESTS
            LOAD(48,1,list[i]);
            #else
            LOAD(ZX,ZX_TURBOROM,list[i]);
            #endif
        }

        // parse cmdline. reload if needed
        do_once
        for( int i = 1; i < __argc; ++i )
        if( __argv[i][0] != '-' ) {
            #if TESTS
            LOAD(48,1,__argv[i]);
            #else
            LOAD(ZX,ZX_TURBOROM,__argv[i]);
            #endif
        }
        else if( __argv[i][1] == 'v' ) cmdkey = 'HELP';


        // clear command
        int cmdkey_ = cmdkey; cmdkey = 0;
        const char *cmdarg_ = cmdarg; cmdarg = 0;

        // parse commands
        ZX_FASTCPU = 0;
        ZX_FULLSCREEN = !tigrWindowed(app); // update var in case user pressed ALT+ENTER, or in some other external way
        switch(cmdkey_) { default: 
            #if DEV
            if(cmdkey_) alert(va("command not found `%08x`", cmdkey_));
            #endif
            break; case 'GAME':  if( ZX_BROWSER == 1 ? numgames : 1 ) active ^= 1, ui_dialog_new(NULL);
            break; case 'MAX':   ZX_FASTCPU = 1; // fast-forward cpu (hold)

            break; case 'PLAY':  tape_play(1); ZX_AUTOSTOP = 0; ZX_AUTOPLAY = 0; // tape_play(!tape_playing()); /*if(!tape_inserted()) active ^= 1; else tape_play(!tape_playing());*/ // open browser if start_tape is requested but no tape has been ever inserted
            break; case 'PREV':  tape_prev();
            break; case 'NEXT':  tape_next();
            break; case 'STOP':  tape_stop();  ZX_AUTOPLAY = 0; ZX_AUTOSTOP = 0;
            break; case 'EJEC':  tape_reset();
            break; case 'FAST':  ZX_FASTTAPE ^= 1;

            break; case  'TV':  { static int mode = 0; do_once mode = ZX_CRT << 1 | ZX_RF; mode = (mode + 1) & 3; if(cmdarg_) mode = atoi(cmdarg_); ZX_RF = mode & 1; crt(ZX_CRT = !!(mode & 2) ); }

            break; case 'FULL': { int mode = cmdarg_ ? atoi(cmdarg_) : (ZX_FULLSCREEN ^ 1);
                                if( app_create(window_title(NULL), mode, ZX_ZOOM) ) ZX_FULLSCREEN = mode; }

            break; case 'ZOOM': { int mode = (ZX_ZOOM + !ZX_ZOOM + 1) % 5; if( !app_wouldfit(mode) ) mode = 1; if( cmdarg_ ) mode = atoi(cmdarg_);
                                if( app_create(window_title(NULL), ZX_FULLSCREEN, mode) ) ZX_ZOOM = mode + !mode; }

            break; case 'SAVE':   quicksave(0);
            break; case 'LOAD':   quickload(0);

            break; case 'PIC':  cmdkey = 'PIC2', ZX_PRINTUI = 0 ^ (!!key_pressed( TK_SHIFT)); // resend screenshot cmd
            break; case 'PIC_': cmdkey = 'PIC2', ZX_PRINTUI = 1 ^ (!!key_pressed( TK_SHIFT)); // resend screenshot cmd
            break; case 'PIC2':
            break; case 'REC':  cmdkey = 'REC2', ZX_PRINTUI = 0 ^ (!!key_pressed( TK_SHIFT)); // resend recording cmd
            break; case 'REC_': cmdkey = 'REC2', ZX_PRINTUI = 1 ^ (!!key_pressed( TK_SHIFT)); // resend recording cmd
            break; case 'REC2':

            break; case 'TURB':  ZX_TURBOROM ^= 1; if(tape_inserted()) boot(ZX, 0|KEEP_MEDIA), reload(0); // toggle turborom and reload
            break; case 'BOOT':  reset(0|KEEP_MEDIA), reload(0);
            break; case 'NMI':   if( pins & Z80_NMI ) pins &= ~Z80_NMI; else pins |= Z80_NMI; RZX_reset(); // @todo: verify
            break; case 'WIPE':  reset(0); ZXDB = zxdb_free(ZXDB); if(ZX_MEDIA) (free(ZX_MEDIA), ZX_MEDIA = 0); if(ZX_TITLE) (free(ZX_TITLE), ZX_TITLE = 0); titlebar(""); // clear media    KEEP_MEDIA/*|QUICK_RESET*/);
            break; case 'POKE':  pok_apply(cmdarg_);
            break; case 'MODE': {
                // cycle
                static int models[] = { 16, 48, 128, 200, 210, 300, 128|1 };
                static int find = 0;
                while(models[find] != (ZX | ZX_PENTAGON))
                find = (find + 1) % countof(models);
                find = (find + 1) % countof(models);
                int model = models[find];

                // override model if arg supplied
                if(cmdarg_) model = atoi(cmdarg_);

                // toggle model and reload
                ZX = model & 0x1FE;
                ZX_PENTAGON = model & 1;

                boot(ZX, 0|KEEP_MEDIA), reload(ZX); // toggle model and reload last media

                // hack: force model if something went wrong. @fixme: investigate why
                if( model & 1 ) if(!ZX_PENTAGON) ZX_PENTAGON = 1, rom_restore();

                titlebar(ZX_TITLE); // refresh titlebar to reflect new model
            }
            // cycle AY cores
            break; case 'AY':    { const int table[] = { 1,2,0,0 }; ZX_AY = table[ZX_AY]; if(cmdarg_) ZX_AY=atoi(cmdarg_); }
            break; case 'WAVE':  ZX_WAVES ^= 1;                                   if(cmdarg_) ZX_WAVES=atoi(cmdarg_);
            break; case 'LENS':  ZX_LENSLOK ^= 1;                                 if(cmdarg_) ZX_LENSLOK=atoi(cmdarg_);
            break; case 'PAL':   ZX_PALETTE = (ZX_PALETTE+1)%countof(ZXPalettes); if(cmdarg_) ZX_PALETTE=atoi(cmdarg_);    palette_use(ZX_PALETTE, ZX_PALETTE == 0 || ZX_PALETTE == 4 ? 1 : 0);
            break; case 'FIRE':  ZX_AUTOFIRE = (ZX_AUTOFIRE+1)%4;                 if(cmdarg_) ZX_AUTOFIRE=atoi(cmdarg_);
            break; case 'GUNS':  ZX_GUNSTICK ^= 1;                                if(cmdarg_) ZX_GUNSTICK=atoi(cmdarg_);   if(ZX_GUNSTICK) ZX_MOUSE = 0, ZX_JOYSTICK = 0; // cycle guns
            break; case 'MICE':  ZX_MOUSE ^= 1;                                   if(cmdarg_) ZX_MOUSE=atoi(cmdarg_);      if(ZX_MOUSE) ZX_GUNSTICK = 0;                  // cycle kempston mouse(s)
            break; case 'ULA':   ZX_ULAPLUS = (ZX_ULAPLUS+1)%3;                   if(cmdarg_) ZX_ULAPLUS=atoi(cmdarg_);    // cycle ulaplus
            break; case 'RUN':   ZX_RUNAHEAD ^= 1;                                if(cmdarg_) ZX_RUNAHEAD=atoi(cmdarg_);   // cycle runahead mode
            break; case 'DEV':   ZX_DEBUG ^= 1;                                   if(cmdarg_) ZX_DEBUG=atoi(cmdarg_);
            break; case 'KL':    ZX_KLMODE ^= 1;                                  if(cmdarg_) ZX_KLMODE=atoi(cmdarg_);     ZX_KLMODE_PATCH_NEEDED = 1;
            break; case 'TENG':  ZX_AUTOLOCALE ^=1;                               if(cmdarg_) ZX_AUTOLOCALE=atoi(cmdarg_); if(ZX_AUTOLOCALE) translate(mem, 0x4000*16, 'en');
            if(ZX_AUTOLOCALE) {
                ui_dialog_new(NULL);
                ui_dialog_option(0,"Game patched. Play, then go to main menu.\n\n",NULL,0,NULL);
                ui_dialog_option(1,"OK\n",NULL,0,NULL);
            }
            // break; case 'POLR':  mic_low ^= 64;                                if(cmdarg_) mic_low=atoi(cmdarg_);
            break; case 'KEYB':  issue2 ^= 1;                                     if(cmdarg_) issue2=atoi(cmdarg_)==2;
#if 0
                                    int do_reset = tape_playing() && q.debug && !strchr("uol", q.debug);
                                    if( do_reset ) {
                                        reset(KEEP_MEDIA), loadfile(ZX_MEDIA,1,0);
                                    }
#endif
            break; case 'CPU':   { const float table[] = { [50]=60,[60]=100,[100]=120,[120]=150,[150]=200,[200]=400,[400]=50 };
                                 ZX_FPSMUL = table[(int)(ZX_FPSMUL)]; if(cmdarg_) ZX_FPSMUL=atoi(cmdarg_); }

            break; case 'HELP':  help();

            break; case 'SCAN':  for( const char *f = cmdarg_ && cmdarg_[0] ? cmdarg_ : app_selectfolder("Select games folder"); f ; f = 0 ) {
                                    // ZX_FOLDER && REALLOC((void*)ZX_FOLDER, 0); // @leak (lubuntu16 would display rubbish otherwise)
                                    rescan( ZX_FOLDER = STRDUP(f) ), /*active = !!numgames,*/ ui_dialog_new(NULL);
                                }

            break; case 'DEVT': ZX_DEVTOOLS ^= 1;

            break; case 'LINK': visit(cmdarg_);

            break; case 'F01': vk_redefine(1);
            break; case 'F02': vk_redefine(2);
            break; case 'F03': vk_redefine(3);
            break; case 'F04': vk_redefine(4);
            break; case 'F05': vk_redefine(5);
            break; case 'F06': vk_redefine(6);
            break; case 'F07': vk_redefine(7);
            break; case 'F08': vk_redefine(8);
            break; case 'F09': vk_redefine(9);
            break; case 'F0:': vk_redefine(10);
            break; case 'F0;': vk_redefine(11);
            break; case 'F0<': vk_redefine(12);

            break; case 'LIST': { // cmdarg in "#ID" format. @todo: add LIST redefineable cmd
                int id = atoi(cmdarg_+1);
                zxdb z = zxdb_search(cmdarg_);

                int count = 0;
                char *list[256] = {0};

                const char *url; int slot = -1;
                ui_dialog_new("- Change media -");
                do {
                    url = zxdb_url(z, va("%d",++slot));
                    if(!url) continue;
                    puts(url);

                    const char *base = strrchr(url, '/')+1;

                    // common game images here only
                    // exclude wav/mp3/ay/html/pdf/txt/pok/rzx from listing
                    int supported = 0;
                    if(!supported) supported |= !!strendi(base, ".sna.zip");
                    if(!supported) supported |= !!strendi(base, ".z80.zip");
                  //if(!supported) supported |= !!strendi(base, ".szx.zip"); // not yet fully supported (see: RaidAereoEm1934.szx.zip)
                  //if(!supported) supported |= !!strendi(base, ".rzx.zip"); // not yet fully supported
                    if(!supported) supported |= !!strendi(base, ".tap.zip");
                    if(!supported) supported |= !!strendi(base, ".tzx.zip");
                    if(!supported) supported |= !!strendi(base, ".pzx.zip"); // (see: Duel2.pzx.zip)
                    if(!supported) supported |= !!strendi(base, ".rom.zip"); // (see: MagTheMagician.rom.zip)
                    if(!supported) supported |= !!strendi(base, ".dsk.zip");
                    if(!supported) supported |= !!strendi(base, ".trd.zip");
                    if(!supported) supported |= !!strendi(base, ".scl.zip"); // (see: Seraphima.scl.zip)
                    if(!supported) supported |= !!strendi(base, ".fdi.zip"); // (see: BlackRaven.fdi.zip)
                    if(!supported) continue;

                    if( strstri(base, "(MasterTape)") ) continue; // wat (see: Nexor)
                    if( strstri(base, "(MasterDisk)") ) continue; // wat (see: BlinkysScarySchool)

#if 0
                    (see: Cabal.ay.zip)
                    (see: AirborneRanger.html.zip)
                    (see: EliteLegend_Info.pdf.zip)
                    
                    (see: JackTheNipper.rzx.zip)
                    (see: FiremanSam_2.slt.zip)
                    (see: DungDarach.wav.zip)
                    (see: AfterBurner.mp3.zip)

                    (see: TheRingOfTheInka.mdr.zip or WhereTimeStoodStill)
                    (see: TheRingOfTheInka.mdv.zip)
                    (see: TheRingOfTheInka.if1.zip)

                    (see: CivilServiceII.mgt.zip)
                    (see: FallingDown.d80.zip)
                    (see: PhantomF4II.d40.zip)

                    (see: ShootOutZX80.o.zip)
                    (see: NowotnikPuzzleThe_3.p.zip)
#endif

                    // Myth-HistoryInTheMaking(MCMSoftwareSA)(SideA).dsk.zip > <DSK|(MCMSoftwareSA)(SideA)|#id#slot
                    char pretty[256]; snprintf(pretty, 256, "%.*s", (int)(strlen(base) - 4), base); // remove zip/rar
                    char ext[8]; snprintf(ext, 8, "%s", strrchr(pretty, '.') + 1); // find ext
                    for( char *s = ext; *s; s++) *s = toupper(*s);
                    const char *tags = strlen(base) > 53/*(_320/(8+1))*/ && strchr(base, '(') ? strchr(base, '(') : base;
                    list[count++] = va("<%s|%.*s|#%d#%d", ext, (int)(strlen(tags)-8*(tags!=base)),tags, id, slot ); // compose ext|name|#id

                } while(url);

                // cancel
                if( count == 0 ) ui_dialog_new(NULL);
                // sort and queue
                if( count > 0 ) {
                    qsort(list, count, sizeof(char *), qsort_strcmp);

                    unsigned hits = -1;
                    unsigned type1 = *((unsigned *)list[0]);

                    for( int i = 0; i < count; ++i ) {
                        puts(list[i]);

                        unsigned type2 = *((unsigned *)list[i]);
                        int sameline = type1 == type2;
                        type1 = type2;

                        char *ext = list[i];
                        char *name = strchr(ext, '|'); *name++ = '\0';
                        char *zxid = strchr(name, '|'); *zxid++ = '\0';

                        const char *romans[] = {
                            ext, "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X"
                        };
                        hits = (hits + 1) * sameline;
                        if( !sameline ) ui_dialog_separator();
                        ui_dialog_option(1, romans[hits], name, 'ZXDB',zxid);

                    }

                    ui_dialog_separator(), ui_dialog_separator(), ui_dialog_cancel();
                }

                zxdb_free(z);
            }

            break; case 'ZXDB': {
                eject();
                if( zxdb_load(cmdarg_, 0) ) {
                    (free(ZX_MEDIA), ZX_MEDIA = strdup(cmdarg_));
                    // update titlebar
                    if( ZXDB.ids[0] )
                        titlebar( ZXDB.ids[2] );
                    // small addendum for tape files that require custom roms
                    /**/ if( !strcmp(cmdarg_,"#4424") ) cmdarg_ = "#4424#10"; // shadow of the unicorn
                    else cmdarg_ = 0;
                    //
                    if( cmdarg_ ) {
                        int len; char *bank = zxdb_download2_unc(cmdarg_, &len);
                        if( bank && len && len < 65536 ) memcpy(rom, bank, len);
                        if( bank ) free(bank);
                        z80_reset(&cpu);
                    }
                } else {
                    // show error dialog
                    ui_dialog_new("Cannot load media");
                    ui_dialog_ok();
                }
            }

            break; case 'JOY0': {
                // 0: no, |1: cursor/protek/agf, |2: kempston, |4: sinclair1, |8: sinclair2, |16:fuller, |32:kempston2, >=256:... custom mappings

                const char *buttons[] = { "□\f\f", "□\b\b\b\b\b\b\b\b\5√\7\f\f\f" };

                #define UI_JOYSTICK(name,condition,toggle,set,hint) \
                ui_dialog_option(1|4,va("<%s", buttons[!!(condition)]),NULL,'JOY',toggle); \
                ui_dialog_option(1|4,va("%s\n", name),hint,'JOY',set);

                ui_dialog_new("- Toggle Joysticks -");
                UI_JOYSTICK("Fuller",     ZX_JOYSTICK&16,"^16","=16", "-Less common interface with unique key mapping-");
                UI_JOYSTICK("Kempston",   ZX_JOYSTICK& 2, "^2", "=2", "-Most popular interface with wide game support-");
                UI_JOYSTICK("Kempston B", ZX_JOYSTICK&32,"^32","=32", "-Additional Kempston interface in Port 55-");
                UI_JOYSTICK("Sinclair 1", ZX_JOYSTICK& 4, "^4", "=4", "-Left port on Interface 2, mapped to keys 1:5-");
                UI_JOYSTICK("Sinclair/Interface 2", ZX_JOYSTICK& 8,"^8","=8", "-Right port on Interface 2, mapped to keys 6:0-");
                UI_JOYSTICK("Cursor (Protek, AGF)",ZX_JOYSTICK& 1, "^1", "=1", "-Early interface with BASIC arrow keys mapping-");
                UI_JOYSTICK("No joystick\n\n",ZX_JOYSTICK==0,"=0","=0", "-Disable joysticks-");
                ui_dialog_option(1|4,"Preset\f\f","-Reset to default configuration-",'JOY',"=19");
                ui_dialog_option(1,"OK\n",NULL, 0,NULL);
            }

            break; case 'JOY':
            {
                if( cmdarg_ ) { // toggles
                    unsigned m = atoi(cmdarg_+1);
                    if(cmdarg_[0]=='=') ZX_JOYSTICK = m;
                    if(cmdarg_[0]=='^') ZX_JOYSTICK ^= m; // (ZX_JOYSTICK & ~m) | ((ZX_JOYSTICK & m) ^ m); // '^' case
                    cmdkey = 'JOY0';
                } else { // cycle
                    // ZX_JOYSTICK = (ZX_JOYSTICK+1)%4; if(ZX_JOYSTICK==3) ZX_JOYSTICK=1|2|16;
                    int next[] = {[0]=4,[4]=8,[8]=1,[1]=2,[2]=1|2|16,[1|2|16]=0,[255]=0};
                    ZX_JOYSTICK = next[ZX_JOYSTICK];
                }
                if(ZX_JOYSTICK) ZX_GUNSTICK = 0; // prevent gunstick/lightgun conflict
            }

            break; case 'PAD0': { // setup remap
                memcpy(ZX_PAD_, ZX_PAD, sizeof(ZX_PAD_));

                ui_dialog_new("- Gamepad to joystick/keyboard bindings -");

    #if 1
                ui_dialog_option(0,"⭠ ⭢ ⭡ ⭣ \4A \2B \5X \6Y \7LB RB LT RT LS RS Bk St\n",NULL,0,0);
                ui_dialog_option(2,"▁ . . . . . . . .. .. .. .. .. .. .. ..\n",NULL,0,0);
    #else
                ui_dialog_option(0,"   ⭠ ⭢ ⭡ ⭣ \4A \2B \5X \6Y \7LB RB LT RT LS RS Bk St\n",NULL,0,0);
                ui_dialog_option(2,"   ▁ . . . . . . . .. .. .. .. .. .. .. ..\n",NULL,0,0);
                ui_dialog_option(2, "LT . . . . . . . . .. .. .. .. .. .. .. ..\n",NULL,0,0);
                ui_dialog_option(2, "RT . . . . . . . . .. .. .. .. .. .. .. ..\n",NULL,0,0);
    #endif

                ui_dialog_separator();
                ui_dialog_option(1,"Save\n",0,'PAD2',0);
                ui_dialog_option(1,"Cancel\n",0,0,0);

                cmdkey = 'PAD1';
            }

            break; case 'PAD1': // tick remap
            {
                cmdkey = 'PAD1'; // repeat

                // scan keyboard
                int vk = -1;
                char *keys = tigrKeys(app);
                for( int i = 0; i < 256; ++i ) {
                    if( keys[i] > keys[i+256] ) {
                        vk = i;
                        break;
                    }
                }
                // scan option. @fixme: this is hacky
                int option = -1;
                for( int i = 0; i < num_options; ++i) {
                    if( options[i].flags & 2 ) {
                        option = i;
                        break;
                    }
                }
                //
                static const char *keysicons[256] = {0}, **keysallowed = keysicons;
                do_once {
                    static char ascii[256][2] = {0};
                    for( int i = 32; i < 128; ++i ) {
                        ascii[i][0] = i;
                        keysicons[i] = ascii[i];
                    }
                    keysicons[TK_SPACE] = "□";
                    keysicons[TK_LEFT] = "🮤";
                    keysicons[TK_RIGHT] = "🮥";
                    keysicons[TK_UP] = "🮧";
                    keysicons[TK_DOWN] = "🮦";
                    keysicons[TK_TAB] = ""; //"";
                //    TK_PAD0=128,TK_PAD1,TK_PAD2,TK_PAD3,TK_PAD4,TK_PAD5,TK_PAD6,TK_PAD7,TK_PAD8,TK_PAD9,
                //    TK_PADMUL,TK_PADADD,TK_PADENTER,TK_PADSUB,TK_PADDOT,TK_PADDIV,
                //    TK_F1,TK_F2,TK_F3,TK_F4,TK_F5,TK_F6,TK_F7,TK_F8,TK_F9,TK_F10,TK_F11,TK_F12,
                //    TK_BACKSPACE,TK_TAB,TK_RETURN,TK_SHIFT,TK_CONTROL,TK_ALT,TK_PAUSE,TK_CAPSLOCK,
                //    TK_ESCAPE,TK_SPACE,TK_PAGEUP,TK_PAGEDN,TK_END,TK_HOME,TK_LEFT,TK_UP,TK_RIGHT,TK_DOWN,
                //    TK_INSERT,TK_DELETE,TK_LWIN,TK_RWIN,TK_NUMLOCK,TK_SCROLL,TK_LSHIFT,TK_RSHIFT,
                //    TK_LCONTROL,TK_RCONTROL,TK_LALT,TK_RALT,TK_SEMICOLON,TK_EQUALS,TK_COMMA,TK_MINUS,
                //    TK_DOT,TK_SLASH,TK_BACKTICK,TK_LSQUARE,TK_BACKSLASH,TK_RSQUARE,TK_TICK
                //    ,TK_PRINT //< @r-lyeh
                }

                // num_buttons
                enum { _12 = countof(ZX_PAD_) };

                // apply (1x8 chars + 2x4 chars)
                if( option >= 0 && vk >= 0 && (vk == TK_BACKSPACE || keysallowed[vk]) ) {

                    // add key to mappings. terminate if no more room.
                    int slot = 0; while(ZX_PAD_[slot] && slot < _12) slot++;
                    if( slot < _12 ) ZX_PAD_[slot] = vk;

                    // delete last mapping if requested
                    if( vk == TK_BACKSPACE ) {
                        if(slot < _12 ) ZX_PAD_[slot] = 0;
                        if(slot > 0) ZX_PAD_[slot-1] = 0;
                    }
                }

                if( key_longpress(TK_BACKSPACE) )
                    memset(ZX_PAD_, 0, sizeof(ZX_PAD_));

                if( option >= 0 )
                {
                    // redraw mappings
                    const char *cursor = "▁";
                    char buf[256], *ptr = buf;
                    for( int i = 0; i < _12; ++i ) {
                        if( ZX_PAD_[i] ) {
                            if( i < 8 )
                            ptr += sprintf(ptr, " %s", keysicons[ZX_PAD_[i]]);
                            else
                            ptr += sprintf(ptr, " %s.", keysicons[ZX_PAD_[i]]);
                        } else {
                            if( i < 8 )
                            ptr += sprintf(ptr, " %s", cursor);
                            else
                            ptr += sprintf(ptr, " %s.", cursor);

                            cursor = ".";
                        }
                    }

                    (void)REALLOC((void*)options[option].text, 0);
                    options[option].text = STRDUP(buf+1);
                }
            }

            break; case 'PAD2': // apply remap
                memcpy(ZX_PAD, ZX_PAD_, sizeof(ZX_PAD_));

            break; case 'MP3P': // mp3 play
                {
                    void *mp3data; int mp3len;
                    sscanf(cmdarg_, "%p %d", &mp3data, &mp3len);

                    drmp3_uint64 mp3_fc;
                    drmp3_config mp3_cfg = {0};
                    for( float *fbuf = drmp3_open_memory_and_read_pcm_frames_f32(mp3data, mp3len, &mp3_cfg, &mp3_fc, NULL); fbuf ; fbuf = 0 ) { // @fixme: @leak
                        int channels = mp3_cfg.channels;
                        int frequency = mp3_cfg.sampleRate;
                        int supported = channels <= 2 && (frequency == 44100 || frequency == 22050);

                        if( !supported ) alert(va("Unsupported MP3 audio stream, %dch %dHz", channels, frequency));
                        if( !supported ) { free(fbuf); continue; }

                        int length16 = mp3_fc;
                        void *data16 = fbuf;

                        // [x] 2ch 44 Everyones a Wally
                        // [x] 1ch 44 SideArms
                        // [x] 1ch 22 Outrun

                        if( channels == 1 ) {
                            if( frequency == 44100 ) { // downsample to 22khz
                                float *s = data16;
                                for( int i = 0; i < length16/2; ++i ) {
                                    s[i] = (s[i*2+0] + s[i*2+1]) / 2;
                                }
                            }
                            play_stream('mp3', data16, length16 / 2, -1);
                        }
                        if( channels == 2 ) { // downsample to mono
                            float *s = data16;
                            for( int i = 0; i < length16; ++i ) {
                                s[i] = (s[i*2+0] + s[i*2+1]) / 2;
                            }
                            if( frequency == 44100 ) { // downsample to 22khz
                                float *s = data16;
                                for( int i = 0; i < length16; ++i ) {
                                    s[i] = (s[i*2+0] + s[i*2+1]) / 2;
                                }
                            }
                            play_stream('mp3', NULL,0, 0); // stop voice
                            play_stream('mp3', data16, length16 / 2, -1);

                            // emit a warning if not played in ideal conditions
                            if( tape_playing() ? ZX_FASTTAPE : ZX_FASTCPU ) {
                                ui_dialog_new("- Warning -");
                                ui_dialog_option(0,"Not played in ideal conditions. Consider stopping\nthe tape and using normal cpu speed for best\nexperience.\n",NULL,0,NULL);
                                ui_dialog_ok();
                            }
                        }
                    }
                }

            break; case 'MP3M': // mp3 mute
               play_stream('mp3', NULL,0, 0); // stop voice

            break; case 'MP3L': // mp3 list
                ui_dialog_new("-Select track-");

                for( int len = 0; len >= 0; len = -1)
                for( char *url = zxdb_url(ZXDB,"mp3"); url; url = 0)
                for( char *data = zxdb_download(ZXDB, url, &len); data; free(data), data = 0 ) {
                    if( len > 2 && data[0] == 'P' && data[1] == 'K' ) {
                        writefile(".Spectral/$$mp3.zip", data, len);

                        zip *z = zip_open(".Spectral/$$mp3.zip","rb");
                        if( z ) {
                            for( int i = 0; i < zip_count(z); ++i ) {
                                const char *fname = zip_name(z,i);
                                if(!strendi(fname, ".mp3")) continue;

                                int unlen = zip_size(z, i);
                                void *unzipped = zip_extract(z, i); // @leak

                                ui_dialog_option(1,va("Track %02d\n",i+1),fname,'MP3P',va("%p %d",unzipped,unlen));
                            }
                            zip_close(z);
                        }

                        unlink(".Spectral/$$mp3.zip");
                    } else {
                        ui_dialog_option(1,va("Track %02d\n",1),strrchr(url,'/')+1,'MP3P',va("%p %d",data,len));
                        data = 0; // @leak
                    }
                }

                ui_dialog_separator();
                ui_dialog_option(1,"Cancel\n",NULL,'MP3M',0);

        }

    // shutdown zxdb browser if user is closing window app. reasoning for this:
    // next z80_opdone() will never finish because we dont tick z80 when zxdb browser is enabled
    // active *= !window_closed();

    #if 0 && NEWCORE
    // ensure there is no pending opcode before exiting main loop: spectral.sav would be corrupt otherwise.
    // also, do not loop indefinitely on invalid DI+HALT combo, which we use in our .SCR viewer.
    // update: moved logic that bypasses z80_opdone(&cpu) in HALT state. rewritten as a forced/benign ZX_Key(ZX_SHIFT) operation; see: input() function.
    // printf("%d %018llx %d %d %d\n", z80_opdone(&cpu), cpu.pins, cpu.step, IFF1(cpu), IFF2(cpu));
    } while( z80_opdone(&cpu) ? 0 : 1 ); // (cpu.pins & Z80_HALT) && (cpu.step == 1) ? 0 : 1 ); // (cpu.pins & Z80_HALT) && (cpu.step == 1 || cpu.step==_z80_special_optable[4] || cpu.step==_z80_special_optable[5]) ? 0 : 1 );
    #endif

    } while( 1 ); // !window_closed() );

    // restore os keys
    enable_os_keys(1);

    // zxplayer does not save state
    if(!ZX_PLAYER)

    {
        // export config
        if(ZX_MEDIA) ZX_MEDIA = strdup(va("%s@%llu", ZX_MEDIA, media_seek[0] = voc_pos)); // @leak
        save_config();

        // export state
        // media_seek[0] = voc_pos / (double)(voc_len+!voc_len);
        for( FILE *state = fopen("Spectral.sav","wb"); state; fclose(state), state = 0) {
            if( !export_state(state) )
                alert("Error exporting state");
        }
    }

    tigrFree(app);
    return 0;
}

void on_cmd(unsigned key, const char *arg) {
    cmdkey = key;
    cmdarg = arg;
}

int gui(const char *status) {
    if( DEV ) {
        float x = 0.5, y = 25.5;
        ui_print(ui,  x*11, y*11, ui_colors, status);
    }

    if( ZX_DEBUG ) {
        tigrClear(dbg, tigrRGBA(0,0,0,128));

        float x = 0.5, y = 2;

        ui_print(dbg, x*11, y*11, ui_colors, status), y += 1.5;

        ui_print(dbg, x*11, y*11, ui_colors, regs(0)), y += 5;

        ui_print(dbg, x*11, y*11, ui_colors, dis(PC(cpu), 22)), y += 22;
    }

    // audio, stats & debug
    if( !active && !do_overlay ) {
        if( ZX_WAVES && ZX_AY > 0 ) {
            int count = _320;
            int tail = audio_write - count;
            if( tail < 0 ) tail = 0;
            float *audioch[1+3+3] = { 
                audio_buffer1+tail, // BEEPER
                audio_buffer2+tail, audio_buffer3+tail, audio_buffer4+tail, // AY1
                audio_buffer5+tail, audio_buffer6+tail, audio_buffer7+tail, // AY2
            };
            draw_audio(ui, 1+3*(ZX > 48)+3*ZX_PENTAGON, audioch, count);
        }
        if( ZX_LENSLOK && !num_options ) {
            if( mouse().rb ) ZX_LENSLOK = 0;
            else lenslok(ui, canvas, mouse_offsets()[0], mouse_offsets()[1] );
        }
    }
    draw_ui();

    if( ZX_DEBUG )
    tigrBlitAlpha(app, dbg, 0,0, 0,0, _320,_240, 1.0f);

    // flush ui
    ui_frame_end();

    // draw ui on top
    tigrBlitAlpha(app, ui, 0,0, 0,0, _320,_240, 1.0f);

    // draw overlay on top
    if( do_overlay ) {
        struct mouse m = mouse();
        tigrRenderMap(app, overlay, m.x, m.y, m.buttons, -m.wheel);
    }

    // draw irc on top
    if( do_irc ) {

        do_once {
            int chat_printf(const char *fmt, ...);
            ircd_printf = chat_printf;

            static char host[] = "eu.dal.net"; // irc.dal.net eu.dal.net uk.eu.dal.net halcyon.dal.net
            static int port = 6667;

        #ifdef _WIN32
            WSADATA wsa;
            WSAStartup(MAKEWORD(2, 2), &wsa);
        #endif

            strcpy(m_ircd.nick, "rlyeh");
            strcpy(m_ircd.name, "Player");
            strcpy(m_ircd.chan, "#spectral105");

            ircd_init(&m_ircd, host, port);
        }

        ircd_update(&m_ircd);

        int x = 0, y = 0;
        tigrBlitAlpha(app, irc, x,y, 0,0, _320,_240, 1.0f);
    }

    // render dialog modal on top
    if( ui_dialog_render(dialog, mouse().wheel) ) {
        // composite result
        tigrBlitAlpha(app, dialog, 0,0, 0,0, _320,_240, 1.0f);
    }

    // test gamepad bindings
    if( DEV ) {
        static int gx, gy, bt;
        do_once gx = _320/2, gy = _240*0.75, bt = 0;
        if( ZX_PAD[1] ) if( tigrKeyHeld(app, ZX_PAD[1]) ) gx -= 1;
        if( ZX_PAD[2] ) if( tigrKeyHeld(app, ZX_PAD[2]) ) gx += 1;
        if( ZX_PAD[3] ) if( tigrKeyHeld(app, ZX_PAD[3]) ) gy -= 1;
        if( ZX_PAD[4] ) if( tigrKeyHeld(app, ZX_PAD[4]) ) gy += 1;
        if( ZX_PAD[5] ) bt= tigrKeyHeld(app, ZX_PAD[5]);
        ui_print(app, gx % _320, gy % _240, ui_colors, bt ? "" : "");
    }

    return 1;
}

int rec(Tigr *canvas) {
    // screenshots
    if( cmdkey == 'PIC2' ) {
        screenshot( window_title(NULL), canvas );
        play('cam', 1);
    }
    // videos
    if( cmdkey == 'REC2' ) {
        /**/ if( record_active() ) record_stop(), play('tape', 0);
        else switch( record_start(va("%s-%04x.mp4", window_title(NULL), file_counter(SLOT_MP4)), window_width(), window_height(), 50) ) {
                default: alert("Cannot record video");
                break; case 2: play('tape', 1);
                break; case 1: alert("FFMPEG" ifdef(win32,".exe","") " not " ifdef(win32,"found","installed") ".\nUsing suboptimal MPEG1 encoder.\n"); play('tape', 1);
        }
    }
    // video
    if( record_active() ) {
        record_frame( canvas->pix, canvas->w, canvas->h );
    }
    return 1;
}

int chat_printf(const char *fmt, ...) {
    static char *buf = 0; if(!buf) buf = REALLOC(0,64*1024);
    
    va_list vl;
    va_start(vl,fmt);
    int rc = vsnprintf(buf, 64*1024-1, fmt, vl);
    va_end(vl);

    printf("%s", buf);

    if( *buf == ':' ) {
        replace(buf, "~", "-"); // for ui_print()

        static int x = 0, y = 0;
        ui_stack bak = ui_push();
        ui_at(irc, x,y);

        char *buf2 = buf, *line;
        while( NULL != (line = strsep(&buf2, "\r\n")) ) {
            if( *line == '\r' || *line == '\n' ) continue;
            char *text = 0; {
                if( *line == ':' ) {
                    text = strchr(line+1, ':');
                    if( text && *text == ':' ) ++text;
                }
            }
            if( text ) {
                ui_label(text);
                ui_x = 0;
                ui_y += 11;
            }
        }

        x = ui_x, y = ui_y;
        ui_pop(bak);
    }

    return rc;
}
