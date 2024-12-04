# System3 for SDL2

This is an SDL2 port of
[System3 for Win32](http://takeda-toshiya.my.coocan.jp/alice/) by Takeda
Toshiya. It supports multiple platforms, including Android and Emscripten.

## Building

### Linux (Debian, Ubuntu)

```bash
$ git submodule update --init
$ sudo apt install g++ cmake libsdl2-dev libsdl2-ttf-dev libsdl2-mixer-dev librtmidi-dev
$ mkdir -p out/debug
$ cd out/debug
$ cmake -DCMAKE_BUILD_TYPE=Debug ../../
$ make
$ sudo make install
```

### MacOS

```bash
$ git submodule update --init
$ brew install cmake pkg-config sdl2 sdl2_ttf sdl2_mixer rtmidi
$ mkdir -p out/debug
$ cd out/debug
$ cmake -DCMAKE_BUILD_TYPE=Debug ../../
$ make
$ sudo make install
```

### Windows (MSYS2)

```bash
$ git submodule update --init
$ pacman -S make mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_ttf mingw-w64-ucrt-x86_64-rtmidi
$ mkdir -p out/debug
$ cd out/debug
$ cmake -G"MSYS Makefiles" -DCMAKE_BUILD_TYPE=Debug ../../
$ make
```

### Windows (Microsoft Visual Studio)

- Visual Studio 2019 can be used to clone this repository and will
  automatically clone submodules as well.
  - If you're using an older version of Visual Studio, install Git and clone
    this repository using the `--recurse-submodules` option.
- Install [CMake](https://cmake.org/download/). (The CMake integration in
  Visual Studio does not work.)
- In the CMake GUI, press the "Browse Source..." button and select the root
  folder of this repository.
- Press the "Browse Build..." button. Create a new folder (e.g., `out`) under
  the top-level directory of the repository and select it.
- Press the "Configure" button. Specify the generator for your version of
  Visual Studio and click "Finish."
- Press the "Generate" button.
- A `System3.sln` file should be generated in the build folder. Open it with
  Visual Studio.

### Emscripten

```bash
$ git submodule update --init
$ mkdir -p out/wasm
$ cd out/wasm
$ emcmake cmake -DCMAKE_BUILD_TYPE=Release ../../
$ make
```

To use the Emscripten build, check out https://github.com/kichikuou/web and
copy the `out/wasm/system3.*` files into its `docs` directory.

### Android

See [android/README.md](android/README.md).

### Nintendo Switch

See [switch/README.md](switch/README.md).

## Running

Usage:

```bash
system3 [options]
```

### Options

#### `-noantialias`
Disables text anti-aliasing.

#### `-fontfile` _filename_
Specifies the font file used for rendering text. Both `.ttf` and `.otf` files
are supported.

#### `-playlist` _filename_
_filename_ is a text file that lists the audio files to play in lieu of CD
audio tracks, one per line. For example:

```plaintext
# This line is ignored
BGM/track02.mp3
BGM/track03.mp3
...
```
The first line is not used because track 1 on a game CD is usually a data
track.

#### `-fm`
By default, system3-sdl2 uses MIDI sound if available. This option forces FM
tone generator emulation.

#### `-mididevice` _number_
Specifies the MIDI device number to use. If not specified, the first available
device is used.

#### `-texthook` _mode_
Specifies the text hook mode. This option is used for capturing in-game text
for translation or other purposes. The available modes are:
- `none`: Disable text hooking.
- `print`: Print the captured text to the console.
- `copy`: Copy the captured text to the clipboard.

#### `-game` _game_id_
As System1-3 have slight variations depending on the game, `system3` uses the
fingerprint of the scenario file (ADISK.DAT) to identify the game being played.
This option allows you to override this detection, which is useful when running
patched games.

Here is a list of available game IDs and their corresponding titles:

| game_id | Title | Notes |
----------|-------|--------
| `bunkasai` | あぶない文化祭前夜 | |
| `crescent` | クレセントムーンがぁる | |
| `dps` | D.P.S - Dream Program System | |
| `dps_sg_fahren` | D.P.S SG - Fahren Fliegen | |
| `dps_sg_katei` | D.P.S SG - 家庭教師はステキなお仕事 | |
| `dps_sg_nobunaga` | D.P.S SG - 信長の淫謀 | |
| `dps_sg2_antique` | D.P.S SG set2 - ANTIQUE HOUSE | |
| `dps_sg2_ikenai` | D.P.S SG set2 - いけない内科検診再び | |
| `dps_sg2_akai` | D.P.S SG set2 - 朱い夜 | |
| `dps_sg3_rabbit` | D.P.S SG set3 - Rabbit P4P | |
| `dps_sg3_shinkon` | D.P.S SG set3 - しんこんさんものがたり | |
| `dps_sg3_sotsugyou` | D.P.S SG set3 - 卒業 | |
| `fukei` | 婦警さんＶＸ | |
| `intruder` | Intruder -桜屋敷の探索- | |
| `tengu` | あぶないてんぐ伝説 | |
| `toushin_hint` | 闘神都市 ヒントディスク | 闘神ランス does not work |
| `little_vampire` | Little Vampire | |
| `yakata` | ALICEの館 | |
| `ayumi_fd` | あゆみちゃん物語 | |
| `ayumi_hint` | あゆみちゃん物語 ヒントディスク | |
| `ayumi_proto` | あゆみちゃん物語 PROTO | |
| `dalk_hint` | DALK ヒントディスク | 零式 does not work |
| `drstop` | Dr. STOP! | |
| `prog_fd` | Prostudent G (FD) | |
| `rance3_hint` | Rance3 ヒントディスク | 走り女 does not work |
| `sdps_maria` | Super D.P.S - マリアとカンパン | |
| `sdps_tono` | Super D.P.S - 遠野の森 | |
| `sdps_kaizoku` | Super D.P.S - うれしたのし海賊稼業 | |
| `yakata2` | ALICEの館II | 殴りまくりたわぁ and おかゆの逆襲 do not work |
| `ambivalenz_fd` | AmbivalenZ −二律背反− (FD) | |
| `ambivalenz_cd` | AmbivalenZ −二律背反− (CD) | |
| `dps_all` | D.P.S. 全部 | |
| `funnybee_cd` | 宇宙快盗ファニーBee (CD) | |
| `funnybee_fd` | 宇宙快盗ファニーBee (FD) | |
| `onlyyou` | Only You −世紀末のジュリエット達− | |
| `onlyyou_demo` | Only You −世紀末のジュリエット達− デモ版 | |
| `prog_cd` | Prostudent G (CD) | |
| `prog_omake` | Prostudent G おまけ | |
| `rance41` | ランス 4.1 〜お薬工場を救え！〜 | |
| `rance42` | ランス 4.2 〜エンジェル組〜 | |
| `ayumi_cd` | あゆみちゃん物語 (CD) | |
| `ayumi_live_256` | あゆみちゃん物語 実写版 | |
| `ayumi_live_full` | あゆみちゃん物語 フルカラー実写版 | |
| `yakata3_cd` | アリスの館3 (CD) | |
| `yakata3_fd` | アリスの館3 (FD) | |
| `hashirionna2` | 走り女2 | |
| `toushin2_gd` |闘神都市2 グラフィックディスク | |
| `toushin2_sp` | 闘神都市2 そして、それから… | |
| `otome` | 乙女戦記 | |
| `ningyo` | 人魚 -蘿子- | |
| `mugen` | 夢幻泡影 | |

### Configuration File `system3.ini`

Every option that can be set via command line flags can also be configured
through the `system3.ini` file located in the game folder. Refer to
[`system3.ini.example`](system3.ini.example) for the file format and available
options. Options specified on the command line will override those in
`system3.ini`.

## Localizing a Game

System3-sdl2 supports localization, whereas the original System1-3 only
supported Japanese. If you're interested in translating games, check out
[Sys0Decompiler](https://alicesoft.miraheze.org/wiki/User_blog:RottenBlock/System_Programming_Resources).

When running a modified (translated) game, system3 cannot automatically detect
the game ID. You need to specify the `game` option in `system3.ini`.
