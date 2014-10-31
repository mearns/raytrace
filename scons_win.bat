@rem Helper script to build under windows

@rem add pkg-config env var
SET PKG_CONFIG_PATH=C:\gtk+-bundle_2.24.10-20120208_win32\lib\pkgconfig

@rem add gtk bins to PATH
SET PATH=%PATH%;C:\gtk+-bundle_2.24.10-20120208_win32\bin

scons
