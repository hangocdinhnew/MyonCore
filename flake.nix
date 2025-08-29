{
  description = "A Nix-flake-based C/C++ development environment";

  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";

  outputs = inputs:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" ];
      forEachSupportedSystem = f: inputs.nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import inputs.nixpkgs { inherit system; };
      });
    in
    {
      devShells = forEachSupportedSystem ({ pkgs }: 
      let
          buildDeps = with pkgs; [
            clang-tools
            cmake
            pkg-config
            ninja
            cppcheck
            doxygen
            makeWrapper
            lua-language-server
            xmake
          ];

          runtimeDeps = with pkgs; [
            wayland
            wayland-protocols
            wayland-scanner
            xorg.libX11
            xorg.libXrandr
            xorg.libXcursor
            xorg.libXi
            alsa-lib
            libGL
            libdrm
            libdecor
            sdl3
            libffi
            libunwind
            libusb1
            xorg.libXext
            libxkbcommon
            vulkan-loader
            wgpu-native
            spdlog
            glm
          ];

          libraryPath = pkgs.lib.makeLibraryPath runtimeDeps;
      in
      {
        default = pkgs.mkShell.override
          {
            # Override stdenv in order to change compiler:
            # stdenv = pkgs.clangStdenv;
          }
          {
            nativeBuildInputs = with pkgs; [
            ] ++ buildDeps;

            buildInputs = with pkgs; [
            ] ++ runtimeDeps;

            shellHook = ''
              export CMAKE_INSTALL_RPATH=${libraryPath}
            '';
          };
      });
    };
}

