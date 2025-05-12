
## CNC Lathe Marlin 
This a version of Marlin modified for Team Chuck Key Cheese's CNC Lathe for the MIT class 2.72 Elements of Mechanical Design. Our main modification was adding custom firmware for supporting conversational lathe CNC operations that require coordinated operations of the X & Z axis, including making tapers and radii. Source code for conversational lathe CNC can be found in [menu_conversational.cpp](https://github.com/audreycui/CNC_Lathe_Marlin/blob/bugfix-2.1.x/Marlin/src/lcd/menu/menu_conversational.cpp) and [conversational.h](https://github.com/audreycui/CNC_Lathe_Marlin/blob/bugfix-2.1.x/Marlin/src/module/conversational.h)

(rest of readme is from the original Marlin repository)
<p align="center"><img src="buildroot/share/pixmaps/logo/marlin-outrun-nf-500.png" height="250" alt="MarlinFirmware's logo" /></p>

<h1 align="center">Marlin 3D Printer Firmware</h1>

<p align="center">
    <a href="/LICENSE"><img alt="GPL-V3.0 License" src="https://img.shields.io/github/license/marlinfirmware/marlin.svg"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/graphs/contributors"><img alt="Contributors" src="https://img.shields.io/github/contributors/marlinfirmware/marlin.svg"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/releases"><img alt="Last Release Date" src="https://img.shields.io/github/release-date/MarlinFirmware/Marlin"></a>
    <a href="https://github.com/MarlinFirmware/Marlin/actions/workflows/ci-build-tests.yml"><img alt="CI Status" src="https://github.com/MarlinFirmware/Marlin/actions/workflows/ci-build-tests.yml/badge.svg"></a>
    <a href="https://github.com/sponsors/thinkyhead"><img alt="GitHub Sponsors" src="https://img.shields.io/github/sponsors/thinkyhead?color=db61a2"></a>
    <br />
    <a href="https://bsky.app/profile/marlinfw.org"><img alt="Follow marlinfw.org on Bluesky" src="https://img.shields.io/static/v1?label=&message=Follow @marlinfw.org&color=1185FE&logo=bluesky&logoColor=white"></a>
    <a href="https://fosstodon.org/@marlinfirmware"><img alt="Follow MarlinFirmware on Mastodon" src="https://img.shields.io/mastodon/follow/109450200866020466?domain=https%3A%2F%2Ffosstodon.org&logoColor=%2300B&style=social"></a>
</p>

Additional documentation can be found at the [Marlin Home Page](//marlinfw.org/).
Please test this firmware and let us know if it misbehaves in any way. Volunteers are standing by!

## Contributors

Marlin is constantly improving thanks to a huge number of contributors from all over the world bringing their specialties and talents. Huge thanks are due to [all the contributors](//github.com/MarlinFirmware/Marlin/graphs/contributors) who regularly patch up bugs, help direct traffic, and basically keep Marlin from falling apart. Marlin's continued existence would not be possible without them.

Marlin Firmware original logo design by Ahmet Cem TURAN [@ahmetcemturan](//github.com/ahmetcemturan).

## Project Leadership

Name|Role|Link|Donate
----|----|----|----
🇺🇸 Scott Lahteine|Project Lead|[[@thinkyhead](//github.com/thinkyhead)]|[💸 Donate](//marlinfw.org/docs/development/contributing.html#donate)
🇺🇸 Roxanne Neufeld|Admin|[[@Roxy-3D](//github.com/Roxy-3D)]|
🇺🇸 Keith Bennett|Admin|[[@thisiskeithb](//github.com/thisiskeithb)]|[💸 Donate](//github.com/sponsors/thisiskeithb)
🇺🇸 Jason Smith|Admin|[[@sjasonsmith](//github.com/sjasonsmith)]|
🇧🇷 Victor Oliveira|Admin|[[@rhapsodyv](//github.com/rhapsodyv)]|
🇬🇧 Chris Pepper|Admin|[[@p3p](//github.com/p3p)]|
🇳🇿 Peter Ellens|Admin|[[@ellensp](//github.com/ellensp)]|[💸 Donate](//ko-fi.com/ellensp)
🇺🇸 Bob Kuhn|Admin|[[@Bob-the-Kuhn](//github.com/Bob-the-Kuhn)]|
🇳🇱 Erik van der Zalm|Founder|[[@ErikZalm](//github.com/ErikZalm)]|

## License

Marlin is published under the [GPL license](/LICENSE) because we believe in open development. The GPL comes with both rights and obligations. Whether you use Marlin firmware as the driver for your open or closed-source product, you must keep Marlin open, and you must provide your compatible Marlin source code to end users upon request. The most straightforward way to comply with the Marlin license is to make a fork of Marlin on Github, perform your modifications, and direct users to your modified fork.
