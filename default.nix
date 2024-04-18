
{ pkgs ? import <nixpkgs> {} }:
let g = path: type: type != "directory" || baseNameOf path != "build";
in
(pkgs.openttd.overrideAttrs (a : {
  src = pkgs.lib.cleanSourceWith {
    src = ./. ;
    filter = g;
  };
}))
