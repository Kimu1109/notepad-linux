# Release Branch

This branch is for building a .deb file.

## How To Build Notepad .deb File.

#### Requirement Packages

- build-essential
- cmake
- qt6-base-dev
- qt6-tools-dev
- qt6-tools-dev-tools
- qt6-l10n-tools
- libicu-dev

To Install Them :

```bash
apt install -y \ 
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    qt6-l10n-tools \
    libicu-dev
```



#### Way To Build

Run This Command :

```bash
sh ./build_deb.sh
```

When the building finished by successful, a .deb file may be in a build directory.

