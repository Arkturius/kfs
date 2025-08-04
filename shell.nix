{ ... }:

let
  pkgs = import (fetchTarball {
    url = "https://github.com/NixOS/nixpkgs/archive/029dea9aaacf.tar.gz";
  }) {};
in
pkgs.mkShell {
  nativeBuildInputs =
    with pkgs; [
        pkgsCross.i686-embedded.buildPackages.gcc
        pkgsCross.i686-embedded.buildPackages.grub2
		mtools
		libisoburn
    ];
  shellHook = ''
	make fclean all
  '';
}
