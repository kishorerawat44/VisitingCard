# K3D Test — first After Effects plugin prototype

`K3D Test.aex` is an experimental Windows x64 effect plugin. It copies the input layer and inverts its RGB colors while retaining alpha. It is **not** a 3D importer, renderer, or an Element 3D replacement. It has no custom controls. This first build demonstrates the intended plugin format and basic render callback only.

The binary was cross-compiled on Linux with Zig 0.14.0-dev.2456 against a 2023-era After Effects SDK mirror. It exports `EffectMain` and contains a PiPL resource. It has **not been run in After Effects or Windows**, and compatibility with After Effects 2026 is not verified. Do not use it in important projects until tested. If After Effects reports a plugin error or crashes, remove the file and restart it.

## Try it in After Effects 2026 on Windows x64

1. Quit After Effects.
2. Copy `K3D Test.aex` to the After Effects 2026 `Support Files\Plug-ins` folder, normally inside `C:\Program Files\Adobe\Adobe After Effects 2026\`. Administrator permission may be required.
3. Restart After Effects and create a new test composition with an image or solid layer.
4. Find **K3D Test** under **Effect > K3D** (or search the Effects & Presets panel) and apply it to the layer. The colors should invert. The plugin processes 8-bpc pixels; use an 8-bpc test project.
5. To uninstall, quit After Effects, delete `K3D Test.aex`, and reopen After Effects.

If it does not appear, first verify the path and restart After Effects. Report any error messages and screenshots for investigation.

## Source and build notes

`K3D.cpp` is the plugin source, and `make_pipl.py` creates Windows PiPL metadata consumed by `pipl.rc`. Adobe's SDK is not included here. The build used the example SDK headers from https://github.com/yergin/after-effects-sdk and Zig's Windows cross-compiler. For production, obtain an officially licensed, current SDK from Adobe, verify licensing and compatibility, and build and test with the supported Windows toolchain.

Example build on Linux with Zig installed, from this directory, replacing `SDK_EXAMPLES` with the path to the SDK's `Examples` directory:

```sh
python3 make_pipl.py
zig rc /fo pipl.res pipl.rc
zig c++ -target x86_64-windows-gnu -shared -O2 \
  -DMSWindows -DWIN32 -D_WINDOWS \
  -I"$SDK_EXAMPLES/Headers" -I"$SDK_EXAMPLES/Headers/SP" \
  -I"$SDK_EXAMPLES/Headers/Win" -I"$SDK_EXAMPLES/Util" \
  K3D.cpp pipl.res -o 'K3D Test.aex'
```

The PiPL declares SDK effect API version 13.28; if changing SDK versions, check the version and metadata against the SDK headers. Keep the PiPL version and flags consistent with `GLOBAL_SETUP`.
