{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs =
    with pkgs; [
        pkgsCross.i686-embedded.buildPackages.gcc
        pkgsCross.i686-embedded.buildPackages.grub2
		mtools
    ];
}
